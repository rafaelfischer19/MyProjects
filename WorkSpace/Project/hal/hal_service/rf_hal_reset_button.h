/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_reset_button.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_RESET_BUTTON_H_
#define GS_HAL_RESET_BUTTON_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/
typedef void (*gs_hal_button_pressed)(uint32_t timePress);

typedef struct {

	uint8_t (*init)(uint8_t port, uint8_t pin, uint8_t pressState);

	uint8_t (*registerCb)(gs_hal_button_pressed callback);

} st_hal_reset_button_class_t;

/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_hal_reset_button_class_t *gs_hal_reset_button_class(void);


#endif /* GS_HAL_RESET_BUTTON_H_ */
