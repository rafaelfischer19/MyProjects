/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_thermo_ntc.c
 * @author	rafael.fischer
 * @date	August 1, 2024
 *
 *  @brief 	This file contains functions for handling NTC thermistor temperature
 *         	sensing. It includes conversion between resistance and temperature.
 ******************************************************************************
 */

/******************************************************************************
 *							INCLUDES
 *****************************************************************************/
/* ---------------------- C language standard library ------------------------*/
/* No C standard library headers included */

/* ---------------------- Stack include --------------------------------------*/
#include PLATFORM_HEADER  // Include platform-specific header

/* ---------------------- Others include -------------------------------------*/
#include "gs_hal_thermo_ntc.h"  // Include header for this module
#include "gs_hal_thermo_sensor.h"  // Include header for sensor interface

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
#if defined(GS_NTC)  // Check if GS_NTC macro is defined
#define GS_TEMP {\
	{-40,195652},{-35,148171},{-30,113347},{-25,87559},{-20,68237},\
    {-15,53650},{-10,42506},{-5,33892},{0,27219},{5,22021},{10,17926},\
	{15,14674},{20,12081},{25,10000},{30,8315},{35,6948},{40,5834},\
	{45,4917},{50,4161},{55,3535},{60,3014},{65,2586},{70,2228},\
	{75,1925},{80,1669},{85,1452},{90,1268},{95,1110},{100,974},\
	{105,858},{110,758},{115,672},{120,596},{125,531}\
}  // Define temperature to resistance mappings in a table

#endif

#define GS_NTC_TABLE (int8u)(sizeof(stNtcTempTable)/ sizeof(st_cels_ntc_temp_t))-1  // Calculate the number of entries in the temperature table

/* --------------------- Private typedef -------------------------------------*/
/* Structure for storing temperature and corresponding NTC resistance */
typedef struct {
	double TempCels;  // Temperature in Celsius
	double rNtc;      // NTC resistance
} st_cels_ntc_temp_t;

/* --------------------- Private variables -----------------------------------*/
/* Table containing temperature to resistance mappings */
static const st_cels_ntc_temp_t stNtcTempTable[] = GS_TEMP;  // Initialize temperature table with predefined values

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_temp_res(double *prNtc, uint16_t *pTempResult);  // Function prototype to convert NTC resistance to temperature

static uint8_t gs_res_temp(uint16_t *pTempCels, double *pNtcResult);  // Function prototype to convert temperature to NTC resistance

static int8u get_Range(double *maxTemp, double *minTemp);  // Function prototype to get the range of temperatures from the table

/* --------------------- Public objects --------------------------------------*/
/* Structure containing pointers to the functions for temperature conversion */
static const st_temp_ntc_class_t thermoNtcClass = {
		.getTempCelsNtc = &gs_temp_res,  // Pointer to function to get temperature from NTC resistance
		.getNctTempCels = &gs_res_temp,  // Pointer to function to get NTC resistance from temperature
		.getRange = &get_Range  // Pointer to function to get temperature range
};


/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

/* Returns a pointer to the temperature NTC class structure */
const st_temp_ntc_class_t *gs_temp_ntc_class_t(void)
{
	return &thermoNtcClass;  // Return pointer to the static class structure
}

/* Determine if a given temperature or resistance is within the limits of the table */
static boolean gs_lim_ntc_temp(double *piTempC, double *pdRntc,
								st_cels_ntc_temp_t *stUpLim,
								st_cels_ntc_temp_t *stDowLim)
{
	uint8_t NtcTable = GS_NTC_TABLE;  // Get the number of entries in the temperature table
	for (int8u i = 0; i < NtcTable; i++) {  // Iterate over the table entries
		*stDowLim = stNtcTempTable[i];  // Set the lower limit
		*stUpLim = stNtcTempTable[i + 1];  // Set the upper limit

		if (piTempC != NULL) {  // Check if temperature pointer is not NULL
			// Check if the temperature is within the bounds
			if (*piTempC >= stDowLim->TempCels
					&& *piTempC <= stUpLim->TempCels) {
				return true;  // Temperature is within range
			}
		} else if (pdRntc != NULL) {  // Check if resistance pointer is not NULL
			// Check if the resistance is within the bounds
			if (*pdRntc <= stDowLim->rNtc
					&& *pdRntc >= stUpLim->rNtc) {
				return true;  // Resistance is within range
			}
		}
	}

	return false;  // No match found
}

/* Convert NTC resistance to temperature in centi-degrees Celsius */
static uint8_t gs_temp_res(double *prNtc, uint16_t *pTempResult)
{
	st_cels_ntc_temp_t stUpLim = {0};  // Initialize upper limit structure
	st_cels_ntc_temp_t stDowLim = {0};  // Initialize lower limit structure
	double CalcRes = 0;  // Variable to store calculated result
	boolean res = gs_lim_ntc_temp(NULL, prNtc, &stUpLim, &stDowLim);  // Check if resistance is within limits

	if (res) {  // If within limits
		// Calculate the temperature based on the resistance
		CalcRes = (stDowLim.rNtc - *prNtc) / (stDowLim.rNtc - stUpLim.rNtc);
		CalcRes *= (stUpLim.TempCels - stDowLim.TempCels);
		CalcRes += stDowLim.TempCels;
		*pTempResult = (uint16_t)(CalcRes * 100);  // Store result in centi-degrees Celsius
		return 0x00;  // Success
	}
	return 0xFF;  // Error
}

/* Convert temperature in centi-degrees Celsius to NTC resistance */
static uint8_t gs_res_temp(uint16_t *pTempCels, double *pNtcResult)
{
	st_cels_ntc_temp_t stUpLim = {0};  // Initialize upper limit structure
	st_cels_ntc_temp_t stDowLim = {0};  // Initialize lower limit structure
	double dCalcRes = 0;  // Variable to store calculated resistance
	double iCalcTemp = (double)*pTempCels / 100;  // Convert temperature from centi-degrees to degrees Celsius
	boolean res = gs_lim_ntc_temp(&iCalcTemp, NULL, &stUpLim, &stDowLim);  // Check if temperature is within limits

	if (res) {  // If within limits
		// Calculate the resistance based on the temperature
		dCalcRes = (iCalcTemp - stDowLim.TempCels)
				/ (stUpLim.TempCels - stDowLim.TempCels);
		dCalcRes *= (stUpLim.rNtc - stDowLim.rNtc);
		dCalcRes += stDowLim.rNtc;
		*pNtcResult = dCalcRes;  // Store result
		return 0x00;  // Success
	}
	return 0xFF;  // Error
}

/* Get the maximum and minimum temperatures from the table */
static int8u get_Range(double *maxTemp, double *minTemp)
{
	*maxTemp = stNtcTempTable[GS_NTC_TABLE].TempCels;  // Set maximum temperature
	*minTemp = stNtcTempTable[0].TempCels;  // Set minimum temperature
	return 0x00;  // Success
}
