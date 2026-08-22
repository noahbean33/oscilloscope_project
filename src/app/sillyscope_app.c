#include "main.h"
#include "sillyscope_app.h"

#include "channel_mgmt.h"
#include "channel_data.h"

#include "erm19296.h"
#include "industries.h"

SPI_HandleTypeDef *disp_spi = NULL;

void display_spi_tx(uint8_t byte) {
	HAL_SPI_Transmit(disp_spi, &byte, 1, 100);
}

void display_set_cd(bool state) {
	HAL_GPIO_WritePin(DISP_A0_GPIO_Port, DISP_A0_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void display_set_cs(bool state) {
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void display_set_rst(bool state) {
	HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void sillyscope_app() {

	HAL_ADCEx_Calibration_Start(ch_data_adc, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(ch_data_adc, (uint32_t *)ch_data.ch, 2);

	erm19296_set_spi_tx(&display_spi_tx);
	erm19296_set_set_cd(&display_set_cd);
	erm19296_set_set_cs(&display_set_cs);
	erm19296_set_set_rst(&display_set_rst);

	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

	erm19296_write_command(0x30);
	erm19296_write_command(0x94);
	HAL_Delay(50);

	erm19296_init();

	erm19296_draw_bitmap(0, 0, 192, 96, industries, true);
	HAL_Delay(50);

	erm19296_tx_framebuffer();
	//HAL_GPIO_WritePin(LED_REC_GPIO_Port, LED_REC_Pin, GPIO_PIN_SET);

	while (1) {

		if (is_recording && ch_event_flags & CH_EVENT_FLAG_TX) {
			ch_event_flags &= ~CH_EVENT_FLAG_TX;

			ch_data_transmit_callback();
		}

		if (btn_event_flags & BTN_EVENT_FLAG_BTN1) {
			btn_event_flags &= ~BTN_EVENT_FLAG_BTN1;

			ch_handle_event(&ch1, &ch2);
		}

		if (btn_event_flags & BTN_EVENT_FLAG_BTN2) {
			btn_event_flags &= ~BTN_EVENT_FLAG_BTN2;

			ch_handle_event(&ch2, &ch1);
		}

		if (btn_event_flags & BTN_EVENT_FLAG_REC) {
			btn_event_flags &= ~BTN_EVENT_FLAG_REC;

			is_recording = !is_recording;
			HAL_GPIO_WritePin(LED_REC_GPIO_Port, LED_REC_Pin, is_recording ? GPIO_PIN_SET : GPIO_PIN_RESET);
		}
	}
}
