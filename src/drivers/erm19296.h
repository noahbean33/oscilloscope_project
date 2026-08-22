#ifndef ERM19296_H
#define ERM19296_H

#include <stdint.h>
#include <stdbool.h>

#define ERM19296_WIDTH 192
#define ERM19296_HEIGHT 96

#define ERM19296_FB_SIZE (ERM19296_WIDTH * ERM19296_HEIGHT / 4)

void erm19296_set_spi_tx(void (*spi_tx)(uint8_t byte));
void erm19296_set_set_cd(void (*set_cd)(bool state));
void erm19296_set_set_cs(void (*set_cs)(bool state));
void erm19296_set_set_rst(void (*set_rst)(bool state));

void erm19296_write_command(uint8_t cmd);
void erm19296_write_one_data(uint8_t data);

void erm19296_tx_framebuffer();
void erm19296_draw_bitmap(int x, int y, int width, int height, const uint8_t *image, bool invert);
void erm19296_init();
#endif
