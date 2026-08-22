#ifndef CHANNEL_MGMT_H
#define CHANNEL_MGMT_H

#include <stdint.h>
#include <stdbool.h>

#include "main.h"

#define BTN_EVENT_FLAG_BTN1 0x01
#define BTN_EVENT_FLAG_BTN2 0x02
#define BTN_EVENT_FLAG_REC  0x04

typedef enum {

	CHANNEL_STATE_OFF,
	CHANNEL_STATE_ON,
	CHANNEL_STATE_REC
} channel_state_e;

typedef struct {
	channel_state_e state;

	uint16_t led_pin;
	GPIO_TypeDef *led_port;

	uint16_t btn_pin;
	GPIO_TypeDef *btn_port;
} channel_s;

extern channel_s ch1;
extern channel_s ch2;

extern uint64_t btn_ch1_last_ms;
extern uint64_t btn_ch2_last_ms;
extern uint64_t btn_rec_last_ms;

extern volatile uint32_t btn_event_flags;

extern volatile bool is_recording;

void ch_mgmt_button1_callback();
void ch_mgmt_button2_callback();
void ch_handle_event(channel_s *target, channel_s *other);
#endif
