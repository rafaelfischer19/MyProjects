/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_cmp_device_monitor.c
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
#include "../../zigbee_platform/zp_cluster_interface/gs_zp_ias_zone_cluster_server.h"
#include "../../util/gs_board.h"
#include "gs_cmp_device_monitor.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
#define GS_LOW_BATTERY_VALUE	2500
#define GS_BATTERY_TICK_MS		1000

/* --------------------- Private typedef -------------------------------------*/
static sl_zigbee_event_t devMonitorEvent;
static st_vi_instance_t *pstDetachVi = NULL;
static st_vi_instance_t *pstLowBatteryVi = NULL;

/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
static void gs_cmp_battery_monitor_event_func(sl_zigbee_event_t *event);

/* --------------------- Public objects --------------------------------------*/

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
static void gs_cmp_battery_monitor_event_func(sl_zigbee_event_t *event)
{
	bool isLowBattery = false;
	bool isIasLowBattery = false;
	uint16_t battery = 0;

	gs_board_class()->deviceMonitor()->getBattery(&battery);

	isLowBattery = (bool) (battery <= GS_LOW_BATTERY_VALUE);

	gs_zp_ias_zone_cluster_server_class()->getBattery(emberAfPrimaryEndpoint(),
														&isIasLowBattery);

	if (isLowBattery != isIasLowBattery) {
		gs_zp_ias_zone_cluster_server_class()->setBattery(
				emberAfPrimaryEndpoint(), isLowBattery);
	}

	pstLowBatteryVi->setViState(pstLowBatteryVi,
								isLowBattery ? enFault : enOff);

	sl_zigbee_event_set_delay_ms(&devMonitorEvent, GS_BATTERY_TICK_MS);
}

static void gs_cmp_device_monitor_detach_callback(uint8_t status)
{
	gs_zp_ias_zone_cluster_server_class()->setTamper(emberAfPrimaryEndpoint(),
														(bool) status);

	pstDetachVi->setViState(pstDetachVi, (bool) status ? enFault : enOff);
}

uint8_t gs_cmp_device_monitor_init(void)
{
	sl_zigbee_event_init(&devMonitorEvent, gs_cmp_battery_monitor_event_func);
	sl_zigbee_event_set_delay_ms(&devMonitorEvent, GS_BATTERY_TICK_MS);

	pstDetachVi = gs_board_class()->visualIndication()->new();
	pstLowBatteryVi = gs_board_class()->visualIndication()->new();

	if (pstDetachVi == NULL || pstLowBatteryVi == NULL) {
		return EMBER_APPLICATION_ERROR_0;
	}

	uint8_t status = gs_board_class()->deviceMonitor()->detachRegister(
			gs_cmp_device_monitor_detach_callback);

	return status;
}











