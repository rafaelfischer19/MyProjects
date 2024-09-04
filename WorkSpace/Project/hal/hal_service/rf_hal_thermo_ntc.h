/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_thermo_ntc.h
 * @author	rafael.fischer
 * @date	1 de ago. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_THERMO_NTC_H_
#define GS_HAL_THERMO_NTC_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
#define GS_NTC


/* -------------------------- Typedef ----------------------------------------*/
typedef struct {
		uint8_t (*getTempCelsNtc) (double *prNtc, uint16_t *pTempResult);

		uint8_t (*getNctTempCels) (uint16_t *pTempCels, double *pNtcResult);

		uint8_t (*getRange) (double *maxTemp, double *minTemp);

}st_temp_ntc_class_t;;

/* -------------------------- Public objects ---------------------------------*/

const st_temp_ntc_class_t *gs_temp_ntc_class_t(void);

#endif /* GS_HAL_THERMO_NTC_H_ */
