#ifndef CHANNEL_DATA_H
#define CHANNEL_DATA_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define CH_EVENT_FLAG_TX 0x01

extern ADC_HandleTypeDef *ch_data_adc;
extern CRC_HandleTypeDef *ch_data_crc;
extern UART_HandleTypeDef *ch_data_uart;

extern volatile uint32_t ch_event_flags;

typedef enum {

	DEVICE_CH_DATA = 1,
	DEVICE_CMD_GET_SETTINGS = 2,
	DEVICE_CMD_SET_SETTINGS = 3,
	DEVICE_CMD_START_LOGGING = 4,
	DEVICE_ERR_BAD_CRC = 5
} device_msg_e;

typedef struct { // sent from PC -> device
	uint16_t msg;
	uint16_t len;

	uint32_t crc;
} packed_msg_s;

typedef struct { // sent from device -> PC
	uint16_t msg;
	uint16_t len;

	uint16_t ch[4];

	uint32_t crc;
} packed_ch_data_s;

typedef struct { // sent from device -> PC
	uint16_t msg;
	uint16_t len;

	uint16_t chflags;

	uint32_t crc;
} packed_settings_data_s;

extern packed_ch_data_s ch_data;

void ch_data_timer_update_callback();
void ch_data_transmit_callback();
#endif
