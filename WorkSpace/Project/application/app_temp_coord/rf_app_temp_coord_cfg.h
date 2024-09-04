/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_app_temp_coord_cfg.h
 * @author	rafael.fischer
 * @date	14 de ago. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_APP_TEMP_COORD_CFG_H_
#define GS_APP_TEMP_COORD_CFG_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
#define APP_NAME "Zigbee Temperature Receiver"
#define APP_VERSION "1.0"
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

const st_thermocoord_zigbee_info_t stThermoInfo1 = {
    .reTemp = {0x09, 0xC4, 0, 0, 0, 0, 0, 0, 0, 0}  // 0x09C4 (decimal 2500) representa 25.00°C
};

#endif /* GS_APP_TEMP_COORD_CFG_H_ */
