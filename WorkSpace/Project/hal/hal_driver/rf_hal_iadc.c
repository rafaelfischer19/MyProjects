/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_iadc.c
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

/******************************************************************************
 *							INCLUDES
 *****************************************************************************/
/* ---------------------- C language standard library ------------------------*/
/* ---------------------- Stack include --------------------------------------*/
#include PLATFORM_HEADER
#include "em_cmu.h"
#include "em_iadc.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_hal_iadc.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
// Set CLK_ADC to 10 MHz
#define CLK_SRC_ADC_FREQ        20000000  // CLK_SRC_ADC
#define CLK_ADC_FREQ            10000000  // CLK_ADC - 10 MHz max in normal mode

#define IADC_INPUT_0_BUS          CDBUSALLOC
#define IADC_INPUT_0_BUSALLOC     GPIO_CDBUSALLOC_CDEVEN0_ADC0
#define IADC_INPUT_1_BUS          CDBUSALLOC
#define IADC_INPUT_1_BUSALLOC     GPIO_CDBUSALLOC_CDODD0_ADC0

#define IADC_INPUT_2_BUS          ABUSALLOC
#define IADC_INPUT_2_BUSALLOC     GPIO_ABUSALLOC_AEVEN0_ADC0
#define IADC_INPUT_3_BUS          ABUSALLOC
#define IADC_INPUT_3_BUSALLOC     GPIO_ABUSALLOC_AODD0_ADC0

#define IADC_INPUT_4_BUS          BBUSALLOC
#define IADC_INPUT_4_BUSALLOC     GPIO_BBUSALLOC_BEVEN0_ADC0
#define IADC_INPUT_5_BUS          BBUSALLOC
#define IADC_INPUT_5_BUSALLOC     GPIO_BBUSALLOC_BODD0_ADC0

/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static boolean isIadcInit = false;

/* --------------------- Private function prototypes -------------------------*/
static void gs_iadc_init(void);
static int32u gs_iadc_read_vin(void);
static int32u gs_iadc_single_read(int8u port, int8u pin, int8u ref,
		boolean inMilliVolts);

/* --------------------- Public objects --------------------------------------*/
/*
 * 'Object' created to encapsulate the class
 */
static const st_iadc_class_t iadcClass = {
		.initIadc = &gs_iadc_init,
		.getVdd = &gs_iadc_read_vin,
		.simpleRead = &gs_iadc_single_read
};


/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

const st_iadc_class_t *gs_hal_iadc_class(void)
{
	return &iadcClass;
}

/**
 * @brief		: Put your description here
 * @pre-cond.	: Conditions needed to APIs/drivers work, if it exists.
 * @post-cond.	: Conditions achieved after API/drivers executed, if it exists.
 * @param[in]	: <name> <description> Fill in this field where there is or is not an input parameter.
 * @param[out]	: <name> <description> Fill this field where there is or is not an output parameter.
 * @return		: <Return value description>.Fill this field wherever there is or not returning
 */
static void gs_iadc_init(void)
{
	if (!isIadcInit) {
		// Declare initialization structures
		IADC_Init_t init = IADC_INIT_DEFAULT;
		IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
		IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
		// Single input structure
		IADC_SingleInput_t singleInput = IADC_SINGLEINPUT_DEFAULT;

		/*
		 * Enable IADC0 and GPIO register clock.
		 *
		 * Note: On EFR32xG21 devices, CMU_ClockEnable() calls have no effect
		 * as clocks are enabled/disabled on-demand in response to peripheral
		 * requests.  Deleting such lines is safe on xG21 devices and will
		 * reduce provide a small reduction in code size.
		 */
		CMU_ClockEnable(cmuClock_IADC0, true);
		CMU_ClockEnable(cmuClock_GPIO, true);

		// Use the FSRC0 as the IADC clock so it can run in EM2
		CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);

		// Set the prescaler needed for the intended IADC clock frequency
		init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ,
				0);

		// Shutdown between conversions to reduce current
		init.warmup = iadcWarmupNormal;

		// Configuration 0 is used by both scan and single conversions by default
		// Use unbuffered AVDD as reference
		initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
		initAllConfigs.configs[0].vRef = IADC_getReferenceVoltage(iadcCfgReferenceVddx);

		initAllConfigs.configs[1].reference = iadcCfgReferenceInt1V2;
		initAllConfigs.configs[1].vRef = IADC_getReferenceVoltage(iadcCfgReferenceInt1V2);

		// Divides CLK_SRC_ADC to set the CLK_ADC frequency for desired sample rate
		initAllConfigs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(
		IADC0,
		CLK_ADC_FREQ, 0, iadcCfgModeNormal, init.srcClkPrescale);
		initAllConfigs.configs[1].adcClkPrescale = IADC_calcAdcClkPrescale(
		IADC0,
		CLK_ADC_FREQ, 0, iadcCfgModeNormal, init.srcClkPrescale);

		// Initialize IADC
		IADC_init(IADC0, &init, &initAllConfigs);

		// Initialize a single-channel conversion
		IADC_initSingle(IADC0, &initSingle, &singleInput);

		// Allocate the analog bus for ADC0 inputs
		GPIO->IADC_INPUT_0_BUS |= IADC_INPUT_0_BUSALLOC;
		GPIO->IADC_INPUT_1_BUS |= IADC_INPUT_1_BUSALLOC;
		GPIO->IADC_INPUT_2_BUS |= IADC_INPUT_2_BUSALLOC;
		GPIO->IADC_INPUT_3_BUS |= IADC_INPUT_3_BUSALLOC;
		GPIO->IADC_INPUT_4_BUS |= IADC_INPUT_4_BUSALLOC;
		GPIO->IADC_INPUT_5_BUS |= IADC_INPUT_5_BUSALLOC;

		// Clear any previous interrupt flags
		IADC_clearInt(IADC0, _IADC_IF_MASK);

		IADC0->EN_CLR = IADC_EN_EN;

		isIadcInit = true;
	}
}

