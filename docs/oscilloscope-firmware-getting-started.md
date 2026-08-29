# Firmware Getting Started

## Live MCU pin map (STM32L432KCU6 U1)

| STM32L432 pin | Function | Schematic net |
|---|---|---|
| PA0 / ADC1_IN5 | CH1 ADC, sequence rank 1 | CH1_ADC |
| PA1 / ADC1_IN6 | CH2 ADC, sequence rank 2 | CH2_ADC |
| PA2 / USART2_TX | Target TX to probe RX | VCP_TARGET_TX_TO_PROBE_RX |
| PA3 / USART2_RX | Probe TX to target RX | VCP_PROBE_TX_TO_TARGET_RX |
| PA4 / SPI1_NSS | LCD chip select | LCD_CS |
| PA5 / SPI1_SCK | LCD serial clock | LCD_SCK |
| PA7 / SPI1_MOSI | LCD serial data; module DB3–DB1 tied | LCD_MOSI |
| PA8 / TIM1_CH1 | Backlight PWM, active high | LCD_BL_PWM |
| PA11 / USB_DM | Native USB FS D− | USB_DM |
| PA12 / USB_DP | Native USB FS D+ | USB_DP |
| PA13 | SWDIO | SWDIO |
| PA14 | SWCLK | SWCLK |
| PA15 | USB/activity LED | LED_USB_DRV |
| PB0 | LCD A0/RS data-command | LCD_DC |
| PB1 | LCD reset | LCD_RST |
| PB3 | RUN/STOP button, active low | BTN_RUN_STOP |
| PB4 | MODE button, active low | BTN_MODE |
| PB5 | SELECT button, active low | BTN_SELECT |
| PB6 | RUN LED | LED_RUN_DRV |
| PB7 | TRIGGER/ARM LED | LED_TRIGGER_DRV |

PA6, PA9, PA10, PC14 and PC15 are intentionally unused. PH3/BOOT0 is strapped low through R6.

## Required peripheral setup

- System clock: HSI-based plan; verify the final clock tree and USB clock in STM32CubeMX.
- ADC1: proposed asynchronous clock target 80 MHz; regular sequence IN5 rank 1 then IN6 rank 2; 12.5-cycle sample time; timer-triggered DMA circular operation; target 3.2 Msps aggregate. Timing remains unmeasured.
- USB device: PA11/PA12 native USB FS.
- SPI1 transmit-only: PA5 SCK, PA7 MOSI; GPIO controls PA4 CS, PB0 A0/RS, PB1 reset.
- TIM1_CH1: PA8 PWM. Logic high commands backlight ON.
- USART2: PA2 TX and PA3 RX, with the direction-explicit STDC14 crossover nets.
- Buttons: GPIO inputs with active-low interpretation; external 10 kΩ pull-ups and 100 Ω/100 nF filters are fitted.
- LEDs: PB6, PB7 and PA15 are active-high outputs; the power LED is rail-driven.

## Startup order

1. Configure power/flash latency and HSI clock tree.
2. Initialize GPIO default states: LCD CS high, LCD reset high after reset pulse, PA8 low, status LEDs off.
3. Start USB device and USART2 VCP test path.
4. Initialize SPI1 and the ERM19296-1 in 4-wire serial mode.
5. Calibrate and configure ADC1, timer trigger and circular DMA.
6. Enable button processing and acquisition state machine.

## STM32 HAL implementation skeleton

Generate the clock, GPIO, ADC1, DMA, TIM trigger, SPI1, USART2, and USB-device setup with STM32CubeMX for the **STM32L432KCU6** package, then keep application code within generated user sections. The following calls are real STM32 HAL APIs; they are an implementation example, not tested project firmware:

```c
static void Scope_StartAcquisition(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_TIM_Base_Start(&htim6); /* Use the timer selected as ADC external trigger. */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_interleaved,
                      ADC_INTERLEAVED_SAMPLE_COUNT);
}

static void Backlight_Set(uint16_t compare)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, compare);
}

static void Lcd_Write(const uint8_t *data, uint16_t length)
{
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)data, length, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

static void ProbeUart_Send(const uint8_t *data, uint16_t length)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)data, length, HAL_MAX_DELAY);
}
```

ADC channel configuration must explicitly use `ADC_CHANNEL_5` as rank 1 and `ADC_CHANNEL_6` as rank 2, both with `ADC_SAMPLETIME_12CYCLES_5`, and an external timer trigger selected in CubeMX/HAL. Verify generated register settings and actual sample timing before relying on the target rate.

Toolchain: STM32CubeIDE with the STM32CubeL4 HAL package. Build and flash with the generated CubeIDE project or equivalent `arm-none-eabi-gcc` flow; no repository-specific build command is documented or claimed tested here.

## Bring-up notes

- Confirm ADC rank order by register readback before interpreting interleaved samples.
- Keep the backlight off before USB configuration.
- Bandwidth, divider compensation, 15 V overload survival, backlight operating point, USB ESD/hot-plug, ADC timing, thermals, and HSI accuracy are prototype verification gates, not firmware-assumed verified performance.
