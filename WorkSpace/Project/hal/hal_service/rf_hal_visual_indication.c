/**
 ******************************************************************************
 * @section company_section Company
 * 
 *
 * @file	rf_hal_visual_indication.c
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
#include PLATFORM_HEADER
#include "error-def.h"
#include "sl_malloc.h"

/* ---------------------- Others include -------------------------------------*/
#include "../../cli/gs_cli_interface.h"
#include "../hal_common/gs_hal_gpio.h"
#include "gs_hal_visual_indication.h"


/******************************************************************************
 *							DEFINES
 *****************************************************************************/
/* --------------------- Private macros --------------------------------------*/
/* --------------------- Private typedef -------------------------------------*/
typedef struct st_visual_indication_control{
		st_vi_instance_t stInstance;
		en_vi_state_t enState;
		struct st_visual_indication_control *next;
}st_visual_indication_control_t;

typedef struct {
	en_vi_state_t ledState;
	int8u percent;
	int32u period;
	int32u interactions;
}st_vi_led_control_t;

/* --------------------- Private variables -----------------------------------*/
extern sl_cli_command_group_t gs_cli_hal_vi_main_group;

static st_vi_led_control_t viLedControl[] = {
	{enButtonAction,10,1000,0},
	{enScanNetwork,10,200,0},
	{enFire,100,GS_MAX_PERIOD,0},
	{enFault,50,1000,0},
	{enOperation,1,4000,0},
	{enOff,0,GS_MAX_PERIOD,0}
};

static uint8_t viPort_ = 0;
static uint8_t viPin_ = 0;
static uint8_t viLedOffState_ = 0;

static en_vi_state_t enViActualState = enOff;

static st_visual_indication_control_t *stViList = NULL;

/* --------------------- Private function prototypes -------------------------*/
static uint8_t gs_hal_visual_indication_init(uint8_t viPort, uint8_t viPin,
												uint8_t viLedOffState,
												bool isEnableCli);
static st_vi_instance_t* gs_hal_visual_indication_new(void);

/* --------------------- Public objects --------------------------------------*/
static const st_hal_visual_indication_class_t halVisualIndicationClass = {
	.init = &gs_hal_visual_indication_init,
	.new = &gs_hal_visual_indication_new
};


/******************************************************************************
 *							FUNCTIONS
 *****************************************************************************/

const st_hal_visual_indication_class_t* gs_hal_visual_indication_class(void)
{
	return &halVisualIndicationClass;
}

static uint8_t gs_hal_vi_update(void)
{
	st_visual_indication_control_t *current = stViList;
	en_vi_state_t state = enOff;

	while (current != NULL) {
		if (current->enState < state) {
			state = current->enState;
		}
		current = current->next;
	}

	if (state != enViActualState) {
		enViActualState = state;

		return gs_hal_gpio_class()->setOutPut(viPort_, viPin_,
										viLedControl[state].percent,
										viLedControl[state].period,
										viLedControl[state].interactions,
										viLedOffState_, false);
	}
	return EMBER_APPLICATION_ERROR_0;
}

static uint8_t gs_hal_set_visual_indication_state(const void *pContext,
													en_vi_state_t enState)
{
	st_vi_instance_t *instance = (st_vi_instance_t*) pContext;

	if (!instance) {
		return EMBER_APPLICATION_ERROR_0;
	}

	st_visual_indication_control_t *current = stViList;

	while (current != NULL) {
		if (&current->stInstance == instance) {
			if (current->enState != enState) {
				current->enState = enState;

				return gs_hal_vi_update();
			}
			break;
		}
		current = current->next;
	}
	return EMBER_APPLICATION_ERROR_0;
}

static st_vi_instance_t* gs_hal_visual_indication_new(void)
{
	st_visual_indication_control_t *new =
			(st_visual_indication_control_t*) sl_malloc(
					sizeof(st_visual_indication_control_t));

	if (new == NULL) {
		return NULL;
	}

	new->enState = enOff;
	new->stInstance.setViState = &gs_hal_set_visual_indication_state;
	new->next = NULL;

	if (stViList == NULL) {
		stViList = new;
	} else {
		st_visual_indication_control_t *current = stViList;

		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new;
	}
	return &new->stInstance;
}

static uint8_t gs_hal_visual_indication_init(uint8_t viPort, uint8_t viPin,
												uint8_t viLedOffState,
												bool isEnableCli)
{
	viPort_ = viPort;
	viPin_ = viPin;
	viLedOffState_ = viLedOffState;

	uint8_t status = gs_hal_gpio_class()->setOut(viPort_, viPin_,
													viLedOffState_);

	if (isEnableCli) {
		gs_cli_interface_class()->addGroup(&gs_cli_hal_vi_main_group);
	}

	return status;
}














