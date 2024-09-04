/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_visual_indication_cli.c
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
#include "sl_zigbee_debug_print.h"

/* ---------------------- Others include -------------------------------------*/
#include "../../cli/gs_cli_interface.h"
#include "gs_hal_visual_indication.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static uint8_t index = 0;
static st_vi_instance_t *instances[5] = { NULL, };

/* --------------------- Private function prototypes -------------------------*/
/* --------------------- Public objects --------------------------------------*/

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
static void gs_cli_create_instance_cmd_function(sl_cli_command_arg_t *arguments)
{
	if (index < 5) {
		instances[index] = gs_hal_visual_indication_class()->new();
		index++;
		sl_zigbee_app_debug_println(" Instance Created ");
	} else {
		sl_zigbee_app_debug_println(" Fail Created Instance ");
	}
}

static void gs_cli_config_instance_cmd_function(sl_cli_command_arg_t *arguments)
{
	uint8_t indexInt = sl_cli_get_argument_uint8(arguments, 0);
	uint8_t state = sl_cli_get_argument_uint8(arguments, 1);
	if(instances[indexInt] != NULL){
		instances[indexInt]->setViState(instances[indexInt], state);
		sl_zigbee_app_debug_println(" Success ");
	} else {
		sl_zigbee_app_debug_println(" Error ");
	}
}

static void gs_cli_print_instance_cmd_function(sl_cli_command_arg_t *arguments)
{
	if (instances[0] == NULL) {
		sl_zigbee_app_debug_println(" Instances is Empty ");
	} else {
		for (int i = 0; i <= index; i++) {
			sl_zigbee_app_debug_print(" %d - %d ", i, instances[i]);
			sl_zigbee_app_debug_print("\n");
		}
	}
}

const sl_cli_command_info_t gs_cli_create_instance_cmd =
	SL_CLI_COMMAND(gs_cli_create_instance_cmd_function,
			"Start the steering process",
			"" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_END, });

const sl_cli_command_info_t gs_cli_config_instance_cmd =
	SL_CLI_COMMAND(gs_cli_config_instance_cmd_function,
			"Stop the steering process",
			"Index of Instance" SL_CLI_UNIT_SEPARATOR
			"Index of Instance" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_UINT8, SL_CLI_ARG_UINT8, SL_CLI_ARG_END, });

const sl_cli_command_info_t gs_cli_print_instance_cmd =
	SL_CLI_COMMAND(gs_cli_print_instance_cmd_function,
			"Print the Instances",
			"" SL_CLI_UNIT_SEPARATOR,
			{ SL_CLI_ARG_END, });

static const sl_cli_command_entry_t gs_hal_vi_sub_table[] = {
  { "new", &gs_cli_create_instance_cmd, false},
  { "config", &gs_cli_config_instance_cmd, false},
  { "print", &gs_cli_print_instance_cmd, false},
  { NULL, NULL, false },
};

const sl_cli_command_info_t gs_cli_hal_vi_sub_menu_group =
		SL_CLI_COMMAND_GROUP(gs_hal_vi_sub_table, "the sub menu table");

const sl_cli_command_entry_t gs_cli_hal_vi_main_table[] = {
  { "halVi", &gs_cli_hal_vi_sub_menu_group, false },
  { NULL, NULL, false },
};

sl_cli_command_group_t gs_cli_hal_vi_main_group =
{
  { NULL },
  false,
  gs_cli_hal_vi_main_table
};
