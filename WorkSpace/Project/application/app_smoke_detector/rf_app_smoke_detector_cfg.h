/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_smoke_detector_cfg.h
 * @author	rafael.fischer
 * @date	24 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_SMOKE_DETECTOR_CFG_H_
#define GS_APP_SMOKE_DETECTOR_CFG_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

const st_smoke_detector_board_t stSmokeBoard = {
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

	//Smoke Sample
	.enableCiPort = gpioPortA,
	.enableCiPin = 4u,
	.irLedPort = gpioPortC,
	.irLedPin = 2u,
	.inAnalogPort = gpioPortA,
	.inAnalogPin = 6u,
};

const st_smoke_zigbee_info_t stSmokeInfo = {
	.mfgName = {0x0C,'G','l','o','b','a','l',' ','S','o','n','i','c'},
	.model = {0x07,'E','V','O','4','0','0','0'}
};

#endif /* GS_APP_SMOKE_DETECTOR_CFG_H_ */
