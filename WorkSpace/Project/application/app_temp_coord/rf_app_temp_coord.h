/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_temp_coord.h
 * @author	rafael.fischer
 * @date	14 de ago. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_TEMP_COORD_H_
#define GS_APP_TEMP_COORD_H_

/* -------------------------- Includes ---------------------------------------*/
#include "gs_app_temp_coord.h"
#include "sl_zigbee_debug_print.h"
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

typedef struct {
	uint16_t reTemp[5];

} st_thermocoord_zigbee_info_t;



// Função para inicializar a aplicação
uint8_t app_init_coord(st_thermocoord_zigbee_info_t *zigbeeInfo);

// Função para processar a aplicação continuamente
//static app_process(void);

#endif /* GS_APP_TEMP_COORD_H_ */
