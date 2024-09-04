/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_cli_interface.h
 * @author	rafael.fischer
 * @date	17 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_CLI_INTERFACE_H_
#define GS_CLI_INTERFACE_H_

/* -------------------------- Includes ---------------------------------------*/
#include "sl_cli.h"

/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/
typedef struct {
	/**
	* @brief		Add a new CLI group.
	* @param		[in] stCommandGroup New Group Pointer.
	*/
	void (*addGroup)(sl_cli_command_group_t *stCommandGroup);

	/**
	* @brief		Remove the CLI group.
	* @param		[in] stCommandGroup The CLI Group to remove.
	*/
	void (*removeGroup)(sl_cli_command_group_t *stCommandGroup);

}st_cli_interface_class_t;

/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_cli_interface_class_t *gs_cli_interface_class(void);


#endif /* GS_CLI_INTERFACE_H_ */
