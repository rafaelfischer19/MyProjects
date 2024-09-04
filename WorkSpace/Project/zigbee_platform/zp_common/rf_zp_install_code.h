/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_install_code.h
 * @author	rafael.fischer
 * @date	17 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_ZP_INSTALL_CODE_H_
#define GS_ZP_INSTALL_CODE_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
#define GS_INSTALL_CODE_SIZE 18

/* -------------------------- Typedef ----------------------------------------*/
typedef struct {

	void (*configure)(void);

	uint8_t (*getinstallCodeKeyFromEui64)(EmberEUI64 eui64,
											int8u *puiBuffToCopyInstallCode,
											int8u uiSizeBuffer);

} st_install_code_class_t;

/* -------------------------- Public objects ---------------------------------*/


/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_install_code_class_t *gs_zp_install_code_class(void);


#endif /* GS_ZP_INSTALL_CODE_H_ */
