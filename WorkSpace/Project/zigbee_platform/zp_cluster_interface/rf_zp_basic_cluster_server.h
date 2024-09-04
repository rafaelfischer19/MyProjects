/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_basic_cluster_server.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_ZP_BASIC_CLUSTER_SERVER_H_
#define GS_ZP_BASIC_CLUSTER_SERVER_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
typedef struct {
	uint8_t (*setMfgName)(uint8_t *name);
	uint8_t (*getMfgName)(uint8_t *name);

	uint8_t (*setModel)(uint8_t *model);
	uint8_t (*getModel)(uint8_t *model);

}st_zp_basic_cluster_server_class_t;

/* -------------------------- Public objects ---------------------------------*/

const st_zp_basic_cluster_server_class_t *gs_zp_basic_cluster_server_class(void);

#endif /* GS_ZP_BASIC_CLUSTER_SERVER_H_ */
