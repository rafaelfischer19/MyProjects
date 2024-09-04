/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_custom_cluster.h
 * @author	rafael.fischer
 * @date	16 de ago. de 2024
 *
 *  @brief 	how this library works
 *         	describe how to use this library and what this implements.
 ******************************************************************************
 */

#ifndef GS_CUSTOM_CLUSTER_H
#define GS_CUSTOM_CLUSTER_H

#include "app/framework/include/af.h"

// Definindo o ID do Cluster Personalizado
#define CUSTOM_CLUSTER_ID 0x1234

// Definindo o Comando Personalizado
#define CUSTOM_COMMAND_HELLO 0x01

// Função para enviar a mensagem "Hello"
void sendHelloMessage(EmberNodeId destination, uint8_t endpoint);

// Função para iniciar o temporizador
void startHelloTimer(void);

#endif // GS_CUSTOM_CLUSTER_H
