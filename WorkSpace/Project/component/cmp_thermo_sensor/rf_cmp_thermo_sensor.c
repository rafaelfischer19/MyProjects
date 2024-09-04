/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file    rf_cmp_thermo_sensor.c
 * @author  rafael.fischer
 * @date    July 26, 2024
 *
 * @brief   Describes how to use this library and what it implements.
 ******************************************************************************
 */

/******************************************************************************
 *                          INCLUDES
 *****************************************************************************/
/* ---------------------- C language standard library ------------------------*/
#include <stdio.h> // Standard I/O functions
#include <time.h> // Time-related functions
#include "sl_zigbee_debug_print.h" // Zigbee debug print functions
#include "sl_component_catalog.h" // Component catalog
#include "nvm3.h"
#include "nvm3_hal_flash.h"

/* ---------------------- Stack include --------------------------------------*/
#include "af.h" // Application framework for Zigbee
#include "error-def.h" // Error definitions for Zigbee
#include "zigbee_app_framework_event.h" // Zigbee application framework events
#include "sl_zigbee_debug_print.h" // Debug print functions for Zigbee

/* ---------------------- Others include -------------------------------------*/
#include "../../zigbee_platform/zp_cluster_interface/gs_zp_ias_zone_cluster_server.h" // IAS Zone Cluster server interface
#include "../../zigbee_platform/zp_cluster_interface/gs_temp_measure_cluster_server.h" // Temperature Measurement Cluster server interface
#include "../../zigbee_platform/zp_zed_network/gs_zp_zed_network.h" // Zigbee End Device network interface
#include "../../util/gs_board.h" // Board utilities
#include "gs_cmp_thermo_sensor.h" // Thermo sensor component header

/******************************************************************************
 *                          DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/

#define GS_THERMO_EVENT_TIME_MS 1000 // Timer event interval for the temperature sensor
#define HEAT_MAX_DELTA_STATIC_RESPONSE 200 // Maximum temperature delta for static response
#define HEAT_TEMP_HYSTERESIS 200 // Temperature hysteresis for triggering alarms
#define SAMPLE_LIST_SIZE 31 // Number of samples in the temperature sample list
#define SAMPLE_LIST_THRESHOLD (SAMPLE_LIST_SIZE - 1) // Threshold index for a full sample list

#define TEMP_MEASUREMENT_CLUSTER_ID 0x0402 // Cluster ID for Temperature Measurement
#define TEMP_MEASURED_VALUE_ATTRIBUTE_ID 0x0000 // Attribute ID for Measured Value

#define GS_ALARM_STATUS_KEY    0x0001 // Chave para armazenar o status do alarme na NVM
#define NVM3_DEFAULT_HANDLE    nvm3_defaultHandle
#define NVM3_CACHE_SIZE        10  // Defina o tamanho do cache aqui
#define NVM3_SECTOR_SIZE       4096  // Valor típico, ajuste conforme necessário

//#define ALARM_STATUS_FLASH_ADDRESS 0x000a0000 // Flash memory address for storing alarm status
//#define NUM_BYTES 4 // Number of bytes to write in flash memory
//#define FLASH_PAGE_SIZE 4096 // Flash page size (commented out)

/* --------------------- Private typedef -------------------------------------*/

typedef struct {
    int16_t tempCelsius; // Temperature in Celsius
    boolean isAlert; // Alert status flag
    uint16_t tempAlarm; // Temperature when the alarm was triggered
} HeatInfo;

typedef enum {
    enNormal, // Normal operation mode
    enPreDetection, // Pre-detection mode for temperature monitoring
    enAlarm, // Alarm mode when temperature exceeds threshold
    enWaitReset // Wait for reset after an alarm is triggered
} en_thermo_state_machine_t; // Thermo sensor state machine states

/* --------------------- Private variables -----------------------------------*/
int thresholds[][2] = {
    {0, 2900},    // > 29°C
    {21, 2100},   // > 21°C
    {20, 2000},   // > 20°C
    {10, 1000},   // > 10°C
    {29, 2000},   // > 20°C
    {29, 3000}    // > 30°C
}; // Temperature thresholds and deltas for alert conditions

char* messages[] = {
    "Reporting for > 29°C\n", // Message for temperature > 29°C
    "Reporting for > 21°C\n", // Message for temperature > 21°C
    "Reporting for > 20°C\n", // Message for temperature > 20°C
    "Reporting for > 10°C\n", // Message for temperature > 10°C
    "Reporting for > 20°C\n", // Message for temperature > 20°C
    "Reporting for > 30°C\n"  // Message for temperature > 30°C
}; // Messages corresponding to each temperature threshold

