#include "erm19296.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define CONTRAST 202

uint8_t ERM19296Framebuffer[ERM19296_FB_SIZE] = {0};

void erm19296_dummy_tx(uint8_t byte) {;}
void erm19296_dummy_cd(bool state) {;}
void erm19296_dummy_cs(bool state) {;}
void erm19296_dummy_rst(bool state) {;}

void (*erm19296_spi_tx)(uint8_t byte) = &erm19296_dummy_tx;
void (*erm19296_set_cd)(bool state) = &erm19296_dummy_cd;
void (*erm19296_set_cs)(bool state) = &erm19296_dummy_cs;
void (*erm19296_set_rst)(bool state) = &erm19296_dummy_rst;

void erm19296_set_spi_tx(void (*spi_tx)(uint8_t byte)) {

	erm19296_spi_tx = spi_tx;
}

void erm19296_set_set_cd(void (*set_cd)(bool state)) {

	erm19296_set_cd = set_cd;
}

void erm19296_set_set_cs(void (*set_cs)(bool state)) {

	erm19296_set_cs = set_cs;
}

void erm19296_set_set_rst(void (*set_rst)(bool state)) {

	erm19296_set_rst = set_rst;
}

void erm19296_write_command(uint8_t cmd) {
	erm19296_set_cs(false);
	erm19296_set_cd(false);
	erm19296_spi_tx(cmd);
	erm19296_set_cs(true);
}

void erm19296_write_one_data(uint8_t data) {
	erm19296_set_cs(false);
	erm19296_set_cd(true);
	erm19296_spi_tx(data);
	erm19296_set_cs(true);
}

void erm19296_write_data(uint8_t *data, int len) {

	for (int i = 0; i < len; i++) {
		erm19296_set_cs(false);
		erm19296_set_cd(true);
		erm19296_spi_tx(data[i]);
		erm19296_set_cs(true);
	}
}

void erm19296_set_pixel(int x, int y, bool state) {

	int pos = x + (y / 8) * ERM19296_WIDTH;

	ERM19296Framebuffer[pos] = state
		? ERM19296Framebuffer[pos] & ~(1 << (y % 8))
		: ERM19296Framebuffer[pos] | 1 << (y % 8);
}

void erm19296_draw_bitmap(int x, int y, int width, int height, const uint8_t *image, bool invert) {

	uint8_t chkMsk = 0x00;
	int size = width*height >> 3;

	int origX = x;
	int origY = y;

	bool state;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 8; j++) {

			chkMsk = 0x80 >> j;

			state = chkMsk & image[i];
			state = invert ? !state : state;

			erm19296_set_pixel(x, y, state);
			x++;

			if ((x - origX) == width) {

				y++;
				x = origX;
			}

			if ((y - origY) == height) {
				return;
			}

			if (y > ERM19296_HEIGHT) {
				return;
			}
		}
	}
}

void erm19296_tx_framebuffer() {
	erm19296_write_command(0xF0);
	erm19296_write_one_data(0x10);

	erm19296_write_command(0x15);
	erm19296_write_one_data(0x00);
	erm19296_write_one_data(0xBF);

	erm19296_write_command(0x75);
	erm19296_write_one_data(0x00);
	erm19296_write_one_data(0x0B);

	erm19296_write_command(0x5C);
	//erm19296_write_data(ERM19296Framebuffer, ERM19296_FB_SIZE);
	for (int page = 0; page < ERM19296_HEIGHT / 8; page++) {
		for(int i = 0; i < ERM19296_WIDTH; i++) {
			erm19296_write_one_data(ERM19296Framebuffer[i + (page * ERM19296_WIDTH)]);
		}
	}
}

void erm19296_init() {

	erm19296_write_command(0x31);
	erm19296_write_command(0x32);

	erm19296_write_one_data(0x00);
	erm19296_write_one_data(0x01);
	erm19296_write_one_data(0x03);

	erm19296_write_command(0x51);
	erm19296_write_one_data(0xFA);

	erm19296_write_command(0x30);
	erm19296_write_command(0x20);
	erm19296_write_one_data(0x0B);

	erm19296_write_command(0x81);
	erm19296_write_one_data(CONTRAST & 0x3F);
	erm19296_write_one_data((CONTRAST >> 6) & 0x07);

	erm19296_write_command(0x0C);
	erm19296_write_command(0xF0);
	erm19296_write_one_data(0x10);

	erm19296_write_command(0xCA);
	erm19296_write_one_data(0x00);
	erm19296_write_one_data(0x5F);
	erm19296_write_one_data(0x00);

	erm19296_write_command(0xBC);
	erm19296_write_one_data(0x00);

	erm19296_write_command(0xAF);
}
