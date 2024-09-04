/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_zc_network.c
 * @author	rafael.fischer
 * @date	17 de jul. de 2024
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
#include PLATFORM_HEADER
#include "af-types.h"
#include "error-def.h"
#include "mac-types.h"
#include "ember-types.h"
#include "af-security.h"
#include "network-creator.h"
#include "network-creator-security.h"
#include "find-and-bind-target.h"
/* ---------------------- Others include -------------------------------------*/
#include "../../cli/gs_cli_interface.h"
#include "../zp_common/gs_zp_install_code.h"
#include "gs_zp_zc_network.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static gs_zb_packet_indicator zbPckIndicatorCb = NULL;

extern sl_cli_command_group_t gs_cli_zc_network_main_group;


/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_zp_network_init(bool isEnableCli);
static uint8_t gs_zp_open_network(EmberEUI64 eui64);

static int8u gs_reg_pckt_indicator_callback(gs_zb_packet_indicator cb);

/* --------------------- Public objects --------------------------------------*/
static const st_zp_zc_network_class_t zcNetworkClass = {
			.init = &gs_zp_network_init,
			.openNetwork = &gs_zp_open_network,
			.registerPacketIndicatorCallback = &gs_reg_pckt_indicator_callback
};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

const st_zp_zc_network_class_t* gs_zp_zc_network_class(void)
{
	return &zcNetworkClass;
}

static uint8_t gs_zp_network_init(bool isEnableCli)
{
	uint8_t status = EMBER_APPLICATION_ERROR_0;
	status = emberAfPluginNetworkCreatorStart(true);

	if (isEnableCli) {
		gs_cli_interface_class()->addGroup(&gs_cli_zc_network_main_group);
	}
	return status;
}

static uint8_t gs_zp_open_network(EmberEUI64 eui64)
{
	uint8_t status = EMBER_APPLICATION_ERROR_0;
	EmberKeyData KeyData;

	uint8_t bufferInstallCode[GS_INSTALL_CODE_SIZE] = { 0, };

	status = gs_zp_install_code_class()->getinstallCodeKeyFromEui64(
			eui64, &bufferInstallCode[0], GS_INSTALL_CODE_SIZE);

	if (status) {
		return status;
	}

	status = sli_zigbee_af_install_code_to_key(&bufferInstallCode[0],
												GS_INSTALL_CODE_SIZE, &KeyData);

	if (status) {
		return status;
	}

	status = emberAfPluginNetworkCreatorSecurityOpenNetworkWithKeyPair(eui64,
																		KeyData);


	emberAfPluginFindAndBindTargetStart(emberAfPrimaryEndpoint());

	return status;
}

bool emberAfPreMessageReceivedCallback(EmberAfIncomingMessage* incomingMessage)
{
	if(zbPckIndicatorCb != NULL){
		zbPckIndicatorCb(false);
	}

  return false;
}

bool emberAfPreMessageSendCallback(EmberAfMessageStruct* messageStruct,
                                        EmberStatus* status)
{
	if (zbPckIndicatorCb != NULL) {
		zbPckIndicatorCb(true);
	}
  return false;
}

static int8u gs_reg_pckt_indicator_callback(gs_zb_packet_indicator cb)
{
	if(zbPckIndicatorCb == NULL && cb != NULL){
		zbPckIndicatorCb = cb;
		return EMBER_SUCCESS;
	}
	return EMBER_APPLICATION_ERROR_0;
}



























