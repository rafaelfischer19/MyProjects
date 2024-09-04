/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_custom_cluster.c
 * @author	rafael.fischer
 * @date	16 de ago. de 2024
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
/* ---------------------- Others include -------------------------------------*/

/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
/* --------------------- Private variables -----------------------------------*/
/* --------------------- Private function prototypes -------------------------*/
/* --------------------- Public objects --------------------------------------*/

/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/
#include "gs_custom_cluster.h"
#include "app/framework/util/attribute-storage.h"

#define HELLO_TIMER_ID 0x01
#define HELLO_INTERVAL_MS 1000 // Intervalo de 1 segundo

static EmberEventControl helloTimerEventControl;

// Função para enviar a mensagem "Hello"
void sendHelloMessage(EmberNodeId destination, uint8_t endpoint) {
    EmberStatus status;
    uint8_t payload[] = "Hello";
    uint8_t payloadLength = sizeof(payload) - 1;  // Excluindo o terminador nulo

    EmberMessageBuffer message = emberFillBufferWithData(payloadLength, payload);

    // Enviar mensagem unicast
//    status = emberAfSendUnicast(EMBER_OUTGOING_DIRECT, destination, endpoint, CUSTOM_CLUSTER_ID, CUSTOM_COMMAND_HELLO, payloadLength, message);
//    if (status != EMBER_SUCCESS) {
//        emberAfAppPrintln("Failed to send Hello message: %d", status);
//    } else {
//        emberAfAppPrintln("Hello message sent successfully.");
//    }
}

// Função do temporizador para enviar a mensagem "Hello"
void helloTimerEventHandler(void) {
    EmberNodeId destination = 0x1234; // ID do nó de destino
    uint8_t endpoint = 1; // Endpoint do dispositivo de destino

    sendHelloMessage(destination, endpoint);

    // Reagendar o temporizador
    emberEventControlSetDelayMS(helloTimerEventControl, HELLO_INTERVAL_MS);
}

// Função para iniciar o temporizador
//void startHelloTimer(void) {
//    emberEventControlSetActive(helloTimerEventControl);
//}

//// Inicialização do Temporizador
//void emberAfMainInitCallback(void) {
//    emberEventControlSetInactive(helloTimerEventControl);
//    startHelloTimer(); // Iniciar o temporizador ao inicializar
//}

// Manipulador de Comando Personalizado
void customClusterCommandHandler(uint8_t commandId, uint8_t* payload, uint16_t payloadLength) {
    if (commandId == CUSTOM_COMMAND_HELLO) {
        emberAfAppPrintln("Received Hello message: %s", payload);
    } else {
        emberAfAppPrintln("Unknown command ID: %d", commandId);
    }
}

//// Função de callback do ZCL para receber comandos
//void emberAfClusterCommandHandler(uint8_t endpoint, EmberClusterId clusterId, uint8_t commandId, uint8_t* payload, uint16_t payloadLength) {
//    if (clusterId == CUSTOM_CLUSTER_ID) {
//        customClusterCommandHandler(commandId, payload, payloadLength);
//    }
//}

