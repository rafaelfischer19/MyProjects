/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_install_code.c
 * @author	rafael.fischer
 * @date	17 de jul. de 2024
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
#include "af.h"
#include "sl_token_manager.h"
#include "mac-types.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_zp_install_code.h"


/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
#define GS_CODE_SIZE 16

/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
tokTypeMfgInstallationCode installCodeInternal = {
		.flags = 0xFFFF,
		.crc = 0xFFFF,
		.value = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};	//Local Install code

/* --------------------- Private function prototypes -------------------------*/
static void gs_drv_config_internal_code(void);
static int8u getGlobalsonic_install_code(EmberEUI64 eui64,
		tokTypeMfgInstallationCode *pstPointToCopy, int8u size);
static void gs_drv_install_code_token_print(void);
static boolean isAll_elements_ff(int8u *puiPointToCheck, int16u size);
static int8u gs_drv_generate_installcode_from_eui64(EmberEUI64 eui64,
		int8u *puiBuffToCopyInstallCode, int8u uiSizeBuffer);

/* --------------------- Public objects --------------------------------------*/
static const st_install_code_class_t gs_install_code = {
		//Functions
		.configure = &gs_drv_config_internal_code,
		.getinstallCodeKeyFromEui64 = &gs_drv_generate_installcode_from_eui64
};


/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_install_code_class_t *gs_zp_install_code_class(void)
{
	return &gs_install_code;
}


// Helper Functions
// Reverse the bits in a byte
static uint8_t reverse(uint8_t b)
{
#if defined(EZSP_HOST) || defined(BOARD_SIMULATION)
  return ((b * 0x0802UL & 0x22110UL) | (b * 0x8020UL & 0x88440UL)) * 0x10101UL >> 16;
#else
	return (__RBIT((uint32_t) b) >> 24); // Cortex function __RBIT uses uint32_t
#endif // EZSP_HOST
}

/**
 * @brief		: Generate an install code from a eui64
 * @param[in]	: eui64		The eui64 used for generate the install code
 * @param[out]	: puiBuffToCopyInstallCode	The Point to copy the generated code
 * @param[in]	: uiSizeBuffer	The size of buffer
 * @return		: (void)
 */
static int8u gs_drv_generate_installcode_from_eui64(EmberEUI64 eui64,
		int8u *puiBuffToCopyInstallCode, int8u uiSizeBuffer)
{
	int8u uiCodeLen = GS_CODE_SIZE + 2; //install code + 2 bytes CRC
	int8u uiBuff[GS_CODE_SIZE + 2] = { 0 };
	tokTypeMfgInstallationCode installCode;

	if (uiSizeBuffer == uiCodeLen) {
		MEMSET((int8u*)&installCode, 0xff, sizeof(tokTypeMfgInstallationCode));
		if (EMBER_SUCCESS
				== getGlobalsonic_install_code(eui64, &installCode,
						GS_CODE_SIZE)) {
			MEMCOPY(&uiBuff[0], (int8u* )&installCode.value, GS_CODE_SIZE);
			uiBuff[GS_CODE_SIZE] = LOW_BYTE(installCode.crc);
			uiBuff[GS_CODE_SIZE + 1] = HIGH_BYTE(installCode.crc);
			MEMCOPY(puiBuffToCopyInstallCode, uiBuff, uiCodeLen);
			return EMBER_SUCCESS;
		}
	}
	return 0xFF;
}

/**
 * @brief		: Generates the install code in the defined pattern for the
 * Global Sonic products. Configures Flags and calculates CRC for install code,
 * for this reason it is necessary to use the stack structure for
 * install code: "tokTypeMfgInstallationCode"
 *
 * @param[in]	: eui64		The eui64 to make a installcode
 *
 * @param[out]	: pstPointToCopy	pointer to where the value will be copied
 * 									must be: "tokTypeMfgInstallationCode".
 * @param[in]	: size	The size of install code value.
 * @return		: EMBER_SUCCESS	If its work, code error otherwise.
 */
static int8u getGlobalsonic_install_code(EmberEUI64 eui64,
		tokTypeMfgInstallationCode *pstPointToCopy, int8u size)
{
	if (size == GS_CODE_SIZE) {
		/*
		 * Install code Structure is defined by GlobalSonic is:
		 * 	1 - Hash the EUI64 for a 16 bytes key.
		 * 	2 - XOR With "Global_Sonic_EVO" 16 bytes String.
		 */
		int8u uiHashEui64[EMBER_ENCRYPTION_KEY_SIZE] = { 0, };
		int8u uiGlobalSonicKey[] = { 'G', 'l', 'o', 'b', 'a',	'l', '_', 'S',
									'o', 'n', 'i', 'c', '_', 'E', 'V', 'O' };

		if(emberAesHashSimple(sizeof(EmberEUI64), &eui64[0], uiHashEui64)
				!= EMBER_SUCCESS){
			return 0xFF;
		}

		int8u uiIntallCodeGS[EMBER_ENCRYPTION_KEY_SIZE] = { 0, };
		for(int8u i = 0; i < EMBER_ENCRYPTION_KEY_SIZE; i++){
			uiIntallCodeGS[i] = uiHashEui64[i] ^ uiGlobalSonicKey[i];
		}

		MEMCOPY(pstPointToCopy->value, uiIntallCodeGS, size);

		//See ::tokTypeMfgInstallationCode->flags for configuration
		pstPointToCopy->flags = 0x0006;

		int16u uiCrc = 0xFFFF;

		// Compute the CRC and verify that it matches.  The bit reversals, byte swap,
		// and ones' complement are due to differences between halCommonCrc16 and the
		// Smart Energy version.
		for (int8u index = 0; index < GS_CODE_SIZE; index++) {
			uiCrc = halCommonCrc16(reverse(pstPointToCopy->value[index]), uiCrc);
		}
		pstPointToCopy->crc = ~HIGH_LOW_TO_INT(reverse(LOW_BYTE(uiCrc)),
				reverse(HIGH_BYTE(uiCrc)));

		return EMBER_SUCCESS;
	}
	return 0xFF;
}

