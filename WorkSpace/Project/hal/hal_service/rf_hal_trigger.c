/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_trigger.c
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
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
#include "error-def.h"

/* ---------------------- Others include -------------------------------------*/
#include "../hal_common/gs_hal_gpio.h"
#include "gs_hal_trigger.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static uint8_t triggerDevicePort = 0;
static uint8_t triggerDevicePin = 0;
static uint8_t triggerDeviceIniState = 0;

static gs_hal_trigger_callback triggerCallback = NULL;

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_hal_trigger_device_init(uint8_t triggerPort, uint8_t triggerPin,
											uint8_t triggerInitState);
static uint8_t gs_hal_trigger_register_callback(
		gs_hal_trigger_callback callback);

/* --------------------- Public objects --------------------------------------*/
static const st_hal_trigger_class_t halTriggerClass = {
	.init = &gs_hal_trigger_device_init,
	.triggerCb = &gs_hal_trigger_register_callback
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_hal_trigger_class_t* gs_hal_trigger_class(void)
{
	return &halTriggerClass;
}

static void gs_hal_trigger_gpio_interrupt(uint8_t port, uint8_t pin)
{
	if (port == triggerDevicePort && pin == triggerDevicePin) {
		uint8_t status = gs_hal_gpio_class()->getInPinState(port, pin);

		if (triggerCallback != NULL) {
			triggerCallback((triggerDeviceIniState != status ? 0 : 1));
		}
	}
}

static uint8_t gs_hal_trigger_register_callback(
		gs_hal_trigger_callback callback)
{
	if (triggerCallback == NULL) {
		triggerCallback = callback;
		gs_hal_trigger_gpio_interrupt(triggerDevicePort, triggerDevicePin);
		return EMBER_SUCCESS;
	}
	return EMBER_APPLICATION_ERROR_0;
}

static uint8_t gs_hal_trigger_device_init(uint8_t triggerPort, uint8_t triggerPin,
											uint8_t triggerIniState)
{
	triggerDevicePort = triggerPort;
	triggerDevicePin = triggerPin;
	triggerDeviceIniState = triggerIniState;

	uint8_t status = gs_hal_gpio_class()->setInterrup(
			triggerPort, triggerPin, 100, gs_hal_trigger_gpio_interrupt);

	return status;
}
