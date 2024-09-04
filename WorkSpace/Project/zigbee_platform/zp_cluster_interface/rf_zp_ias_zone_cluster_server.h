/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_ias_zone_cluster_server.h
 * @author	rafael.fischer
 * @date	23 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_ZP_IAS_ZONE_CLUSTER_SERVER_H_
#define GS_ZP_IAS_ZONE_CLUSTER_SERVER_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
typedef struct {
		uint8_t (*setAlarm1)(uint8_t endPoint, bool isAlarm);
		uint8_t (*getAlarm1)(uint8_t endPoint, bool *isAlarm);

		uint8_t (*setAlarm2)(uint8_t endPoint, bool isAlarm);
		uint8_t (*getAlarm2)(uint8_t endPoint, bool *isAlarm);

		uint8_t (*setTamper)(uint8_t endPoint, bool isTamper);
		uint8_t (*getTamper)(uint8_t endPoint, bool *isTamper);

		uint8_t (*setBattery)(uint8_t endPoint, bool isBattery);
		uint8_t (*getBattery)(uint8_t endPoint, bool *isBattery);

		uint8_t (*setTrouble)(uint8_t endPoint, bool isTrouble);
		uint8_t (*getTrouble)(uint8_t endPoint, bool *isTrouble);

		//uint8_t (*setAtTemp)(uint8_t endPoint, bool isTemp);
		//uint8_t (*getAtTemp)(uint8_t endPoint, bool *isTemp);


} st_zp_ias_zone_cluster_server_class_t;



/* -------------------------- Public objects ---------------------------------*/

const st_zp_ias_zone_cluster_server_class_t* gs_zp_ias_zone_cluster_server_class(
		void);


#endif /* GS_ZP_IAS_ZONE_CLUSTER_SERVER_H_ */
