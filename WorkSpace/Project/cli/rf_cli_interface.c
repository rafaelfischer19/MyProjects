/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_cli_interface.c
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
#include PLATFORM_HEADER

/* ---------------------- Others include -------------------------------------*/
#include "gs_cli_interface.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
extern const sl_cli_handle_t sl_cli_handles[];
extern const uint32_t sl_cli_handles_count;

/* --------------------- Private function prototypes -------------------------*/
static void gs_cli_interface_add(sl_cli_command_group_t *stCommandGroup);
static void gs_cli_interface_remove(sl_cli_command_group_t *stCommandGroup);

/* --------------------- Public objects --------------------------------------*/
/*
 * 'Object' created to encapsulate the class
 */
const st_cli_interface_class_t cliInterfaceClass = {
	.addGroup = &gs_cli_interface_add,
	.removeGroup = &gs_cli_interface_remove,
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_cli_interface_class_t *gs_cli_interface_class(void)
{
	return &cliInterfaceClass;
}

/**
 * @brief		Add a new CLI group.
 * @param		[in] stCommandGroup New Group Pointer.
 */
static void gs_cli_interface_add(sl_cli_command_group_t *stCommandGroup)
{
	for (uint8_t i = 0; i < sl_cli_handles_count; i++) {
		sl_cli_command_add_command_group(sl_cli_handles[i], stCommandGroup);
	}
}

/**
 * @brief		Remove the CLI group.
 * @param		[in] stCommandGroup The CLI Group to remove.
 */
static void gs_cli_interface_remove(sl_cli_command_group_t *stCommandGroup)
{
	for (uint8_t i = 0; i < sl_cli_handles_count; i++) {
		sl_cli_command_remove_command_group(sl_cli_handles[i], stCommandGroup);
	}
}
