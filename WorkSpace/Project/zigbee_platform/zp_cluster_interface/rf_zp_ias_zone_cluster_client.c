/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_ias_zone_cluster_client.c
 * @author	rafael.fischer
 * @date	14 de ago. de 2024
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
//#include "find-and-bind-target.h"
#include "zap-id.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_zp_ias_zone_cluster_client.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
#define GS_SPECIFIC_ENROLL_REPORT		0xAC


/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
/* --------------------- Public objects --------------------------------------*/

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

static int8u gs_cmp_ias_client_write_cie_ieee(int16u nodeID, int8u destEp)
{
	int8u status = EMBER_SUCCESS;

	emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), destEp);

	uint8_t writeAttributes[] = {
			LOW_BYTE(ZCL_IAS_CIE_ADDRESS_ATTRIBUTE_ID),
			HIGH_BYTE(ZCL_IAS_CIE_ADDRESS_ATTRIBUTE_ID),
			ZCL_IEEE_ADDRESS_ATTRIBUTE_TYPE,
			0, 0, 0, 0, 0, 0, 0, 0, // ieee (filled in later)
			};

	emberAfGetEui64(&writeAttributes[3]);

	emberAfFillCommandGlobalClientToServerWriteAttributes(
			ZCL_IAS_ZONE_CLUSTER_ID, writeAttributes, sizeof(writeAttributes));

	status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, nodeID);
	emberAfIasZoneClusterPrintln("Writing CIE Address to IAS Zone Server: 0x%X",
			status);

	return status;
}

static int32u gs_cmp_ias_client_command(sl_service_opcode_t opcode,
		sl_service_function_context_t *context)
{
	EmberAfIasEnrollResponseCode responseCode =
			EMBER_ZCL_IAS_ENROLL_RESPONSE_CODE_SUCCESS;
	int8u zoneId = 0x00;
	int8u status = EMBER_SUCCESS;

	assert(opcode == SL_SERVICE_FUNCTION_TYPE_ZCL_COMMAND);
	EmberAfClusterCommand *cmd = (EmberAfClusterCommand*) context->data;

	if (!cmd->mfgSpecific) {
		switch (cmd->commandId) {
		case ZCL_ZONE_ENROLL_REQUEST_COMMAND_ID:

			emberAfFillCommandIasZoneClusterZoneEnrollResponse(responseCode,
					zoneId);

			// Need to send this command with our source EUI because the server will
			// check our EUI64 against his CIE Address to see if we're his CIE.
			emberAfGetCommandApsFrame()->options |=
					EMBER_APS_OPTION_SOURCE_EUI64;
			status = emberAfSendResponse();
			emberAfCorePrintln(
					"Sent enroll response with responseCode: 0x%X, zoneId: 0x%X, status: 0x%X",
					responseCode, zoneId, status);
			if (status == EMBER_SUCCESS) {
				return EMBER_ZCL_STATUS_SUCCESS;
			} else {
				return EMBER_ZCL_STATUS_FAILURE;
			}
			break;
		case ZCL_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID:
			emberAfSendDefaultResponse(cmd, EMBER_ZCL_STATUS_SUCCESS);
			return EMBER_ZCL_STATUS_SUCCESS;
			break;
		default:
			break;
		}
	}
	//keep returning error for the stack to do what it has to do
	return EMBER_ZCL_STATUS_UNSUP_COMMAND;
}

static int32u gs_cmp_ota_config_command(sl_service_opcode_t opcode,
		sl_service_function_context_t *context)
{
	assert(opcode == SL_SERVICE_FUNCTION_TYPE_ZCL_COMMAND);
	EmberAfClusterCommand *cmd = (EmberAfClusterCommand*) context->data;

	switch (cmd->commandId) {
//	case ZCL_RESET_INFO_REPORT_COMMAND_ID:
//		//Send a write attribute for this device
//		if (cmd->apsFrame->sourceEndpoint < 4) {
//			gs_cmp_ias_client_write_cie_ieee(cmd->source,
//					cmd->apsFrame->sourceEndpoint);
//		}
//		break;
	case GS_SPECIFIC_ENROLL_REPORT:
		//Send a write attribute for this device
		if (cmd->apsFrame->sourceEndpoint < 4) {
			gs_cmp_ias_client_write_cie_ieee(cmd->source,
					cmd->apsFrame->sourceEndpoint);
		}
		break;
	default:
		break;
	}
	//keep returning error for the stack to do what it has to do
	return EMBER_ZCL_STATUS_UNSUP_COMMAND;
}

int8u zb_zc_ias_zone_client_init(void)
{
	int8u status = EMBER_SUCCESS;

	status = (int8u) sl_zigbee_subscribe_to_zcl_commands(
	ZCL_IAS_ZONE_CLUSTER_ID, 0xFFFF,
	ZCL_DIRECTION_SERVER_TO_CLIENT, gs_cmp_ias_client_command);

	status = (int8u) sl_zigbee_subscribe_to_zcl_commands(
	ZCL_OTA_CONFIGURATION_CLUSTER_ID, 0x1002,
	ZCL_DIRECTION_SERVER_TO_CLIENT, gs_cmp_ota_config_command);

	return status;
}
