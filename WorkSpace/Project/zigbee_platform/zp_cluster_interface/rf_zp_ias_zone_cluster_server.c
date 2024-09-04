/**
 ******************************************************************************
 * @section company_section Company
 *
 *
 * @file	rf_zp_ias_zone_cluster_server.c
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
#include "ias-zone-server.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_zp_ias_zone_cluster_server.h"

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
#define GS_ZB_TEMP_MEASURE_UNKNOWN 		0x8000

/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
static uint16_t zoneStatus = 0;

/* --------------------- Private function prototypes -------------------------*/
static uint8_t getAlarm1(uint8_t endpoint, bool *isAlarm);
static uint8_t getAlarm2(uint8_t endpoint, bool *isAlarm);
static uint8_t getTamper(uint8_t endpoint, bool *isTamper);
static uint8_t getBattery(uint8_t endpoint, bool *isBattery);
static uint8_t getTrouble(uint8_t endpoint, bool *isTrouble);
//static uint8_t getAtTemp(uint8_t endpoint, bool *isTemp);


static uint8_t setAlarm1(uint8_t endpoint, bool isAlarm);
static uint8_t setAlarm2(uint8_t endpoint, bool isAlarm);
static uint8_t setTamper(uint8_t endpoint, bool isTamper);
static uint8_t setBattery(uint8_t endpoint, bool isBattery);
static uint8_t setTrouble(uint8_t endpoint, bool isTrouble);
//static uint8_t setAtTemp(uint8_t endpoint, bool isTemp);


/* --------------------- Public objects --------------------------------------*/
static const st_zp_ias_zone_cluster_server_class_t zoneClusterServerClass = {
	.getAlarm1 = &getAlarm1,
	.getAlarm2 = &getAlarm2,
	.getBattery = &getBattery,
	.getTamper = &getTamper,
	.getTrouble = &getTrouble,
	//.getAtTemp = &getAtTemp,


	.setAlarm1 = &setAlarm1,
	.setAlarm2 = &setAlarm2,
	.setBattery = &setBattery,
	.setTamper = &setTamper,
	.setTrouble = &setTrouble,
	//.setAtTemp = &setAtTemp,

};

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
const st_zp_ias_zone_cluster_server_class_t* gs_zp_ias_zone_cluster_server_class(
		void)
{
	return &zoneClusterServerClass;
}

static uint8_t gs_zp_ias_zone_status_update(uint8_t endpoint)
{
	static uint16_t lastZoneStatusReport = 0;

	if (lastZoneStatusReport != zoneStatus) {
		lastZoneStatusReport = zoneStatus;

		return emberAfPluginIasZoneServerUpdateZoneStatus(
				endpoint, lastZoneStatusReport, 100);
	}
	return EMBER_APPLICATION_ERROR_0;
}

static uint8_t getAlarm1(uint8_t endpoint, bool *isAlarm)
{
	(void)endpoint;
	*isAlarm = (bool) (zoneStatus & 0x0001);

	return EMBER_SUCCESS;
}

static uint8_t getAlarm2(uint8_t endpoint, bool *isAlarm)
{
	(void)endpoint;
	*isAlarm = (bool) (zoneStatus & 0x0001 << 1);

	return EMBER_SUCCESS;
}

static uint8_t getTamper(uint8_t endpoint, bool *isTamper)
{
	(void)endpoint;
	*isTamper = (bool) (zoneStatus & 0x0001 << 2);

	return EMBER_SUCCESS;
}

static uint8_t getBattery(uint8_t endpoint, bool *isBattery)
{
	(void)endpoint;
	*isBattery = (bool) (zoneStatus & 0x0001 << 3);

	return EMBER_SUCCESS;
}

static uint8_t getTrouble(uint8_t endpoint, bool *isTrouble)
{
	(void)endpoint;
	*isTrouble = (bool) (zoneStatus & 0x0001 << 6);

	return EMBER_SUCCESS;
}

//static uint8_t getAtTemp(uint8_t endpoint, bool *isTrouble)
//{
//	(void)endpoint;
//	*isTrouble = (bool) (zoneStatus & 0x0001 << 8);
//
//	return EMBER_SUCCESS;
//}


static uint8_t setAlarm1(uint8_t endpoint, bool isAlarm)
{
	if (isAlarm) {
		zoneStatus |= 0x0001;
	} else {
		zoneStatus &= ~0x0001;
	}
	return gs_zp_ias_zone_status_update(endpoint);
}

static uint8_t setAlarm2(uint8_t endpoint, bool isAlarm)
{
	if (isAlarm) {
		zoneStatus |= 0x0001 << 1;
	} else {
		zoneStatus &= ~(0x0001 << 1);
	}
	return gs_zp_ias_zone_status_update(endpoint);
}

static uint8_t setTamper(uint8_t endpoint, bool isTamper)
{
	if (isTamper) {
		zoneStatus |= 0x0001 << 2;
	} else {
		zoneStatus &= ~(0x0001 << 2);
	}
	return gs_zp_ias_zone_status_update(endpoint);
}

static uint8_t setBattery(uint8_t endpoint, bool isBattery)
{
	if (isBattery) {
		zoneStatus |= 0x0001 << 3;
	} else {
		zoneStatus &= ~(0x0001 << 3);
	}
	return gs_zp_ias_zone_status_update(endpoint);
}

static uint8_t setTrouble(uint8_t endpoint, bool isTrouble)
{
	if (isTrouble) {
		zoneStatus |= 0x0001 << 6;
	} else {
		zoneStatus &= ~(0x0001 << 6);
	}
	return gs_zp_ias_zone_status_update(endpoint);
}

//static uint8_t setAtTemp(uint8_t endpoint, bool isTrouble)
//{
//	if (isTrouble) {
//		zoneStatus |= 0x0001 << 6;
//	} else {
//		zoneStatus &= ~(0x0001 << 6);
//	}
//	return gs_zp_ias_zone_status_update(endpoint);
//}


