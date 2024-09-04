/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_zed_network.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_ZP_ZED_NETWORK_H_
#define GS_ZP_ZED_NETWORK_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

typedef struct {

	uint8_t (*init)(bool isEnableCli);

	uint8_t (*startScanNetwork)(void);

	uint8_t (*stopScanNetwork)(void);

	uint8_t (*leaveNetwork)(void);

}st_zp_zed_network_class_t;

/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_zp_zed_network_class_t *gs_zp_zed_network_class(void);


#endif /* GS_ZP_ZED_NETWORK_H_ */
