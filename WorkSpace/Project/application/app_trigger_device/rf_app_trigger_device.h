/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_trigger_device.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_TRIGGER_DEVICE_H_
#define GS_APP_TRIGGER_DEVICE_H_

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

	//Trigger Device
	uint8_t triggerPort;
	uint8_t triggerPin;
	uint8_t triggerState;
} st_trigger_device_board_t;

typedef struct {
	uint8_t mfgName[13];
	uint8_t model[8];
} st_trigger_zigbee_info_t;

uint8_t gs_app_trigger_device_init(const st_trigger_device_board_t *board,
									const st_trigger_zigbee_info_t *zigbeeInfo,
									bool isEnableCli);


#endif /* GS_APP_TRIGGER_DEVICE_H_ */
