/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_visual_indication.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_VISUAL_INDICATION_H_
#define GS_HAL_VISUAL_INDICATION_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
typedef enum {
	enButtonAction	 		= 0, /**< Button Action */
	enScanNetwork 			= 1, /**< Scan Network */
	enFire 					= 2, /**< Fire Alarm */
	enFault 				= 3, /**< Fault Alarm */
	enOperation 			= 4, /**< Operation Mode */
	enOff		 			= 5, /**< Visual Indication OFF */
}en_vi_state_t;

typedef struct {
		uint8_t (*setViState) (const void *pContext, en_vi_state_t state);
}st_vi_instance_t;

/* -------------------------- Public objects ---------------------------------*/

typedef struct {
		uint8_t (*init)(uint8_t viPort, uint8_t viPin, uint8_t viLedOffState,
						bool isEnableCli);
		st_vi_instance_t* (*new)(void);
}st_hal_visual_indication_class_t;

const st_hal_visual_indication_class_t* gs_hal_visual_indication_class(void);

#endif /* GS_HAL_VISUAL_INDICATION_H_ */
