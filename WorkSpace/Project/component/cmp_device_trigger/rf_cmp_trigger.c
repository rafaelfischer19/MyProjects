/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_cmp_trigger.c
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
#include "gs_cmp_trigger.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
static st_vi_instance_t *pstTriggerVi = NULL;

/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
/* --------------------- Public objects --------------------------------------*/

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

static void gs_cmp_device_monitor_trigger_callback(uint8_t status)
{
	gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(),
														(bool) status);

	pstTriggerVi->setViState(pstTriggerVi, (bool) status ? enFire : enOff);
}

uint8_t gs_cmp_trigger_button_init(void)
{


	pstTriggerVi = gs_board_class()->visualIndication()->new();


	if (pstTriggerVi == NULL) {
		return EMBER_APPLICATION_ERROR_0;
	}

	uint8_t status = gs_board_class()->triggertButton()->triggerCb(
			gs_cmp_device_monitor_trigger_callback);

	return status;
}
