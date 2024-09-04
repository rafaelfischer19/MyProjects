/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_smoke_sample.c
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
#include "em_gpio.h"
#include "hal.h"

/* ---------------------- Others include -------------------------------------*/
#include "../hal_common/gs_hal_gpio.h"
#include "../hal_driver/gs_hal_iadc.h"
#include "gs_hal_smoke_sample.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
#define teste(stat) if(stat != 0) return status;

/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static uint8_t ciPort;
static uint8_t ciPin;
static uint8_t ledPort;
static uint8_t ledPin;
static uint8_t analogPort;
static uint8_t analogPin;

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_hal_smoke_sample_init(uint8_t enableCiPort, uint8_t enableCiPin,
		uint8_t irLedPort, uint8_t irLedPin, uint8_t inAnalogPort,
		uint8_t inAnalogPin);
static uint8_t gs_hal_get_sample(st_hal_smoke_data_t *pstSample);

/* --------------------- Public objects --------------------------------------*/
/*
 * 'Object' created to encapsulate the class
 */
static const st_hal_smoke_sample_class_t halSmokeSampleClass = {
	.init = &gs_hal_smoke_sample_init,
	.getSample = &gs_hal_get_sample
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_hal_smoke_sample_class_t *gs_hal_smoke_sample_class(void){
	return &halSmokeSampleClass;
}

static uint8_t gs_hal_get_sample(st_hal_smoke_data_t *pstSample)
{
	uint8_t status = EMBER_APPLICATION_ERROR_0;

	if (pstSample == NULL) {
		status = EMBER_APPLICATION_ERROR_0;
		return status;
	}

	gs_hal_gpio_class()->setOut(ciPort, ciPin, 1);

	halCommonDelayMicroseconds(200);

	pstSample->ledOff = gs_hal_gpio_class()->analogRead(analogPort, analogPin);

	gs_hal_gpio_class()->setOut(ledPort, ledPin, 1);

	halCommonDelayMicroseconds(200);

	pstSample->ledOn = gs_hal_gpio_class()->analogRead(analogPort, analogPin);

	gs_hal_gpio_class()->setOut(ledPort, ledPin, 0);
	gs_hal_gpio_class()->setOut(ciPort, ciPin, 0);

	return 0;
}

static uint8_t gs_hal_smoke_sample_init(uint8_t enableCiPort,
												uint8_t enableCiPin,
												uint8_t irLedPort,
												uint8_t irLedPin,
												uint8_t inAnalogPort,
												uint8_t inAnalogPin)
{
	ciPort = enableCiPort;
	ciPin = enableCiPin;
	ledPort = irLedPort;
	ledPin = irLedPin;
	analogPort = inAnalogPort;
	analogPin = inAnalogPin;

	gs_hal_gpio_class()->setOut(ciPort, ciPin, 0);

	gs_hal_gpio_class()->setOut(ledPort, ledPin, 0);

	gs_hal_gpio_class()->setAnalog(analogPort, analogPin, iadcInternalRefe1V2, false);
	return 0;
}




















