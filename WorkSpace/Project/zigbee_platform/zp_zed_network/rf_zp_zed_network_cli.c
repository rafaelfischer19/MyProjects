/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_zed_network_cli.c
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
#include "sl_zigbee_command_interpreter.h"
#include "sl_zigbee_debug_print.h"

/* ---------------------- Others include -------------------------------------*/
#include "../../cli/gs_cli_interface.h"
#include "gs_zp_zed_network.h"

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
static void gs_cli_start_scan_cmd_function(sl_cli_command_arg_t *arguments)
{
	uint8_t status = gs_zp_zed_network_class()->startScanNetwork();

	sl_zigbee_app_debug_print(" Zed Network Start: 0x%1X", status);
}

static void gs_cli_stop_scan_cmd_function(sl_cli_command_arg_t *arguments)
{
	uint8_t status = gs_zp_zed_network_class()->stopScanNetwork();

	sl_zigbee_app_debug_print(" Zed Network Stop: 0x%1X", status);
}

const sl_cli_command_info_t gs_cli_start_scan_cmd =
	SL_CLI_COMMAND(gs_cli_start_scan_cmd_function,
			"Start the steering process",
			"" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_END, });

const sl_cli_command_info_t gs_cli_stop_scan_cmd =
	SL_CLI_COMMAND(gs_cli_stop_scan_cmd_function,
			"Stop the steering process",
			"" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_END, });

static const sl_cli_command_entry_t gs_zed_network_sub_table[] = {
  { "startScan", &gs_cli_start_scan_cmd, false},
  { "stopScan", &gs_cli_stop_scan_cmd, false},
  { NULL, NULL, false },
};

const sl_cli_command_info_t gs_cli_zed_network_sub_menu_group =
		SL_CLI_COMMAND_GROUP(gs_zed_network_sub_table, "the sub menu table");

const sl_cli_command_entry_t gs_cli_zed_network_main_table[] = {
  { "zed-network", &gs_cli_zed_network_sub_menu_group, false },
  { NULL, NULL, false },
};

sl_cli_command_group_t gs_cli_zed_network_main_group =
{
  { NULL },
  false,
  gs_cli_zed_network_main_table
};

