/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_gpio.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_HAL_GPIO_H_
#define GS_HAL_GPIO_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
#define GS_MAX_PERIOD	(HALF_MAX_INT32U_VALUE - 1)

/* -------------------------- Typedef ----------------------------------------*/
typedef void (*interruptCallback_t)(int8u port, int8u pin);

typedef struct {

	int8u (*setOutPut)(int8u port, int8u pin, int8u percent, int32u period,
						int32u interactions, boolean invert,
						boolean forceUpdate);
	int8u (*setOut)(int8u port, int8u pin, int8u status);

	int8u (*setAnalog)(int8u port, int8u pin, int8u ref, boolean inMilliVolts);

	int32u (*analogRead)(int8u port, int8u pin);

	int32u (*getVin)(void);

	int8u (*setInterrup)(int8u port, int8u pin, int8u pullUp, interruptCallback_t callback);

	int8u (*getInPinState)(int8u port, int8u pin);

	int8u (*setDisableInterrup)(int8u port, int8u pin);

	int8u (*setEnableInterrup)(int8u port, int8u pin);



}st_gpio_class_t;




/* -------------------------- Public objects ---------------------------------*/
const st_gpio_class_t *gs_hal_gpio_class(void);


#endif /* GS_HAL_GPIO_H_ */
