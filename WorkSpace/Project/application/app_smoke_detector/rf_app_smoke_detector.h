/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_smoke_detector.h
 * @author	rafael.fischer
 * @date	24 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_SMOKE_DETECTOR_H_
#define GS_APP_SMOKE_DETECTOR_H_

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

	//Smoke Sample
	uint8_t enableCiPort;
	uint8_t enableCiPin;
	uint8_t irLedPort;
	uint8_t irLedPin;
	uint8_t inAnalogPort;
	uint8_t inAnalogPin;
} st_smoke_detector_board_t;

typedef struct {
	uint8_t mfgName[13];
	uint8_t model[8];
} st_smoke_zigbee_info_t;

uint8_t gs_app_smoke_detector_init(const st_smoke_detector_board_t *board,
									const st_smoke_zigbee_info_t *zigbeeInfo,
									bool isEnableCli);


#endif /* GS_APP_SMOKE_DETECTOR_H_ */
