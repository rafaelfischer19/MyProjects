/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_iadc.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_IADC_H_
#define GS_HAL_IADC_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
#define IADC_ERROR			0xFFFFFFFF
typedef enum {
	iadcVddRef = 0,
	iadcInternalRefe1V2 = 1,
} en_iadc_reference_t;

typedef struct {

	//Initialize the IADC to work with single read
	void (*initIadc)(void);

	//Read AD value from a source - return AD conversion, value in mV or Error
	int32u (*simpleRead)(int8u port, int8u pin, int8u ref, boolean inMilliVolts);

	//Read the Internal power supply (VDD)
	int32u (*getVdd)(void);

} st_iadc_class_t;



/* -------------------------- Public objects ---------------------------------*/

const st_iadc_class_t *gs_hal_iadc_class(void);


#endif /* GS_HAL_IADC_H_ */
