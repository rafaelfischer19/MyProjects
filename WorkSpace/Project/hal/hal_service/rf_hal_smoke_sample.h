/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_smoke_sample.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_SMOKE_SAMPLE_H_
#define GS_HAL_SMOKE_SAMPLE_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
typedef struct {
	uint16_t ledOff;
	uint16_t ledOn;
}st_hal_smoke_data_t;

/* -------------------------- Public objects ---------------------------------*/

typedef struct {

	uint8_t (*init)(uint8_t enableCiPort, uint8_t enableCiPin,
					uint8_t irLedPort, uint8_t irLedPin, uint8_t inAnalogPort,
					uint8_t inAnalogPin);

	uint8_t (*getSample)(st_hal_smoke_data_t *pstSample);

}st_hal_smoke_sample_class_t;

/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_hal_smoke_sample_class_t *gs_hal_smoke_sample_class(void);


#endif /* GS_HAL_SMOKE_SAMPLE_H_ */
