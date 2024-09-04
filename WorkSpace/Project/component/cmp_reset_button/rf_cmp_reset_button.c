/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_cmp_reset_button.c
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
#include PLATFORM_HEADER

/* ---------------------- Others include -------------------------------------*/
#include "../../zigbee_platform/zp_zed_network/gs_zp_zed_network.h"
#include "../../util/gs_board.h"
#include "gs_cmp_reset_button.h"

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

static void gs_cmp_reset_button_callback(uint32_t timePress) {
	if (timePress > 9500 && timePress < 10500) {
		gs_zp_zed_network_class()->leaveNetwork();
	}

}

uint8_t gs_cmp_reset_button_init(void)
{
	return gs_board_class()->resetButton()->registerCb(
			gs_cmp_reset_button_callback);
}


