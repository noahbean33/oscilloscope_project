#include "channel_mgmt.h"
#include "main.h"

#define BTN_DEBOUNCE_PERIOD_MS 300

uint64_t btn_ch1_last_tick = 0;
uint64_t btn_ch2_last_tick = 0;
uint64_t btn_rec_last_tick = 0;

volatile bool is_recording = false;
volatile uint32_t btn_event_flags = 0x00;

channel_s ch1 = {
	.state = CHANNEL_STATE_OFF,
	.led_port = LED_CH1_GPIO_Port,
	.led_pin = LED_CH1_Pin,
	.btn_port = BTN_CH1_GPIO_Port,
	.btn_pin = BTN_CH1_Pin
};

channel_s ch2 = {
	.state = CHANNEL_STATE_OFF,
	.led_port = LED_CH2_GPIO_Port,
	.led_pin = LED_CH2_Pin,
	.btn_port = BTN_CH2_GPIO_Port,
	.btn_pin = BTN_CH2_Pin
};

void rec_button_callback() {
	uint64_t curr_tick = HAL_GetTick();
	if (curr_tick - btn_rec_last_tick < BTN_DEBOUNCE_PERIOD_MS) return;
	btn_event_flags |= BTN_EVENT_FLAG_REC;
	btn_rec_last_tick = curr_tick;
}

void ch_mgmt_button1_callback() {
	uint64_t curr_tick = HAL_GetTick();
	if (curr_tick - btn_ch1_last_tick < BTN_DEBOUNCE_PERIOD_MS) return;
	btn_event_flags |= BTN_EVENT_FLAG_BTN1;
	btn_ch1_last_tick = curr_tick;
}

void ch_mgmt_button2_callback() {
	uint64_t curr_tick = HAL_GetTick();
	if (curr_tick - btn_ch2_last_tick < BTN_DEBOUNCE_PERIOD_MS) return;
	btn_event_flags |= BTN_EVENT_FLAG_BTN2;
	btn_ch2_last_tick = curr_tick;
}

void ch_handle_event(channel_s *target, channel_s *other) {
	if (target->state == CHANNEL_STATE_ON) {
		target->state = CHANNEL_STATE_OFF;
		HAL_GPIO_WritePin(target->led_port, target->led_pin, GPIO_PIN_RESET);
	} else {
		if (other->state != CHANNEL_STATE_OFF) {
			other->state = CHANNEL_STATE_OFF;
			HAL_GPIO_WritePin(other->led_port, other->led_pin, GPIO_PIN_RESET);
		}

		target->state = CHANNEL_STATE_ON;
		HAL_GPIO_WritePin(target->led_port, target->led_pin, GPIO_PIN_SET);
	}
}
