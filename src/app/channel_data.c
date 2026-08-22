#include "channel_data.h"
#include "usbd_cdc_if.h"

static uint8_t rx_buffer[4096];

ADC_HandleTypeDef *ch_data_adc = NULL;
CRC_HandleTypeDef *ch_data_crc = NULL;
UART_HandleTypeDef *ch_data_uart = NULL;

volatile uint32_t ch_event_flags = 0x00;

packed_ch_data_s ch_data = {
	.msg = DEVICE_CH_DATA,
	.len = sizeof(packed_ch_data_s),
	.crc = 0
};

packed_msg_s ch_msg = {
	.msg = 0,
	.len = sizeof(packed_msg_s),
	.crc = 0,
};

void ch_data_timer_update_callback() {
	ch_event_flags |= CH_EVENT_FLAG_TX;
}

void ch_data_transmit_callback() {
	ch_data.crc = 0;
	ch_data.crc = HAL_CRC_Calculate(ch_data_crc, (uint32_t *)&ch_data, sizeof(ch_data));

	HAL_UART_Transmit(ch_data_uart, (uint8_t *)&ch_data, sizeof(ch_data), 100);
	//CDC_Transmit_FS((uint8_t *)&ch_data, sizeof(ch_data));
}

void handle_usb_packet() {
	memcpy(rx_buffer, UserRxBufferFS, APP_RX_DATA_SIZE);

	uint16_t len = (uint16_t)rx_buffer[1] << 8 | rx_buffer[2];
	uint32_t rx_crc = (uint16_t)rx_buffer[len - 1] << 8 | rx_buffer[len];

	rx_buffer[len - 1] = 0;
	rx_buffer[len] = 0;

	uint32_t calc_crc = HAL_CRC_Calculate(ch_data_crc,
		(uint32_t *)&rx_buffer, sizeof(uint8_t) * len);

	if (rx_crc != calc_crc) {
		ch_msg.msg = DEVICE_ERR_BAD_CRC;
		ch_msg.crc = 0;

		ch_msg.crc = HAL_CRC_Calculate(ch_data_crc,
			(uint32_t *)&ch_msg, sizeof(ch_msg));

		CDC_Transmit_FS((uint8_t *)&ch_msg, sizeof(ch_msg));
		return;
	}

	switch (rx_buffer[0]) {
		case DEVICE_CMD_GET_SETTINGS: break;
		case DEVICE_CMD_SET_SETTINGS: break;
		default: break;
	}
}
