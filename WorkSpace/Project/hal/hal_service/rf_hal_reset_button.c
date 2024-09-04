/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_reset_button.c
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
#include "error.h"
#include "sl_malloc.h"
#include "hal.h"

/* ---------------------- Others include -------------------------------------*/
#include "../hal_common/gs_hal_gpio.h"
#include "gs_hal_visual_indication.h"
#include "gs_hal_reset_button.h"
#include "sl_zigbee_debug_print.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
typedef struct st_button_cb {
	gs_hal_button_pressed callback;
	struct st_button_cb *next;
} st_button_cb_t;

/* --------------------- Private variables -----------------------------------*/
static uint8_t btnPort = 0;
static uint8_t btnPin = 0;
static uint8_t btnPressState = 0;

static st_vi_instance_t *pstViButtonPress = NULL;
static st_button_cb_t *pstButtonCbList = NULL;

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_hal_reset_button_init(uint8_t port, uint8_t pin, uint8_t pressState);
static uint8_t gs_hal_reset_button_register_callback(gs_hal_button_pressed callback);

/* --------------------- Public objects --------------------------------------*/
const st_hal_reset_button_class_t halResetButtonClass = {
	.init = &gs_hal_reset_button_init,
	.registerCb = &gs_hal_reset_button_register_callback
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_hal_reset_button_class_t* gs_hal_reset_button_class(void)
{
	return &halResetButtonClass;
}

static uint8_t gs_hal_reset_button_register_callback(gs_hal_button_pressed callback)
{
	st_button_cb_t *new = (st_button_cb_t*)sl_malloc(sizeof(st_button_cb_t));

	if (new == NULL) {
		return EMBER_APPLICATION_ERROR_0;
	}

	new->callback = callback;
	new->next = NULL;

	if (pstButtonCbList == NULL) {
		pstButtonCbList = new;
	} else {
		st_button_cb_t *current = pstButtonCbList;

		while(current->next != NULL) {
			current = current->next;
		}
		current->next = new;
	}
	return EMBER_SUCCESS;
}

static void gs_hal_gpio_button_callback(uint8_t port, uint8_t pin)
{
	static uint32_t timePressed = 0;

	if (port != btnPort || pin != btnPin || pstViButtonPress == NULL) {
		return;
	}

	uint8_t btnState = gs_hal_gpio_class()->getInPinState(port, pin);

	if (btnState == btnPressState) {
		pstViButtonPress->setViState(pstViButtonPress, enButtonAction);

		timePressed = halCommonGetInt32uMillisecondTick();

		sl_zigbee_app_debug_println(" Button is Pressed ");
	} else {
		pstViButtonPress->setViState(pstViButtonPress, enOff);

		uint32_t timeNow = halCommonGetInt32uMillisecondTick();

		timePressed = elapsedTimeInt32u(timePressed, timeNow);

		st_button_cb_t *current = pstButtonCbList;

		while(current != NULL) {
			current->callback(timePressed);

			current = current->next;
		}
		sl_zigbee_app_debug_println(" Button is Released ");
	}
}

static uint8_t gs_hal_reset_button_init(uint8_t port, uint8_t pin,
										uint8_t pressState)
{
	btnPin = pin;
	btnPort = port;
	btnPressState = pressState;

	uint8_t status = gs_hal_gpio_class()->setInterrup(port, pin, 1, gs_hal_gpio_button_callback);

	pstViButtonPress = gs_hal_visual_indication_class()->new();

	if (pstViButtonPress == NULL) {
		return EMBER_APPLICATION_ERROR_0;
	}
	return status;
}
