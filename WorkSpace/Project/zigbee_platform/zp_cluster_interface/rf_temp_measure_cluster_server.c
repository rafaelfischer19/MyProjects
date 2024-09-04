/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_temp_measure_cluster_server.c
 * @author	rafael.fischer
 * @date	13 de ago. de 2024
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
#include "attribute-storage.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_temp_measure_cluster_server.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
#define GS_ZB_TEMP_MEASURE_UNKNOWN 		0x8000

/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
//Get Measured Value Attribute
static int32u getMeasuredValue(double *measuredValue);
//Set Measured Value Attribute
static int32u setMeasuredValue(double measuredValue);
//Get Minimum Measured Value Attribute
static int32u getMinMeasuredValue(double *minMeasuredValue);
//Set Minimum Measured Value Attribute
static int32u setMinMeasuredValue(double minMeasuredValue);
//Get Maximum Measured Value Attribute
static int32u getMaxMeasuredValue(double *maxMeasuredValue);
//Set Maximum Measured Value Attribute
static int32u setMaxMeasuredValue(double maxMeasuredValue);

/* --------------------- Public objects --------------------------------------*/
static const st_zb_temp_measure_cluster_server_t tempMeasureClass = {
		.getMaxMeasuredValue = &getMaxMeasuredValue,
		.getMeasuredValue = &getMeasuredValue,
		.getMinMeasuredValue = &getMinMeasuredValue,
		.setMaxMeasuredValue = &setMaxMeasuredValue,
		.setMeasuredValue = &setMeasuredValue,
		.setMinMeasuredValue = &setMinMeasuredValue,
};


/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_zb_temp_measure_cluster_server_t *gs_zb_temp_measure_cluster_class()
{
	return &tempMeasureClass;
}

static int32u getMeasuredValue(double *measuredValue)
{
	EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;

	int32u read = 0;
	status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
	ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID, (int16u*) &read,
			ZCL_INT16S_ATTRIBUTE_TYPE);

	*measuredValue = (double) (read / (double) 100);

	return status;
}

//Set Measured Value Attribute
static int32u setMeasuredValue(double measuredValue)
{
	EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
	int32u write = (int32u) (measuredValue * 100.0);
	double max, min;

	status = getMaxMeasuredValue(&max);
	status |= getMinMeasuredValue(&min);

	if ((boolean) status) {
		return status;
	}

	if ((max * 100.0) != GS_ZB_TEMP_MEASURE_UNKNOWN && measuredValue > max) {
		status |= EMBER_ZCL_STATUS_INVALID_VALUE;
	}

	if ((min * 100.0) != GS_ZB_TEMP_MEASURE_UNKNOWN && measuredValue < min) {
		status |= EMBER_ZCL_STATUS_INVALID_VALUE;
	}

	if (status) {
		write = GS_ZB_TEMP_MEASURE_UNKNOWN;
	}

	status = emberAfWriteServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
	ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID, (int16u*) &write,
			ZCL_INT16S_ATTRIBUTE_TYPE);

	return status;
}

//Get Minimum Measured Value Attribute
static int32u getMinMeasuredValue(double *minMeasuredValue)
{
	EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;

	int32u read = 0;
	status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
	ZCL_TEMP_MIN_MEASURED_VALUE_ATTRIBUTE_ID, (int8u*) &read,
			ZCL_INT16S_ATTRIBUTE_TYPE);

	*minMeasuredValue = (double) (read / (double) 100);

	return status;
}

//Set Minimum Measured Value Attribute
static int32u setMinMeasuredValue(double minMeasuredValue)
{
	EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;

	int32u write = (int32u) (minMeasuredValue * 100);

	if (write <= (int32u) 0x954D || write >= (int32u) 0x7FFE) {
		return EMBER_ZCL_STATUS_INVALID_VALUE;
	}

	status = emberAfWriteServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
	ZCL_TEMP_MIN_MEASURED_VALUE_ATTRIBUTE_ID, (int8u*) &write,
			ZCL_INT16S_ATTRIBUTE_TYPE);

	return status;
}

//Get Maximum Measured Value Attribute
static int32u getMaxMeasuredValue(double *maxMeasuredValue)
{
	EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;

	int32u read = 0;
	status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
	ZCL_TEMP_MAX_MEASURED_VALUE_ATTRIBUTE_ID, (int8u*) &read,
			ZCL_INT16S_ATTRIBUTE_TYPE);

	*maxMeasuredValue = (double) (read / (double) 100);

	return status;
}

//Set Maximum Measured Value Attribute
static int32u setMaxMeasuredValue(double maxMeasuredValue)
{
	EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;

	int32u write = (int32u) (maxMeasuredValue * 100);

	if (write <= (int32u) 0x954E || write >= (int32u) 0x7FFF) {
		return EMBER_ZCL_STATUS_INVALID_VALUE;
	}

	status = emberAfWriteServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
	ZCL_TEMP_MAX_MEASURED_VALUE_ATTRIBUTE_ID, (int8u*) &write,
			ZCL_INT16S_ATTRIBUTE_TYPE);

	return status;
}