/**
 * @brief		: Put your description here
 * @pre-cond.	: Conditions needed to APIs/drivers work, if it exists.
 * @post-cond.	: Conditions achieved after API/drivers executed, if it exists.
 * @param[in]	: <name> <description> Fill in this field where there is or is not an input parameter.
 * @param[out]	: <name> <description> Fill this field where there is or is not an output parameter.
 * @return		: <Return value description>.Fill this field wherever there is or not returning
 */
static int32u gs_iadc_read_vin(void)
{
	int32u result = IADC_ERROR;
	if (isIadcInit) {
		//Enable IADC
		IADC0->EN_SET = IADC_EN_EN;

		IADC_SingleInput_t pstSingleInput;
		pstSingleInput.posInput = iadcPosInputDvdd;
		pstSingleInput.negInput = iadcNegInputGnd;
		//The IADC are configured to work with internal reference and VDD
		pstSingleInput.configId = iadcInternalRefe1V2;

		//update the single input
		IADC_updateSingleInput(IADC0, &pstSingleInput);

		// Start single
		IADC_command(IADC0, iadcCmdStartSingle);

		// Wait for conversion to be complete
		while ((IADC0->STATUS
				& (_IADC_STATUS_CONVERTING_MASK | _IADC_STATUS_SINGLEFIFODV_MASK))
				!= IADC_STATUS_SINGLEFIFODV)
			; //while combined status bits 8 & 6 don't equal 1 and 0 respectively

		//Disable
		IADC0->EN_CLR = IADC_EN_EN;

		//get result
		// Calculate input voltage:
		// for Vref = VBGR = 1,21V, and with analog gain = 1x, and Voltage
		// at Positive Input DVVD /4 represents
		// 4.84V full scale IADC range.
		result = IADC_pullSingleFifoResult(IADC0).data;
		result = (int32u) ((double) (((double) result * 4.84) / (double) 0xFFF)
				* (double) 1000.0);
	}

	return result;
}

/**
 * @brief		: Put your description here
 * @pre-cond.	: Conditions needed to APIs/drivers work, if it exists.
 * @post-cond.	: Conditions achieved after API/drivers executed, if it exists.
 * @param[in]	: <name> <description> Fill in this field where there is or is not an input parameter.
 * @param[out]	: <name> <description> Fill this field where there is or is not an output parameter.
 * @return		: <Return value description>.Fill this field wherever there is or not returning
 */
static int32u gs_iadc_single_read(int8u port, int8u pin, int8u ref,
		boolean inMilliVolts)
{
	static volatile int32u iadcResult = 0;
	if (isIadcInit && GPIO_PORT_PIN_VALID(port, pin)
			&& ref <= iadcInternalRefe1V2) {

		IADC_SingleInput_t pstSingleInput = IADC_SINGLEINPUT_DEFAULT;
		pstSingleInput.posInput = IADC_portPinToPosInput(port,pin);
		pstSingleInput.negInput = iadcNegInputGnd;
		//The IADC are configured to work with internal reference and VDD
		pstSingleInput.configId = ref;

		//Enable IADC
		IADC0->EN_SET = IADC_EN_EN;
		//update the single input
		IADC_updateSingleInput(IADC0, &pstSingleInput);

		// Start single
		IADC_command(IADC0, iadcCmdStartSingle);

		// Wait for conversion to be complete
		while ((IADC0->STATUS
				& (_IADC_STATUS_CONVERTING_MASK | _IADC_STATUS_SINGLEFIFODV_MASK))
				!= IADC_STATUS_SINGLEFIFODV)
			; //while combined status bits 8 & 6 don't equal 1 and 0 respectively

		//Disable
		IADC0->EN_CLR = IADC_EN_EN;

		//get result
		iadcResult = IADC_pullSingleFifoResult(IADC0).data;

		//if we want this in mV
		if(inMilliVolts){
			if (ref == iadcVddRef) {
				iadcResult = (int32u) ((double) (((double) iadcResult
						* (double) (gs_iadc_read_vin() / (double) 1000.0))
						/ (double) 0xFFF) * (double) 1000.0);
			}else{
				iadcResult = (int32u)((double)(((double)iadcResult * 1.21)/(double)0xFFF)*(double) 1000.0);
			}
		}

		return iadcResult;
	}

	return IADC_ERROR;
}




