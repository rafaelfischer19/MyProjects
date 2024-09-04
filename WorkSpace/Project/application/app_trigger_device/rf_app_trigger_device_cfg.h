/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_trigger_device_cfg.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_TRIGGER_DEVICE_CFG_H_
#define GS_APP_TRIGGER_DEVICE_CFG_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

const st_trigger_device_board_t stTriggerBoard = {
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

	//Trigger Device
	.triggerPort = gpioPortA,
	.triggerPin = 4u,
	.triggerState = 0u,
};

const st_trigger_zigbee_info_t stTriggerInfo = {
	.mfgName = {0x0C,'G','l','o','b','a','l',' ','S','o','n','i','c'},
	.model = {0x07,'E','V','O','3','0','0','1'}
};

#endif /* GS_APP_TRIGGER_DEVICE_CFG_H_ */
