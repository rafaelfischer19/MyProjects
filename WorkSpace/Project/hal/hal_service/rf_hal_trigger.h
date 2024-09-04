/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_trigger.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_TRIGGER_H_
#define GS_HAL_TRIGGER_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/
typedef void (*gs_hal_trigger_callback)(uint8_t status);

typedef struct {
	uint8_t (*init)(uint8_t triggerPort, uint8_t triggerPin,
					uint8_t triggerIniState);

	uint8_t (*triggerCb)(gs_hal_trigger_callback callback);
} st_hal_trigger_class_t;

const st_hal_trigger_class_t *gs_hal_trigger_class(void);


#endif /* GS_HAL_TRIGGER_H_ */
