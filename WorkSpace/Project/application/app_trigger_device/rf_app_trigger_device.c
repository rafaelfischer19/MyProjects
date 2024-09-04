/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_trigger_device.c
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
#include "af.h"

/* ---------------------- Others include -------------------------------------*/
#include "../../util/gs_board.h"
#include "../../zigbee_platform/zp_zed_network/gs_zp_zed_network.h"
#include "../../zigbee_platform/zp_cluster_interface/gs_zp_basic_cluster_server.h"

#include "../../component/cmp_reset_button/gs_cmp_reset_button.h"
#include "../../component/cmp_device_monitor/gs_cmp_device_monitor.h"
#include "../../component/cmp_device_trigger/gs_cmp_trigger.h"
#include "gs_app_trigger_device.h"
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
static uint8_t gs_app_trigger_board_init(const st_trigger_device_board_t *board,
										bool isEnableCli)
{
	uint8_t status = EMBER_SUCCESS;

	//Initialize visual indication
	status |= gs_board_class()->visualIndication()->init(
			board->viPort, board->viPin, board->viLedOffState, isEnableCli);

	//Initialize device monitor
	status |= gs_board_class()->deviceMonitor()->init(board->detachPort,
														board->detachPin,
														board->detachInitState);

	//Initialize the reset button
	status |= gs_board_class()->resetButton()->init(board->resetBtnPort,
													board->resetBtnPin,
													board->resetBtnPressState);

	//Initialize trigger device
	status |= gs_board_class()->triggertButton()->init(board->triggerPort,
													board->triggerPin,
													board->triggerState);

	return status;
}

static uint8_t gs_app_trigger_zigbee_init(
		const st_trigger_zigbee_info_t *zigbeeInfo, bool isEnableCli)
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

uint8_t gs_app_trigger_device_init(const st_trigger_device_board_t *board,
									const st_trigger_zigbee_info_t *zigbeeInfo,
									bool isEnableCli)
{
	uint8_t status = gs_app_trigger_board_init(board, isEnableCli);

	if (status) {
		return status;
	}

	status |= gs_app_trigger_zigbee_init(zigbeeInfo, isEnableCli);

	status |= gs_cmp_device_monitor_init();
	status |= gs_cmp_reset_button_init();
	status |= gs_cmp_trigger_button_init();
	//status |= gs_cmp_smoke_detector_init();

	return status;
}