/**
 * @brief		: Check if the value or values are 0xFF
 * @param[in]	: puiPointToCheck	Data pointer to check.
 * @param[out]	: size	The size of data.
 * @return		: TRUE if all values are 0xFF, FALSE  otherwise
 */
static boolean isAll_elements_ff(int8u *puiPointToCheck, int16u size)
{
	for (int16u var = 0; var < size; var++) {
		if (*puiPointToCheck != 0xFF) {
			return false;
		}
		puiPointToCheck++;
	}
	return true;
}

/**
 * @brief		: Checks if two install codes are the same.
 * @param[in]	: puiCodeA	InstallCode data point to compare.
 * @param[in]	: puiCodeB	InstallCode data point to compare.
 * @param[out]	: size	The size of data.
 * @return		: TRUE if the InstallCode are equals, FALSE otherwise.
 */
boolean isInstall_code_equals(int8u *puiCodeA, int8u *puiCodeB, int16u size)
{
	for (int16u var = 0; var < size; var++) {
		if (*puiCodeA != *puiCodeB) {
			return false;
		}
		puiCodeA++;
		puiCodeB++;
	}
	return true;
}

/**
 * @brief		: It performs a configuration of the installation code in the
 * pattern defined by Global Sonic.
 *
 * @pre-cond.	: The install code memory region can only be written if all
 * values are FF's, so for the configuration to work correctly this condition
 * must be met, otherwise an "assert" will be generated.
 *
 * @post-cond.	: The install code configured in the pattern defined by
 * Global Sonic. Or "assert" condition some requirement is not met.
 */
static void gs_drv_config_internal_code(void)
{
	tokTypeMfgInstallationCode installCodeFormToken;

	//read Install Code fom token
	if (EMBER_SUCCESS
			== sl_token_get_manufacturing_data(
					TOKEN_MFG_INSTALLATION_CODE_ADDRESS, 0x7F,
					&installCodeFormToken,
					sizeof(tokTypeMfgInstallationCode))) {
		//read the local eui64 used for convert into install code
		EmberEUI64 eui64Local;
		MEMCOPY(eui64Local, emberGetEui64(), sizeof(EmberEUI64));
		getGlobalsonic_install_code(eui64Local,&installCodeInternal, GS_CODE_SIZE);

		if (isAll_elements_ff((int8u*) &installCodeFormToken,
				sizeof(tokTypeMfgInstallationCode))) {
			// if all elements are 0xFF we can configure
			sl_token_set_manufacturing_data(TOKEN_MFG_INSTALLATION_CODE_ADDRESS,
					&installCodeInternal, sizeof(tokTypeMfgInstallationCode));
			gs_drv_install_code_token_print();
			//reboot to apply changes
			halReboot();
		} else {
			if (!isInstall_code_equals((int8u*) &installCodeFormToken,
					(int8u*) &installCodeInternal,
					sizeof(tokTypeMfgInstallationCode))) {
				sl_zigbee_app_debug_print("%s: code is already configured\n",
						"ASSERT");
				gs_drv_install_code_token_print();
				//if not all 0xFF assert
				assert(
						isInstall_code_equals((int8u* )&installCodeFormToken,
								(int8u* )&installCodeInternal,
								sizeof(tokTypeMfgInstallationCode)));
			} else {
				sl_zigbee_app_debug_print("%s code is already configured\n",
						"install");
			}
		}
	} else {
		sl_zigbee_app_debug_print("%s: Can't read install code from token\n",
				"ERROR");
	}
}

/**
 * @brief      : Print the install code configured in NVM memory
 * @pre-cond.  : debug print enable for use "sl_zigbee_app_debug_print"
 */
static void gs_drv_install_code_token_print(void)
{
	tokTypeMfgInstallationCode installCode;

	//read the intall code from token
	if (EMBER_SUCCESS
			== sl_token_get_manufacturing_data(
					TOKEN_MFG_INSTALLATION_CODE_ADDRESS, 0x7F, &installCode,
					sizeof(tokTypeMfgInstallationCode))) {
		sl_zigbee_app_debug_print(
				"%s install code - Flags: 0x%2X InstallCode: \n", "print",
				installCode.flags);
		sl_zigbee_app_debug_print_buffer(installCode.value, 16, true);
		sl_zigbee_app_debug_print("\nCRC: 0x%2X \n", installCode.crc);
	} else {
		sl_zigbee_app_debug_print("%s: Install Code Read \n", "ERROR");
	}
}