static sl_zigbee_event_t thermoSensorEvent; // Event structure for the temperature sensor
//static sl_zigbee_event_t thermoDelayEvent; // Delay event structure (commented out)
static en_thermo_state_machine_t enThermoStateMachine = enNormal; // Initial state machine state
static st_vi_instance_t *stThermoSensorVi = NULL; // Pointer to virtual interface instance
static uint16_t sampleList[SAMPLE_LIST_SIZE] = { 0 }; // List of temperature samples
static HeatInfo heatInfo = {0}; // Heat information structure
static uint16_t lastTemperature = 0; // Last recorded temperature
//bool alarmStatus = false; // Initial alarm status (false)
//static uint16_t temperatura = 2400;  // Example temperature in Celsius (commented out)

/* --------------------- Private function prototypes -------------------------*/
static void gs_cmp_thermo_sensor_event_func(sl_zigbee_event_t *event); // Event function for the temperature sensor
//static void thermoDelayEventHandler(sl_zigbee_event_t *event); // Delay event handler (commented out)
static void addSample(uint16_t temp); // Function to add a temperature sample to the list
static bool checkStaticResponse(void); // Function to check if the static response indicates an alert
static void processHeatDetection(void); // Function to process heat detection and trigger alarms
static int16_t getSample(void); // Function to get a temperature sample from the sensor
static void reportTemperatureVariation(void); // Function to report temperature variation
static uint8_t load_alarm_status(void);
static void save_alarm_status(bool alarmActive);
static void nvm3_init(void);
static void reportTemperatureViaZCL(void); // Function to report temperature via Zigbee Cluster Library (ZCL)
///void saveAlarmStatus(bool isAlarmActive); // Function to save alarm status in flash memory

/******************************************************************************
 *                          FUNCTIONS
 *****************************************************************************/

static void nvm3_init(void)
{
    Ecode_t result; // Declare a variable to store the result of the NVM3 initialization

    static nvm3_Init_t nvm3_initData = { // Define and initialize the NVM3 initialization structure
        .nvmAdr = (uint32_t *)0x0FE00000, // Set the base address for NVM3 memory region
        .nvmSize = 0x20000, //128kb // Specify the size of the NVM3 memory region (128 KB)
        .cachePtr = NULL, // Set cache pointer to NULL (no cache is used)
        .cacheEntryCount = NVM3_CACHE_SIZE, // Specify the number of cache entries (defined elsewhere)
        .repackHeadroom = NVM3_SECTOR_SIZE // Usando o tamanho típico de setor
        // Set the repack headroom size, typically equal to the size of a sector
    };

    result = nvm3_open(NVM3_DEFAULT_HANDLE, &nvm3_initData);
    // Initialize the NVM3 module with the provided parameters and store the result

    if (result != ECODE_OK) {
        // Check if the initialization was unsuccessful
        sl_zigbee_app_debug_println("Failed to initialize NVM3: %d", result);
        // Print an error message with the result code if initialization failed
    }
}

/**
 * @brief Function to load the alarm status from NVM3
 */
static uint8_t load_alarm_status(void)
{
    bool alarmActive; // Declare a variable to store the alarm status (active or inactive)

    Ecode_t result = nvm3_readData(NVM3_DEFAULT_HANDLE, GS_ALARM_STATUS_KEY, &alarmActive, sizeof(alarmActive));
    // Read the alarm status from NVM3 memory using the key 'GS_ALARM_STATUS_KEY' and store the result in 'alarmActive'
    // The size of the data being read is the size of 'alarmActive' (a boolean)

    if (result == ECODE_OK) {
        // Check if the read operation was successful (result equals ECODE_OK)
        return alarmActive;
        // Return the value of 'alarmActive' (1 if active, 0 if inactive)
    } else {
        return 0; // Return 0 (inactive) if the read operation failed
    }
}

/**
 * @brief Function to save the alarm status to NVM3
 */
static void save_alarm_status(bool alarmActive)
{
    Ecode_t result = nvm3_writeData(NVM3_DEFAULT_HANDLE, GS_ALARM_STATUS_KEY, &alarmActive, sizeof(alarmActive));
    // Attempt to write the alarm status to NVM3 using the key 'GS_ALARM_STATUS_KEY'
    // The data to be written is the value of 'alarmActive', and the result of the operation is stored in 'result'

    if (result != ECODE_OK) {
        // Check if the write operation failed by comparing 'result' to 'ECODE_OK'
        sl_zigbee_app_debug_println("Failed to save alarm status: %d", result);
        // If the write operation failed, print an error message with the result code
    }
}

