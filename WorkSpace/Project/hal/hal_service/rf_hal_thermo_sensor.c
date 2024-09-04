/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_thermo_sensor.c
 * @author	rafael.fischer
 * @date	July 25, 2024
 *
 *  @brief 	This file contains functions for handling the temperature sensor.
 *         	It includes initialization and reading functions for the sensor.
 ******************************************************************************
 */

/******************************************************************************
 *							INCLUDES
 *****************************************************************************/
/* ---------------------- C language standard library ------------------------*/
/* No standard C library headers included */

/* ---------------------- Stack include --------------------------------------*/
#include "af.h"  // Include application framework header
#include "error-def.h"  // Include error definitions header
#include "em_gpio.h"  // Include GPIO (General Purpose Input/Output) definitions header
#include "hal.h"  // Include hardware abstraction layer header
#include "zigbee_app_framework_event.h"  // Include Zigbee application framework event header

/* ---------------------- Others include -------------------------------------*/
#include "../../zigbee_platform/zp_cluster_interface/gs_zp_ias_zone_cluster_server.h"  // Include cluster interface for IAS zone server
#include "../../zigbee_platform/zp_zed_network/gs_zp_zed_network.h"  // Include Zigbee end device network header
#include "../hal_common/gs_hal_gpio.h"  // Include GPIO hardware abstraction layer header
#include "../hal_driver/gs_hal_iadc.h"  // Include IADC (Integrated Analog-to-Digital Converter) driver header
#include "gs_hal_thermo_sensor.h"  // Include this module's header
#include "gs_hal_thermo_ntc.h"  // Include NTC (Negative Temperature Coefficient) thermistor header

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* No private macros defined */

/* --------------------- Private typedef -------------------------------------*/
/* No private typedefs defined */

/* --------------------- Private variables -----------------------------------*/
/* Variables to store the configuration of the temperature sensor */
static uint8_t thermoSensorPort;  // Port number for the temperature sensor
static uint8_t thermoSensorPin;  // Pin number for the temperature sensor
static boolean thermoSensorInitState = false;  // Flag to check if the sensor has been initialized
static uint8_t analogPort;  // Port number for the analog input
static uint8_t analogPin;  // Pin number for the analog input

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_hal_thermo_sensor_init(uint8_t thermoPort, uint8_t thermoPin,
											uint8_t inAnalogPort,
											uint8_t inAnalogPin);  // Function prototype for initializing the temperature sensor

static uint16_t getThermoSensor(void);  // Function prototype for reading the temperature sensor

/* --------------------- Public objects --------------------------------------*/
/* Structure containing function pointers for temperature sensor operations */
static const st_hal_thermo_sensor_class_t thermoSensorClass = {
	.init = &gs_hal_thermo_sensor_init,  // Pointer to the initialization function
	.getThermoSensor = &getThermoSensor  // Pointer to the function to read the sensor
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

/* Return a pointer to the temperature sensor class structure */
const st_hal_thermo_sensor_class_t* gs_hal_thermo_sensor_class(void)
{
	return &thermoSensorClass;  // Return the pointer to the static class structure
}

/* Initialize the temperature sensor with specified ports and pins */
static uint8_t gs_hal_thermo_sensor_init(uint8_t thermoPort, uint8_t thermoPin,
											uint8_t inAnalogPort,
											uint8_t inAnalogPin)
{
	if(!thermoSensorInitState){  // Check if the sensor is not already initialized

		thermoSensorPort = thermoPort;  // Set the temperature sensor port
		thermoSensorPin = thermoPin;  // Set the temperature sensor pin
		analogPort = inAnalogPort;  // Set the analog port
		analogPin = inAnalogPin;  // Set the analog pin

		// Configure the analog input with a reference voltage
		uint8_t status = gs_hal_gpio_class()->setAnalog(analogPort, analogPin,
														iadcVddRef, true);

		if(status){  // Check if there was an error configuring the analog input
			return status;  // Return the error status
		}

		// Configure the temperature sensor pin as output with a specific period
		status = gs_hal_gpio_class()->setOutPut(thermoSensorPort,
												thermoSensorPin, 0,
												GS_MAX_PERIOD, 0, false, false);

		if(status){  // Check if there was an error configuring the output
			return status;  // Return the error status
		}

		thermoSensorInitState = true;  // Mark the sensor as initialized
		return EMBER_SUCCESS;  // Return success
	}

	return EMBER_APPLICATION_ERROR_0;  // Return error if the sensor is already initialized
}

/* Read the temperature from the sensor */
static uint16_t getThermoSensor(void)
{
	uint8_t status = EMBER_SUCCESS;  // Initialize status as success

	uint16_t temP = 0;  // Variable to store the temperature value

	if(thermoSensorInitState){  // Check if the sensor is initialized

		// Set the temperature sensor pin high for a short period
		status = gs_hal_gpio_class()->setOutPut(thermoSensorPort,
												thermoSensorPin, 100,
												GS_MAX_PERIOD, 0, false, false);

		halCommonDelayMicroseconds(200);  // Delay to allow sensor to stabilize

		// Read the analog value from the sensor
		uint32_t resMiV = gs_hal_gpio_class()->analogRead(analogPort, analogPin);

		if(resMiV == IADC_ERROR){  // Check if there was an error reading the analog value
			status |= EMBER_APPLICATION_ERROR_0;  // Set error status
		}

		// Read the reference voltage
		uint32_t Vin = gs_hal_gpio_class()->getVin();

		if(Vin == IADC_ERROR){  // Check if there was an error reading the reference voltage
			status |= EMBER_APPLICATION_ERROR_1;  // Set error status
		}

		// Calculate the temperature in millivolts
		double ResultThermo = (resMiV * (double)10000.0)/(Vin-resMiV);

		// Convert millivolts to temperature in centi-degrees Celsius
		status |= gs_temp_ntc_class_t()->getTempCelsNtc(&ResultThermo, &temP);

		// Set the temperature sensor pin low
		status = gs_hal_gpio_class()->setOutPut(thermoSensorPort, thermoSensorPin, 0, GS_MAX_PERIOD, 0, false, false);

		if (status) {  // Check if there was an error setting the pin low
			return temP;  // Return the temperature value
		}

		return temP;  // Return the temperature value

	}
	return temP;  // Return the temperature value if sensor is not initialized
}
