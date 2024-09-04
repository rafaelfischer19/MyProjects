/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_thermo_detector_cfg.h
 * @author	rafael.fischer
 * @date	29 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_THERMO_DETECTOR_CFG_H_
#define GS_APP_THERMO_DETECTOR_CFG_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

const st_thermo_sensor_board_t stThermoBoard = {
	//Visual Indication
	.viPort = gpioPortD,
	.viPin = 1u,
	.viLedOffState = 1u,

	//Device Monitor
	.detachPort = gpioPortA,
	.detachPin = 0u,
	.detachInitState = 0u,

	//Reset Button
	.resetBtnPort = gpioPortA,
	.resetBtnPin = 3u,
	.resetBtnPressState = 0u,

	//thermo Sensor
	.thermoPort = gpioPortA,
	.thermoPin = 4u,
	.inAnalogPort = gpioPortC,
	.inAnalogPin = 3u,

};

const st_thermo_zigbee_info_t stThermoInfo = {
	.mfgName = {0x0C,'G','l','o','b','a','l',' ','S','o','n','i','c'},
	.model = {0x07,'E','V','O','4','0','0','0'}
};

#endif /* GS_APP_THERMO_DETECTOR_CFG_H_ */
