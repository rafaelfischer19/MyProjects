/**
 ******************************************************************************
 * @section company_section Company
 *
 *
 * @file	rf_app_end_device.h
 * @author	rafael.fischer
 * @date	28 de ago. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_END_DEVICE_H_
#define GS_APP_END_DEVICE_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

typedef struct {
	//Visual Indication
	uint8_t viPort;
	uint8_t viPin;
	uint8_t viLedOffState;

	//Device Monitor
	uint8_t detachPort;
	uint8_t detachPin;
	uint8_t detachInitState;

	//Reset Button
	uint8_t resetBtnPort;
	uint8_t resetBtnPin;
	uint8_t resetBtnPressState;

	//thermo sensor
	uint8_t thermoPort;
	uint8_t thermoPin;
	uint8_t thermoInitState;
	uint8_t inAnalogPort;
	uint8_t inAnalogPin;

	//Smoke Sample
	uint8_t enableCiPort;
	uint8_t enableCiPin;
	uint8_t irLedPort;
	uint8_t irLedPin;
	uint8_t inAnalogPortSmoke;
	uint8_t inAnalogPinSmoke;

} st_thermo_sensor_board_t;

typedef struct {
	uint8_t mfgName[13];
	uint8_t model[8];
} st_thermo_zigbee_info_t;

uint8_t gs_app_thermo_sensor_init(const st_thermo_sensor_board_t *board,
									const st_thermo_zigbee_info_t *zigbeeInfo,
									bool isEnableCli);

#endif /* GS_APP_END_DEVICE_H_ */
