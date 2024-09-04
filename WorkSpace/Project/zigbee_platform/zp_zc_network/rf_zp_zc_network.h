/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_zp_zc_network.h
 * @author	rafael.fischer
 * @date	17 de jul. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_ZP_ZC_NETWORK_H_
#define GS_ZP_ZC_NETWORK_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/

//Call back for any packet received or send
typedef void (*gs_zb_packet_indicator)(boolean isTx);

/* -------------------------- Public objects ---------------------------------*/

typedef struct {
	/**
	* @brief		Put your description here.
	* @pre-cond.	Conditions needed to work, if it exists, otherwise, delete the line.
	* @post-cond.	Conditions achieved after executed, if it exists, otherwise, delete the line..
	* @param		[in] varName Description for input variables.
	* @param		[out] varName Description for output variables..
	* @return		Return value description..
	* @note 		If exists any note to this method.
	* @warning 	If exists any warning to this method.
	* @see 		Method Name or any other thing to see additionally
	*/
	uint8_t (*init)(bool isEnableCli);

	uint8_t (*openNetwork) (EmberEUI64 eui64);

	//Register callback for Zigbee Rx/Tx Packet
	int8u (*registerPacketIndicatorCallback)(gs_zb_packet_indicator cb);

}st_zp_zc_network_class_t;

/**
 * @brief		Primary class Method
 * @return		The virtual methods of the class.
 */
const st_zp_zc_network_class_t *gs_zp_zc_network_class(void);



#endif /* GS_ZP_ZC_NETWORK_H_ */
