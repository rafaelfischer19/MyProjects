/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_temp_measure_cluster_server.h
 * @author	rafael.fischer
 * @date	13 de ago. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_TEMP_MEASURE_CLUSTER_SERVER_H_
#define GS_TEMP_MEASURE_CLUSTER_SERVER_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
typedef struct {

	//Get Measured Value Attribute - °C (e.g. 57,93°C)
int32u (*getMeasuredValue)(double *measuredValue);
	//Set Measured Value Attribute - °C (e.g. 57,93°C)
	int32u (*setMeasuredValue)(double measuredValue);

	//Get Minimum Measured Value Attribute - °C (e.g. 57,93°C)
	int32u (*getMinMeasuredValue)(double *minMeasuredValue);
	//Set Minimum Measured Value Attribute - °C (e.g. 57,93°C)
	int32u (*setMinMeasuredValue)(double minMeasuredValue);

	//Get Maximum Measured Value Attribute - °C (e.g. 57,93°C)
	int32u (*getMaxMeasuredValue)(double *maxMeasuredValue);
	//Set Maximum Measured Value Attribute - °C (e.g. 57,93°C)
	int32u (*setMaxMeasuredValue)(double maxMeasuredValue);

} st_zb_temp_measure_cluster_server_t;

/* -------------------------- Public objects ---------------------------------*/

const st_zb_temp_measure_cluster_server_t *gs_zb_temp_measure_cluster_class();


#endif /* GS_TEMP_MEASURE_CLUSTER_SERVER_H_ */
