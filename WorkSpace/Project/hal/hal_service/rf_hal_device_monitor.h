/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_device_monitor.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_DEVICE_MONITOR_H_
#define GS_HAL_DEVICE_MONITOR_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
typedef void (*gs_detach_callback)(uint8_t status);

typedef struct {
	uint8_t (*init)(uint8_t detachPort, uint8_t detachPin,
					uint8_t detachInitState);

	uint8_t (*detachRegister)(gs_detach_callback callback);

	uint8_t (*getBattery)(uint16_t *battery);
} st_hal_device_monitor_class_t;

/* -------------------------- Public objects ---------------------------------*/

const st_hal_device_monitor_class_t *gs_hal_device_monitor_class(void);


#endif /* GS_HAL_DEVICE_MONITOR_H_ */
