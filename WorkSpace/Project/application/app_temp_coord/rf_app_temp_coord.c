/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_temp_coord.c
 * @author	rafael.fischer
 * @date	14 de ago. de 2024
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
#include "sl_zigbee_debug_print.h"
#include "gs_app_temp_coord.h"
#include "../../util/gs_board.h"
#include "../../zigbee_platform/zp_zed_network/gs_zp_zed_network.h"
#include "../../component/cmp_coord_temp/gs_cmp_coord_temp.h"
#include "../../zigbee_platform/zp_cluster_interface/gs_temp_measure_cluster_server.h"

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
static uint8_t gs_app_temp_zigbee_init(st_thermocoord_zigbee_info_t *zigbeeInfo) {


		uint16_t status = EMBER_SUCCESS;

		//status |= gs_zp_zed_network_class()->init(isEnableCli);

		uint16_t buffer[5] = { 0, };

		MEMCOPY(buffer, &zigbeeInfo->reTemp[5], zigbeeInfo->reTemp[5]+1);

		status |= gs_zb_temp_measure_cluster_class()->getMeasuredValue(&buffer[5]);




		return status;



}


uint8_t app_init_coord(st_thermocoord_zigbee_info_t *zigbeeInfo) {

	uint8_t status; //= gs_app_coord_zigbee_init(isEnableCli);

//	if(status){
//		return status;
//	}

	//status |= gs_app_smoke_zigbee_init(zigbeeInfo, isEnableCli);

	//status |= gs_app_temp_zigbee_init(zigbeeInfo);

	status |= gs_cmp_coord_temp_init();

	return status;


}


