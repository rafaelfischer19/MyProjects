/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_cli_usage.c
 * @author	rafael.fischer
 * @date	17 de jul. de 2024
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
#include "sl_zigbee_debug_print.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_cli_interface.h"

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

// -------------- SUB MENU BLOCK

//Create the sub menu print command function
static void gs_cli_sub_menu_print_function(sl_cli_command_arg_t *arguments)
{
	uint8_t valueToPrint = sl_cli_get_argument_uint8(arguments, 0);
	sl_zigbee_app_debug_println("You are here!! - the sub menu print work: %d",
								valueToPrint);
}

//Create the sub menu print command
const sl_cli_command_info_t gs_cli_sub_menu_print_cmd =
SL_CLI_COMMAND(gs_cli_sub_menu_print_function,
		"Sub menu Print Command",
		"value to print" SL_CLI_UNIT_SEPARATOR,
		{	SL_CLI_ARG_UINT8, SL_CLI_ARG_END,})
;

// Create a new sub menu command table
static const sl_cli_command_entry_t gs_cli_sub_menu_table[] = {
  { "print", &gs_cli_sub_menu_print_cmd, false},
  { NULL, NULL, false },
};

// Create a new sub menu Group
const sl_cli_command_info_t gs_cli_sub_menu_group =
		SL_CLI_COMMAND_GROUP(gs_cli_sub_menu_table, "the sub menu table");

// ------------ END SUB MENU BLOCK


// ------------- START SIMPLE COMMAND BLOCK
static void gs_cli_simple_cmd_function(sl_cli_command_arg_t *arguments)
{
	uint8_t valueToPrint = sl_cli_get_argument_uint8(arguments, 0);
	sl_zigbee_app_debug_println(
			"You are here!! - the simple CMD print work: %d", valueToPrint);
}

//Create a simple command
const sl_cli_command_info_t gs_cli_simple_cmd =
	SL_CLI_COMMAND(gs_cli_simple_cmd_function,
			"Simple Command",
			"value to Print" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_UINT8, SL_CLI_ARG_END, });

// ------------ END SIMPLE COMMAND BLOCK

static void gs_cli_artur_function(sl_cli_command_arg_t *arguments)
{
	sl_zigbee_app_debug_println(" TESTE ");
}

//Create a simple command
const sl_cli_command_info_t gs_artur_cmd =
	SL_CLI_COMMAND(gs_cli_artur_function,
			"Simple Command",
			"" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_END, });

static void gs_cli_artur_value_function(sl_cli_command_arg_t *arguments)
{
	uint8_t valueToPrint = sl_cli_get_argument_uint8(arguments, 0);
	uint8_t valueToPrint1 = sl_cli_get_argument_uint8(arguments, 1);
	sl_zigbee_app_debug_println(" TESTE %d - %d", valueToPrint, valueToPrint1);
}

//Create a simple command
const sl_cli_command_info_t gs_artur_value_cmd =
	SL_CLI_COMMAND(gs_cli_artur_value_function,
			"Simple Command",
			"valor 1" SL_CLI_UNIT_SEPARATOR
			"valor 2" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_UINT8, SL_CLI_ARG_UINT8, SL_CLI_ARG_END, });

// Create main command table
const sl_cli_command_entry_t gs_cli_main_table[] = {
  { "gs-ex-submenu", &gs_cli_sub_menu_group, false },
  { "gs-ex-direct-cmd", &gs_cli_simple_cmd, false },
  { "artur", &gs_artur_cmd, false },
  { "artur-value", &gs_artur_value_cmd, false },
  { NULL, NULL, false },
};

//The CLI Main Group
sl_cli_command_group_t gs_cli_main_group =
{
  { NULL },
  false,
  gs_cli_main_table
};