//void teste(void) {
//    // Function to print the list of temperature samples (for debugging)
//    for (uint8_t i = 0; i < SAMPLE_LIST_SIZE; i++) {
//        sl_zigbee_app_debug_print("[%d] ", sampleList[i]); // Print each sample
//    }
//    sl_zigbee_app_debug_println("\n\n\n\n\n\n\n\n"); // Print new lines for separation
//}

// Function to check if the static response indicates an alert condition
bool checkStaticResponse(void) {
    int16_t difference = lastTemperature - sampleList[SAMPLE_LIST_SIZE - 1]; // Calculate the temperature difference
    return difference < 200; // Return true if the difference is less than the threshold
}

void addSample(uint16_t temp) {
    for (uint8_t i = SAMPLE_LIST_SIZE - 1; i > 0; i--) {
        sampleList[i] = sampleList[i - 1]; // Shift all samples down the list
    }
    sampleList[0] = temp; // Add the new sample to the start of the list
    lastTemperature = temp; // Update the last recorded temperature
}

void processHeatDetection(void) {
    // Main function to process heat detection and manage alerts
    //gs_zp_ias_zone_cluster_server_class()->setAtTemp(emberAfPrimaryEndpoint(), false); // Example function call (commented out)
    getSample(); // Get a new temperature sample from the sensor
    addSample(heatInfo.tempCelsius); // Add the sample to the list

    bool isAlert = false;
    if (heatInfo.tempCelsius >= 3000) { // Check if temperature exceeds alert threshold (5400)
        isAlert = checkStaticResponse() || heatInfo.tempCelsius >= 3200; // Check for static response or high temperature (6400)
    }

    if (isAlert != heatInfo.isAlert) { // Check if alert status has changed
        heatInfo.isAlert = isAlert; // Update alert status
        if (heatInfo.isAlert) {
            heatInfo.tempAlarm = heatInfo.tempCelsius; // Set the alarm temperature
            reportTemperatureVariation();
            //gs_cmp_thermo_sensor_event_func(&thermoSensorEvent); // Trigger the event function
//            sl_zigbee_event_set_active(&thermoSensorEvent); // Activate the event (commented out)
        }
    }

//    if (sampleList[SAMPLE_LIST_SIZE - 1] - sampleList[0] > 200) {
//        sl_zigbee_app_debug_println("Reporting for > 29°C\n");
//        reportTemperatureVariation();
//
//    } else if (sampleList[SAMPLE_LIST_SIZE - 1] - sampleList[21] > 2100) {
//        sl_zigbee_app_debug_println("Reporting for > 21°C\n");
//        reportTemperatureVariation();
//
//    } else if (sampleList[SAMPLE_LIST_SIZE - 1] - sampleList[20] > 2000) {
//        sl_zigbee_app_debug_println("Reporting for K > 20°C\n");
//        reportTemperatureVariation();
//
//    } else if (sampleList[SAMPLE_LIST_SIZE - 1] - sampleList[10] > 1000) {
//        sl_zigbee_app_debug_println("Reporting for K > 10°C\n");
//        reportTemperatureVariation();
//
//    } else if (sampleList[SAMPLE_LIST_SIZE - 1] - sampleList[29] > 2000) {
//        sl_zigbee_app_debug_println("Reporting for K > 2°C\n");
//        reportTemperatureVariation();
//
//    } else if (sampleList[SAMPLE_LIST_SIZE - 1] - sampleList[29] > 3000) {
//        sl_zigbee_app_debug_println("Reporting for K > 2x°C\n");
//        reportTemperatureVariation();
//    }

    // Check temperature against thresholds and report if necessary
    for (uint8_t i = 0; i < sizeof(thresholds) / sizeof(thresholds[0]); i++) {
        if (sampleList[SAMPLE_LIST_SIZE - 1] - sampleList[thresholds[i][0]] > thresholds[i][1]) {
            sl_zigbee_app_debug_println(messages[i]); // Print the corresponding message

            if (i == 0) {
                gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), true); // Set alarm for the first condition
            }

            reportTemperatureVariation(); // Report temperature variation
            break; // Exit the loop after the first condition is met
        }
    }

    // Check if the temperature is within a valid range for reporting
    if (heatInfo.tempCelsius >= -4000 && heatInfo.tempCelsius <= 8500) {
        EmberAfStatus status = gs_zb_temp_measure_cluster_class()->setMeasuredValue(heatInfo.tempCelsius); // Set the measured temperature value
        if (status != EMBER_ZCL_STATUS_SUCCESS) {
            sl_zigbee_app_debug_println("Failed to set temperature!"); // Print error message if setting temperature fails
        }
        // Uncomment the following line if you need to set an attribute with the IAS Zone Cluster
        // gs_zp_ias_zone_cluster_server_class()->setAtTemp(emberAfPrimaryEndpoint(), true); // Set attribute (commented out)
    }

    reportTemperatureViaZCL(); // send
}

