/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_basic_cluster_server.c
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
#include "af.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_zp_basic_cluster_server.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
static uint8_t setMfgName(uint8_t *name);
static uint8_t getMfgName(uint8_t *name);
static uint8_t setModel(uint8_t *model);
static uint8_t getModel(uint8_t *model);

/* --------------------- Public objects --------------------------------------*/
const st_zp_basic_cluster_server_class_t basicClusterServerClass = {
	.setMfgName = &setMfgName,
	.getMfgName = &getMfgName,
	.setModel = &setModel,
	.getModel = &getModel

};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_zp_basic_cluster_server_class_t* gs_zp_basic_cluster_server_class(void)
{
	return &basicClusterServerClass;
}

static uint8_t getModel(uint8_t *model)
{
	uint8_t ModelNameRead[32] = { 0, };
	emberAfReadServerAttribute(emberAfPrimaryEndpoint(), ZCL_BASIC_CLUSTER_ID,
	ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
								ModelNameRead, sizeof(ModelNameRead));
	uint8_t size = ModelNameRead[0];
	if (size == 0 || size > 32) {
		return EMBER_APPLICATION_ERROR_0;
	}
	MEMCOPY(model, &model, ++size);
	return 0;
}

static uint8_t setModel(uint8_t *model)
{
	uint8_t size = model[0];
	if (size > 32) {
		return EMBER_APPLICATION_ERROR_0;
	}
	return emberAfWriteServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_BASIC_CLUSTER_ID,
										ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
										model, ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
}

static uint8_t getMfgName(uint8_t *name)
{
	uint8_t mfgNameRead[32] = { 0, };
	emberAfReadServerAttribute(emberAfPrimaryEndpoint(), ZCL_BASIC_CLUSTER_ID,
	ZCL_MANUFACTURER_NAME_ATTRIBUTE_ID,
								mfgNameRead, sizeof(mfgNameRead));
	uint8_t size = mfgNameRead[0];
	if (size == 0 || size > 32) {
		return EMBER_APPLICATION_ERROR_0;
	}
	MEMCOPY(name, &mfgNameRead, ++size);
	return 0;
}

static uint8_t setMfgName(uint8_t *name)
{
	uint8_t size = name[0];
	if (size > 32) {
		return EMBER_APPLICATION_ERROR_0;
	}
	return emberAfWriteServerAttribute(emberAfPrimaryEndpoint(),
	ZCL_BASIC_CLUSTER_ID,
										ZCL_MANUFACTURER_NAME_ATTRIBUTE_ID,
										name, ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
}
