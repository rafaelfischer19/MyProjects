/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_zc_network_cli.c
 * @author	rafael.fischer
 * @date	18 de jul. de 2024
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
#include "mac-types.h"
#include "sl_zigbee_command_interpreter.h"
#include "sl_zigbee_debug_print.h"

/* ---------------------- Others include -------------------------------------*/
#include "../../cli/gs_cli_interface.h"
#include "gs_zp_zc_network.h"

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

static void gs_cli_open_network_cmd_function(sl_cli_command_arg_t *arguments)
{
	EmberEUI64 eui64;

	sl_zigbee_copy_eui64_arg(arguments, 0, eui64, true);

	uint8_t status = gs_zp_zc_network_class()->openNetwork(eui64);

	sl_zigbee_app_debug_println("Open network: 0x%1X", status);
}

//Create a simple command
const sl_cli_command_info_t gs_cli_open_network_cmd =
	SL_CLI_COMMAND(gs_cli_open_network_cmd_function,
			"Open the network for a eui64",
			"Device eui64" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_HEX, SL_CLI_ARG_END, });

// Create a new sub menu command table
static const sl_cli_command_entry_t gs_zc_network_sub_table[] = {
  { "openNetwork", &gs_cli_open_network_cmd, false},
  { NULL, NULL, false },
};


// Create a new sub menu Group
const sl_cli_command_info_t gs_cli_zc_network_sub_menu_group =
		SL_CLI_COMMAND_GROUP(gs_zc_network_sub_table, "the sub menu table");


// Create main command table
const sl_cli_command_entry_t gs_cli_zc_network_main_table[] = {
  { "zc-network", &gs_cli_zc_network_sub_menu_group, false },
  { NULL, NULL, false },
};


//The CLI Main Group
sl_cli_command_group_t gs_cli_zc_network_main_group =
{
  { NULL },
  false,
  gs_cli_zc_network_main_table
};