void reportTemperatureVariation(void) {

    // Set the alarm status on the IAS Zone Cluster Server to "true"
    gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), true);



//    bool newAlarmStatus = true;  // Alarm is activated

    // If the visual indication object is not null, set its state to "Fire"
    if (stThermoSensorVi != NULL) {
        stThermoSensorVi->setViState(stThermoSensorVi, enFire);
    }

    // Update the alarm status and save it to flash memory
//    alarmStatus = newAlarmStatus;
//    saveAlarmStatus(alarmStatus);
    save_alarm_status(true);
}

void reportTemperatureViaZCL(void) {
    // Define the cluster and attribute to be reported
    uint16_t clusterId = ZCL_TEMP_MEASUREMENT_CLUSTER_ID;
    uint16_t attributeId = ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID;
    uint16_t temperature = heatInfo.tempCelsius;  // Assume heatInfo.tempCelsius is in centigrade

    // Buffer to store the report attribute record
    uint8_t reportAttributeRecords[5];
    reportAttributeRecords[0] = LOW_BYTE(attributeId);  // Attribute ID (lower byte)
    reportAttributeRecords[1] = HIGH_BYTE(attributeId); // Attribute ID (upper byte)
    reportAttributeRecords[2] = ZCL_INT16S_ATTRIBUTE_TYPE;  // Data type of the attribute
    reportAttributeRecords[3] = LOW_BYTE(temperature);  // Temperature value (lower byte)
    reportAttributeRecords[4] = HIGH_BYTE(temperature); // Temperature value (upper byte)

    // Fill the ZCL command to report the attribute
    emberAfFillCommandGlobalServerToClientReportAttributes(clusterId,
                                                           reportAttributeRecords,
                                                           sizeof(reportAttributeRecords));

    // Send the command to the client directly (adjust the sending method as needed)
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0);

    //sl_zigbee_app_debug_println("Temperature report sent successfully: %d°C", temperature);

}

// Gets a temperature sample
int16_t getSample(void) {
    //teste();  // Call a test function (presumably for debugging or testing purposes)

    heatInfo.tempCelsius = gs_board_class()->thermoSensor()->getThermoSensor();  // Retrieve temperature from the sensor

    // Ensure temperature reading is within a valid range
    if (heatInfo.tempCelsius < -4000 || heatInfo.tempCelsius > 8500) {
        heatInfo.tempCelsius = 0;  // Set temperature to 0 if out of range
    }

    return heatInfo.tempCelsius; // Return the temperature in Celsius
}

// Function to save the alarm status in flash memory
//void saveAlarmStatus(bool isAlarmActive) {
//    MSC_Status_TypeDef mscStatus;
//    uint32_t data = (uint32_t)isAlarmActive;  // Convert the boolean value to uint32_t
//    uint32_t *flashAddress = (uint32_t *)ALARM_STATUS_FLASH_ADDRESS;
//
//    // Disable interrupts to protect the flash memory write operation
//    CORE_CRITICAL_SECTION(
//        MSC_Init();  // Initialize the flash memory controller
//
//        mscStatus = MSC_ErasePage(flashAddress);  // Erase the flash memory page
//        if (mscStatus != mscReturnOk) {
//            MSC_Deinit();  // Deinitialize the flash memory controller if erase fails
//            return;  // Exit the function if page erase fails
//        }
//
//        mscStatus = MSC_WriteWord(flashAddress, &data, NUM_BYTES);  // Write the alarm status to flash memory
//        if (mscStatus != mscReturnOk) {
//            // Add error handling here if necessary
//        }
//
//        MSC_Deinit();  // Deinitialize the flash memory controller
//    );
//}

