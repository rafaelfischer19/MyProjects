/**
 ******************************************************************************
 * @section company_section Company
 *
 *
 * @file	rf_zp_zed_network.c
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
#include "network-steering.h"
#include "find-and-bind-initiator.h"

/* ---------------------- Others include -------------------------------------*/
#include "../../cli/gs_cli_interface.h"
#include "../../hal/hal_service/gs_hal_visual_indication.h"
#include "../zp_common/gs_zp_install_code.h"
#include "gs_zp_zed_network.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static sl_zigbee_event_t scanNetworkEvent;

extern sl_cli_command_group_t gs_cli_zed_network_main_group;

static bool isStopSteering = false;

static st_vi_instance_t *pstViNetwork = NULL;

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_zp_zed_leave_network(void);
static uint8_t gs_zp_zed_stop_scan_network(void);
static uint8_t gs_zp_zed_start_scan_network(void);
static uint8_t gs_zp_zed_network_init(bool isEnableCli);

/* --------------------- Public objects --------------------------------------*/
static const st_zp_zed_network_class_t zpZedNetworkClass = {
	.init = &gs_zp_zed_network_init,
	.startScanNetwork = &gs_zp_zed_start_scan_network,
	.stopScanNetwork = &gs_zp_zed_stop_scan_network,
	.leaveNetwork = &gs_zp_zed_leave_network
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_zp_zed_network_class_t* gs_zp_zed_network_class(void)
{
	return &zpZedNetworkClass;
}


void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
													uint8_t totalBeacons,
													uint8_t joinAttempts,
													uint8_t finalState)
{
	sl_zigbee_app_debug_println("%s network %s: 0x%02X", "Join", "complete",
								status);

	if (status == EMBER_SUCCESS) {
		uint8_t statusStartFind = emberAfPluginFindAndBindInitiatorStart(
				emberAfPrimaryEndpoint());

		sl_zigbee_app_debug_println(" Network Association Complete.");
		sl_zigbee_app_debug_println(" Start Find and Binding: 0x%1X",
									statusStartFind);
	} else {
		if (!isStopSteering) {
			sl_zigbee_event_set_delay_ms(&scanNetworkEvent, 5000);
		}
		isStopSteering = false;
	}

	if (pstViNetwork != NULL) {
		pstViNetwork->setViState(pstViNetwork, enOff);
	}
}

/** @brief Find and Bind Complete
 *
 * This callback is fired by the initiator when the Find and Bind process is
 * complete.
 *
 * @param status Status code describing the completion of the find and bind
 * process Ver.: always
 */
void emberAfPluginFindAndBindInitiatorCompleteCallback(EmberStatus status)
{
	sl_zigbee_app_debug_println(" Start Find and Binding Complete: 0x%1X",
								status);

	if (pstViNetwork != NULL) {
		pstViNetwork->setViState(pstViNetwork, enOperation);
	}
}

/**
 * @brief		Leave network.
 * @return		0x00 if all good, code error otherwise.
 */
static uint8_t gs_zp_zed_leave_network(void)
{
	return emberLeaveNetwork();
}

/**
 * @brief		The event function to find a network.
 * @param		[in] event The event.
 */
static void gs_zp_zed_scan_network_event_func(sl_zigbee_event_t *event)
{
	if (pstViNetwork != NULL) {
		pstViNetwork->setViState(pstViNetwork, enScanNetwork);
	}
	gs_zp_zed_start_scan_network();
}

/**
 * @brief		Stop scan the network.
 * @return		0x00 if all good, code error otherwise.
 */
static uint8_t gs_zp_zed_stop_scan_network(void)
{
	isStopSteering = true;

	sl_zigbee_event_set_inactive(&scanNetworkEvent);

	return emberAfPluginNetworkSteeringStop();
}

/**
 * @brief		Start scan the network.
 * @return		0x00 if all good, code error otherwise.
 */
static uint8_t gs_zp_zed_start_scan_network(void)
{
	return emberAfPluginNetworkSteeringStart();
}

/**
 * @brief		Initialize the ZIGBEE sleepy device.
 * @param		[in] isEnableCli Enable the CLI.
 * @return		0x00 if all good, code error otherwise.
 */
static uint8_t gs_zp_zed_network_init(bool isEnableCli)
{
	sl_zigbee_event_init(&scanNetworkEvent, gs_zp_zed_scan_network_event_func);

	gs_zp_install_code_class()->configure();

	if (isEnableCli) {
		gs_cli_interface_class()->addGroup(&gs_cli_zed_network_main_group);
	}

	pstViNetwork = gs_hal_visual_indication_class()->new();

	if (pstViNetwork == NULL) {
		return EMBER_APPLICATION_ERROR_0;
	}

	return EMBER_SUCCESS;
}




















