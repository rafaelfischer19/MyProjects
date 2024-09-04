/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_device_monitor.c
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
#include "gs_hal_device_monitor.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static uint8_t detachDevicePort = 0;
static uint8_t detachDevicePin = 0;
static uint8_t detachDeviceInitState = 0;

static gs_detach_callback detachCallback = NULL;

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_hal_device_monitor_init(uint8_t detachPort, uint8_t detachPin,
											uint8_t detachInitState);
static uint8_t gs_hal_detach_register_callback(
		gs_detach_callback callback);
static uint8_t getBatteryMilli(uint16_t *battery);

/* --------------------- Public objects --------------------------------------*/
static const st_hal_device_monitor_class_t deviceMonitorClass = {
	.init = &gs_hal_device_monitor_init,
	.detachRegister = &gs_hal_detach_register_callback,
	.getBattery = &getBatteryMilli
};


/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_hal_device_monitor_class_t* gs_hal_device_monitor_class(void)
{
	return &deviceMonitorClass;
}

static uint8_t getBatteryMilli(uint16_t *battery)
{
	uint32_t batteryRead = gs_hal_gpio_class()->getVin();

	if (batteryRead == 0xFFFFFFFF || battery == NULL) {
		return EMBER_APPLICATION_ERROR_0;
	}
	*battery = (uint16_t) batteryRead;

	return EMBER_SUCCESS;
}

static void gs_hal_detach_gpio_interrupt(uint8_t port, uint8_t pin)
{
	if (port == detachDevicePort && pin == detachDevicePin) {
		uint8_t status = gs_hal_gpio_class()->getInPinState(port, pin);

		if (detachCallback != NULL) {
			detachCallback((detachDeviceInitState != status ? 1 : 0));
		}
	}
}

static uint8_t gs_hal_detach_register_callback(
		gs_detach_callback callback)
{
	if (detachCallback == NULL) {
		detachCallback = callback;
		gs_hal_detach_gpio_interrupt(detachDevicePort, detachDevicePin);
		return EMBER_SUCCESS;
	}
	return EMBER_APPLICATION_ERROR_0;
}

static uint8_t gs_hal_device_monitor_init(uint8_t detachPort, uint8_t detachPin,
											uint8_t detachInitState)
{
	detachDevicePort = detachPort;
	detachDevicePin = detachPin;
	detachDeviceInitState = detachInitState;

	uint8_t status = gs_hal_gpio_class()->setInterrup(
			detachPort, detachPin, 0, gs_hal_detach_gpio_interrupt);

	return status;
}











