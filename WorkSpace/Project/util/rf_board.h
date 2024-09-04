/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_board.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_BOARD_H_
#define GS_BOARD_H_

/* -------------------------- Includes ---------------------------------------*/
#include "../hal/hal_service/gs_hal_visual_indication.h"
#include "../hal/hal_service/gs_hal_device_monitor.h"
#include "../hal/hal_service/gs_hal_reset_button.h"
#include "../hal/hal_service/gs_hal_trigger.h"
#include "../hal/hal_service/gs_hal_smoke_sample.h"
#include "../hal/hal_service/gs_hal_thermo_sensor.h"

/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
typedef struct {
	const st_hal_visual_indication_class_t* (*visualIndication)(void);

	const st_hal_device_monitor_class_t* (*deviceMonitor)(void);

	const st_hal_reset_button_class_t* (*resetButton)(void);

	const st_hal_smoke_sample_class_t* (*smokeSample)(void);

	const st_hal_trigger_class_t* (*triggertButton)(void);

	const st_hal_thermo_sensor_class_t* (*thermoSensor)(void);

} st_board_class_t;

/* -------------------------- Public objects ---------------------------------*/

const st_board_class_t* gs_board_class(void);

#endif /* GS_BOARD_H_ */
