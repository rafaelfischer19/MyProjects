/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file    rf_cmp_smoke_detector.c
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

#include "af.h"
#include "zigbee_app_framework_event.h"
#include "ember-types.h"
#include "error-def.h"
#include "mac-types.h"
/* ---------------------- Stack include --------------------------------------*/

/* ---------------------- Others include -------------------------------------*/
#include "../../zigbee_platform/zp_cluster_interface/gs_zp_ias_zone_cluster_server.h"
#include "../../util/gs_board.h"
#include "gs_cmp_smoke_detector.h"
#include "../../util/gs_custom_tokens.h"
#include "sl_token_api.h"

/******************************************************************************
 *                          DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/

#define GS_SMOKE_EVENT_TIME_MS 4000
#define GS_SMOKE_SET_POINT     200
#define GS_SMOKE_INIT_DIFF     800

/* --------------------- Private typedef -------------------------------------*/

typedef enum {
    enNormal,
    enPreDetection,
    enAlarm,
    enWaitReset
} en_smoke_state_machine_t;

/* --------------------- Private variables -----------------------------------*/

static sl_zigbee_event_t smokeDetectorEvent;
static en_smoke_state_machine_t enSmokeStateMachine = enNormal;
static st_vi_instance_t *stSmokeVi = NULL;

/* --------------------- Private function prototypes -------------------------*/

static void gs_cmp_smoke_detector_event_func(sl_zigbee_event_t *event);
static void gs_cmp_smoke_reset_button(uint32_t timePress);
static uint8_t load_alarm_status(void);
static void save_alarm_status(bool alarmActive);

/******************************************************************************
 *                          FUNCTIONS
 *****************************************************************************/

static uint8_t load_alarm_status(void)
{
    bool alarmActive;
    halCommonGetToken(&alarmActive, TOKEN_ALARM_STATUS);
    return alarmActive;
}

static void save_alarm_status(bool alarmActive)
{
    halCommonSetToken(TOKEN_ALARM_STATUS, &alarmActive);
}

static void gs_cmp_smoke_detector_event_func(sl_zigbee_event_t *event)
{
    static uint8_t smokeAlarmCount = 0;

    uint32_t eventTime = GS_SMOKE_EVENT_TIME_MS;
    st_hal_smoke_data_t stSmokeSample;

    gs_board_class()->smokeSample()->getSample(&stSmokeSample);
    uint16_t smokeDiff = stSmokeSample.ledOn - stSmokeSample.ledOff;

    switch (enSmokeStateMachine) {
        case enNormal:
            if (smokeDiff > (GS_SMOKE_INIT_DIFF + GS_SMOKE_SET_POINT)) {
                smokeAlarmCount++;
                enSmokeStateMachine = enPreDetection;
                eventTime = GS_SMOKE_EVENT_TIME_MS >> smokeAlarmCount;
            }
            break;
        case enPreDetection:
            if (smokeDiff > (GS_SMOKE_INIT_DIFF + GS_SMOKE_SET_POINT)) {
                smokeAlarmCount++;
                eventTime = GS_SMOKE_EVENT_TIME_MS >> smokeAlarmCount;

                if (smokeAlarmCount >= 3) {
                    enSmokeStateMachine = enAlarm;
                    stSmokeVi->setViState(stSmokeVi, enFire);
                    gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), true);
                    save_alarm_status(true);
                }
            } else {
                enSmokeStateMachine = enNormal;
                smokeAlarmCount = 0;
            }
            break;
        case enWaitReset:
            break;
        default:
            break;
    }
    sl_zigbee_event_set_delay_ms(&smokeDetectorEvent, eventTime);
}

static void gs_cmp_smoke_reset_button(uint32_t timePress)
{
    sl_zigbee_app_debug_println("Button pressed, time: %d ms\n", timePress);

    if ((timePress > 2500 && timePress < 4500)) {
        sl_zigbee_app_debug_println("Reset conditions met, resetting...\n");

        stSmokeVi->setViState(stSmokeVi, enOff);
        gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), false);
        enSmokeStateMachine = enNormal;
        save_alarm_status(false);
    } else {
        sl_zigbee_app_debug_println("Reset conditions not met.\n");
    }
}

uint8_t gs_cmp_smoke_detector_init(void)
{
    sl_zigbee_event_init(&smokeDetectorEvent, gs_cmp_smoke_detector_event_func);

    stSmokeVi = gs_board_class()->visualIndication()->new();

    if (stSmokeVi == NULL) {
        return EMBER_APPLICATION_ERROR_0;
    }

    gs_board_class()->resetButton()->registerCb(gs_cmp_smoke_reset_button);

    if (load_alarm_status()) {
        enSmokeStateMachine = enAlarm;
        stSmokeVi->setViState(stSmokeVi, enFire);
        gs_zp_ias_zone_cluster_server_class()->setAlarm1(emberAfPrimaryEndpoint(), true);
    } else {
        enSmokeStateMachine = enNormal;
    }

    sl_zigbee_event_set_delay_ms(&smokeDetectorEvent, GS_SMOKE_EVENT_TIME_MS);

    return EMBER_SUCCESS;
}
