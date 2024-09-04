/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_board.c
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

/* ---------------------- Others include -------------------------------------*/
#include "gs_board.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
/* --------------------- Public objects --------------------------------------*/
const st_board_class_t boardClass = {
	.deviceMonitor = &gs_hal_device_monitor_class,
	.resetButton = &gs_hal_reset_button_class,
	.smokeSample = &gs_hal_smoke_sample_class,
	.visualIndication = &gs_hal_visual_indication_class,
	.triggertButton = &gs_hal_trigger_class,
	.thermoSensor = &gs_hal_thermo_sensor_class
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

const st_board_class_t* gs_board_class(void)
{
	return &boardClass;
}