// Function to load the alarm status from flash memory
//bool loadAlarmStatus(void) {
//    uint32_t *flashAddress = (uint32_t *)ALARM_STATUS_FLASH_ADDRESS;
//    uint32_t storedData;
//
//    // Read the value stored in flash memory
//    storedData = *flashAddress;
//
//    // Check if the read value is valid
//    if (storedData != 0xFFFFFFFF) {
//        // Return the boolean value corresponding to the alarm status
//        return (bool)storedData;
//    } else {
//        // If the value in memory is 0xFFFFFFFF, it may indicate that memory has not been written yet
//        // or that the default value is not an active alarm
//        return false;  // Default value (no alarm)
//    }
//}

// Function to clear the alarm status in flash memory
//void clearAlarmStatus(void) {
//    MSC_Status_TypeDef mscStatus;
//    uint32_t *flashAddress = (uint32_t *)ALARM_STATUS_FLASH_ADDRESS;
//
//    // Disable interrupts to protect the flash memory write operation
//    CORE_CRITICAL_SECTION(
//        MSC_Init();  // Initialize the flash memory controller
//
//        // Erase the memory page where the alarm status was saved
//        mscStatus = MSC_ErasePage(flashAddress);
//        if (mscStatus != mscReturnOk) {
//            MSC_Deinit();  // Deinitialize the flash memory controller if erase fails
//            return;  // Exit the function if page erase fails
//        }
//
//        MSC_Deinit();  // Deinitialize the flash memory controller
//    );
//}

// Function to handle the thermo reset button press
static void gs_cmp_thermo_reset_button(uint32_t timePress) {

    if (timePress > 2500 && timePress < 4500) {
        if (stThermoSensorVi != NULL) {
            stThermoSensorVi->setViState(stThermoSensorVi, enOff);  // Turn off the visual indication
        }

        gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), false);  // Clear the alarm in the IAS Zone Cluster

        enThermoStateMachine = enNormal;  // Set the thermo state machine to normal

        save_alarm_status(false);

        //clearAlarmStatus();  // Clear the alarm status in flash memory
    }
}

// Update function for gs_cmp_thermo_sensor_event_func
static void gs_cmp_thermo_sensor_event_func(sl_zigbee_event_t *event) {

    //static bool alarmActive = false; // Additional control to avoid repeated calls

    uint32_t eventTime = GS_THERMO_EVENT_TIME_MS;

    if (event != NULL) {
        // Check if the alarm should be activated
        if (heatInfo.isAlert) {
            enThermoStateMachine = enAlarm;
            //alarmActive = true;

            if (stThermoSensorVi != NULL) {
                stThermoSensorVi->setViState(stThermoSensorVi, enFire);
            }

            gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), true);
        }
    } else {
        // Check if the current state is alarm before changing to normal
        if (enThermoStateMachine == enAlarm && !heatInfo.isAlert) {
            enThermoStateMachine = enNormal;
            //alarmActive = false;

            if (stThermoSensorVi != NULL) {
                stThermoSensorVi->setViState(stThermoSensorVi, enOff);
            }

            gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), false);
        }
    }

    processHeatDetection();

    // Reschedule the event to continue checking at the defined interval
    sl_zigbee_event_set_delay_ms(&thermoSensorEvent, eventTime);
}

// Initialization function for the thermo sensor component
uint8_t gs_cmp_thermo_sensor_init(void) {

	nvm3_init();

    sl_zigbee_event_init(&thermoSensorEvent, gs_cmp_thermo_sensor_event_func);  // Initialize the Zigbee event
    stThermoSensorVi = gs_board_class()->visualIndication()->new();  // Create a new visual indication instance

    if (stThermoSensorVi == NULL) {
        return EMBER_APPLICATION_ERROR_0;  // Return error if visual indication instance creation fails
    }

    gs_board_class()->resetButton()->registerCb(gs_cmp_thermo_reset_button);  // Register the reset button callback

//    alarmStatus = loadAlarmStatus();  // Load the alarm status from flash memory
//
////     Update the alarm state based on the loaded status
    if (load_alarm_status()) {
        gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), true);  // Set the alarm in the IAS Zone Cluster
        if (stThermoSensorVi != NULL) {
            stThermoSensorVi->setViState(stThermoSensorVi, enFire);  // Set visual indication to fire state
        }
    } else {
        gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), false);  // Clear the alarm in the IAS Zone Cluster
        if (stThermoSensorVi != NULL) {
            stThermoSensorVi->setViState(stThermoSensorVi, enOff);  // Turn off the visual indication
        }
    }

    sl_zigbee_event_set_delay_ms(&thermoSensorEvent, GS_THERMO_EVENT_TIME_MS);  // Set the event delay for the temperature sensor

    return EMBER_SUCCESS;  // Return success
}
