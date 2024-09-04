/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_smoke_detector.c
 * @author	rafael.fischer
 * @date	24 de jul. de 2024
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
#include "af.h"

/* ---------------------- Others include -------------------------------------*/
#include "../../util/gs_board.h"
#include "../../zigbee_platform/zp_zed_network/gs_zp_zed_network.h"
#include "../../zigbee_platform/zp_cluster_interface/gs_zp_basic_cluster_server.h"

#include "../../component/cmp_device_monitor/gs_cmp_device_monitor.h"
#include "../../component/cmp_reset_button/gs_cmp_reset_button.h"
#include "../../component/cmp_smoke_detector/gs_cmp_smoke_detector.h"
#include "gs_app_smoke_detector.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
/* --------------------- Public objects --------------------------------------*/

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

static uint8_t gs_app_smoke_board_init(
		const st_smoke_detector_board_t *board, bool isEnableCli)
{
	uint8_t status = EMBER_SUCCESS;

	status |= gs_board_class()->visualIndication()->init(
			board->viPort, board->viPin, board->viLedOffState, isEnableCli);

	status |= gs_board_class()->deviceMonitor()->init(board->detachPort,
														board->detachPin,
														board->detachInitState);

	status |= gs_board_class()->resetButton()->init(board->resetBtnPort,
													board->resetBtnPin,
													board->resetBtnPressState);

	status |= gs_board_class()->smokeSample()->init(board->enableCiPort,
													board->enableCiPin,
													board->irLedPort,
													board->irLedPin,
													board->inAnalogPort,
													board->inAnalogPin);

	return status;
}

static uint8_t gs_app_smoke_zigbee_init(
		const st_smoke_zigbee_info_t *zigbeeInfo, bool isEnableCli)
{
	uint8_t status = EMBER_SUCCESS;

	status |= gs_zp_zed_network_class()->init(isEnableCli);

	uint8_t buffer[32] = { 0, };

	MEMCOPY(buffer, &zigbeeInfo->mfgName[0], zigbeeInfo->mfgName[0]+1);

	status |= gs_zp_basic_cluster_server_class()->setMfgName(&buffer[0]);

	MEMSET(buffer, 0, 32);
	MEMCOPY(buffer, &zigbeeInfo->model[0], zigbeeInfo->model[0]+1);

	status |= gs_zp_basic_cluster_server_class()->setModel(&buffer[0]);

	return status;
}

uint8_t gs_app_smoke_detector_init(const st_smoke_detector_board_t *board,
									const st_smoke_zigbee_info_t *zigbeeInfo,
									bool isEnableCli)
{
	uint8_t status = gs_app_smoke_board_init(board, isEnableCli);

	if(status){
		return status;
	}

	status |= gs_app_smoke_zigbee_init(zigbeeInfo, isEnableCli);

	status |= gs_cmp_device_monitor_init();
	status |= gs_cmp_reset_button_init();
	status |= gs_cmp_smoke_detector_init();

	return status;
}










