/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_thermo_sensor.h
 * @author	rafael.fischer
 * @date	25 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_THERMO_SENSOR_H_
#define GS_HAL_THERMO_SENSOR_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/

typedef struct {
	uint8_t (*init)(uint8_t thermoPort, uint8_t thermoPin, uint8_t inAnalogPort, uint8_t inAnalogPin);
	uint16_t (*getThermoSensor)(void);
} st_hal_thermo_sensor_class_t;

/* -------------------------- Public objects ---------------------------------*/

const st_hal_thermo_sensor_class_t *gs_hal_thermo_sensor_class(void);

#endif /* GS_HAL_THERMO_SENSOR_H_ */






