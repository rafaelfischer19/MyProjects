/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_custom_tokens.h
 * @author	rafael.fischer
 * @date	3 de set. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_CUSTOM_TOKENS_H_
#define GS_CUSTOM_TOKENS_H_

/* -------------------------- Includes ---------------------------------------*/
/* -------------------------- Define -----------------------------------------*/
/* -------------------------- Typedef ----------------------------------------*/
/* -------------------------- Public objects ---------------------------------*/

//#include "sl_token_api.h"

#define TOKEN_ALARM_STATUS 0x0001
#define TOKEN_ALARM_STATUS_SIZE sizeof(bool) // Ajuste conforme o tipo de dado

// Definições de tokens personalizados
#define ALARM_STATUS_DEFAULT   false

#ifdef DEFINETYPES
// Definição do tipo de dado do token
typedef bool tokTypeAlarmStatus;
#endif

#ifdef DEFINETOKENS
// Definição do token para armazenar o status do alarme
DEFINE_BASIC_TOKEN(ALARM_STATUS,
                   tokTypeAlarmStatus,
                   ALARM_STATUS_DEFAULT)
#endif




#endif /* GS_CUSTOM_TOKENS_H_ */
