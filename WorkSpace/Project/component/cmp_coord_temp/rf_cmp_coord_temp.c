/******************************************************************************
 * @section company_section Company
 * 
 *
 * @file    rf_cmp_client_temp.c
 * @author  rafael.fischer
 * @date    16 de ago. de 2024
 *
 * @brief   Implementação para um cliente Zigbee para solicitar e receber
 *          leituras de temperatura de um servidor.
 ******************************************************************************
 */

/******************************************************************************
 *                          INCLUDES
 *****************************************************************************/
/* ---------------------- C language standard library ------------------------*/
#include <stdio.h>

/* ---------------------- Stack include --------------------------------------*/
#include "af.h"
#include "error-def.h"
#include "sl_zigbee_debug_print.h"

/* ---------------------- Others include -------------------------------------*/
#include "gs_cmp_coord_temp.h"

/******************************************************************************
 *                          DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/

#define TEMP_READ_INTERVAL_MS 2000

/* --------------------- Private typedef -------------------------------------*/

/* --------------------- Private variables -----------------------------------*/
static sl_zigbee_event_t tempReadEvent;
static uint16_t lastTemperature = 0; // Buffer para armazenar o último valor da temperatura
static bool callbackResult = false;  // Variável global para armazenar o resultado do callback

/* --------------------- Private function prototypes -------------------------*/
static void tempReadEventHandler(sl_zigbee_event_t *event);
//static void processTemperatureReading(uint16_t temperatureValue);

/******************************************************************************
 *                          FUNCTIONS
 *****************************************************************************/

// Função para processar a leitura de temperatura
//static void processTemperatureReading(uint16_t (temperatureValue)) {
//
//
//    // Converta o valor para graus Celsius, assumindo que o valor está em décimos de grau Celsius
//	uint16_t tempCelsius = (temperatureValue) / 10;
//	sl_zigbee_app_debug_println("Temperatura recebida2: %.2d°C", tempCelsius);
//
//    //sl_zigbee_app_debug_println("Resultado do callback: %s", callbackResult ? "TRUE" : "FALSE");
//
//}

// Callback para processar relatórios de atributos

//emberAfReportAttributesCallback
//emberAfReadAttributesResponseCallback
//emberAfWriteAttributesResponseCallback


bool emberAfReportAttributesCallback(EmberAfClusterId clusterId,
                                     uint8_t *buffer,
                                     uint16_t bufLen) {
    if (clusterId == ZCL_TEMP_MEASUREMENT_CLUSTER_ID) {
        // Verifique se o buffer não é NULL e tem comprimento suficiente
        if (buffer != NULL && bufLen >= 5) {
            // Imprima o conteúdo do buffer para depuração
            sl_zigbee_app_debug_println("Buffer recebido: %02X %02X %02X %02X %02X",
                                        buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);

            // Obtém o ID do atributo (2 bytes)
            uint16_t attributeId = (buffer[0] | (buffer[1] << 8));
            // Obtém o tipo de dado (1 byte)
            uint8_t dataType = buffer[2];

            // Imprima o ID do atributo e o tipo de dado para depuração
            sl_zigbee_app_debug_println("ID do atributo: %04X", attributeId);
            sl_zigbee_app_debug_println("Tipo de dado: %02X", dataType);

            // Verifique se o atributo é o valor medido de temperatura e o tipo de dado é INT16S
            if (attributeId == ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID && dataType == ZCL_INT16S_ATTRIBUTE_TYPE) {
                // Obtém o valor da temperatura (2 bytes)
                int temperatura = (int)((buffer[3] << 8) | buffer[4]);

                // Imprima o valor da temperatura para depuração
                sl_zigbee_app_debug_println("Temperatura recebida: %d°C", temperatura);


                sl_zigbee_app_debug_println("Valor da temperatura em hexadecimal: %04X", temperatura);




                // Atualiza o valor da temperatura e processa
                lastTemperature = temperatura;
                callbackResult = true;

                // Converte e processa a temperatura
               // processTemperatureReading(lastTemperature);

                sl_zigbee_app_debug_println("emberAfReportAttributesCallback retornou TRUE");
                return true; // Mensagem foi processada
            } else {
                sl_zigbee_app_debug_println("Atributo ou tipo de dado não correspondem.");
            }
        } else {
            sl_zigbee_app_debug_println("Buffer inválido ou comprimento insuficiente.");
        }
    } else {
        sl_zigbee_app_debug_println("Cluster ID não corresponde.");
    }

    // Se chegou até aqui, a mensagem não foi processada
    sl_zigbee_app_debug_println("emberAfReportAttributesCallback retornou FALSE");
    return false; // Mensagem não foi processada
}


// Função para lidar com eventos de leitura de temperatura
static void tempReadEventHandler(sl_zigbee_event_t *event) {
    // Em vez de solicitar a leitura diretamente, processa o último valor armazenado
    //processTemperatureReading(lastTemperature);

    // Reagendar o evento para ser acionado novamente após o intervalo especificado
    sl_zigbee_event_set_delay_ms(&tempReadEvent, TEMP_READ_INTERVAL_MS);
}

// Função de inicialização
uint8_t gs_cmp_coord_temp_init(void) {
    // Inicializar o evento de leitura de temperatura
    sl_zigbee_event_init(&tempReadEvent, tempReadEventHandler);
    sl_zigbee_event_set_delay_ms(&tempReadEvent, TEMP_READ_INTERVAL_MS);

    // Simular a execução do callback emberAfReportAttributesCallback
    // Usando valores de teste para verificar a funcionalidade
//    uint8_t testBuffer[] = {0x00, 0x00, 0x29, 0x07, 0xD0}; // Exemplo: 0x07D0 (2000) equivale a 20,00°C
//    uint16_t testBufLen = sizeof(testBuffer);
//
//
//    bool result = emberAfReportAttributesCallback(ZCL_TEMP_MEASUREMENT_CLUSTER_ID, testBuffer, testBufLen);

    return 0;

    // Verificar o resultado do callback
    //sl_zigbee_app_debug_println("Teste de emberAfReportAttributesCallback: %s", result ? "Sucesso" : "Falha");
}
