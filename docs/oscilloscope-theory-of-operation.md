# Theory of Operation

## Design and claim boundary

This file describes the live **101-component, 57-net STM32L432KCU6 internal-ADC MVP** as of 2026-08-28. Implemented circuitry, configured targets, and modeled results must not be described as measured, tested, certified, or production-proven without evidence.

## Analog signal paths

Each channel is DC-coupled and unipolar:

`BNC → 3 × 300 kΩ high arm → CHx_CLAMP with 200 kΩ low arm and compensation → BAV199LT1G rail clamp → 10 kΩ → TLV9062IDGKR unity buffer → 47 Ω / 1 nF ADC network → STM32 ADC`

- CH1 terminates at PA0 / ADC1_IN5.
- CH2 terminates at PA1 / ADC1_IN6.
- R12 and R18 are **47 Ω Yageo RC0603FR-0747RL**.
- D3/D4 BAV199 mapping is pin 1 to AGND, pin 3 to CHx_CLAMP, and pin 2 to VDDA.
- Nominal low-frequency input impedance is 1.10 MΩ and nominal divider ratio is 5.5:1.

The normal measurement range is 0–12 V DC. Fifteen volts is an **unmeasured overload-survival target**, not a normal rating or safety certification. Inputs are ground referenced and non-isolated; negative, mains, CAT, floating, and high-energy measurements are prohibited.

## Acquisition architecture

The proposed firmware configuration uses ADC1 rank 1 = IN5/CH1 and rank 2 = IN6/CH2, an 80 MHz asynchronous ADC clock target, 12.5-cycle sample time, timer-triggered DMA, and 3.2 Msps aggregate target. Calculated target values are 312.5 ns rank spacing and 1.6 Msps/channel. ADC timing, settling, crosstalk, and rate remain unmeasured.

The implemented 47 Ω/1 nF network passed a bounded acquisition model at 0.558 LSB worst-case external error. That is a simulation result, not a hardware measurement.

## LCD, controls, and backlight

J4 is the SSW-120-01-T-S 1×20 interface for the exact ERM19296-1 4-wire SPI pin-header option. U1 uses PA4 for LCD_CS, PA5 for LCD_SCK, PA7 for LCD_MOSI, PB0 for LCD_DC, and PB1 for LCD_RST.

The backlight is switched from VBUS_PROT by fitted **NCE3401 Q1** with 2N7002KT1G Q2 as the gate pull-down. PA8/TIM1_CH1 drives the control path; PWM high commands backlight ON. R19 is a provisional 27 Ω Yageo RC2010FK-0727RL. LCD pin 6 remains continuously grounded. Backlight current and thermal performance remain unmeasured.

Buttons are active-low on PB3/PB4/PB5. Status LED drive nets use PB6, PB7, and PA15. USART2 uses PA2 TX on `VCP_TARGET_TX_TO_PROBE_RX` and PA3 RX on `VCP_PROBE_TX_TO_TARGET_RX`.

## USB and power

USB FS uses PA11/USB_DM and PA12/USB_DP through USBLC6-2SC6. The final USB_DP repair uses a bottom-layer bypass with explicit top/bottom transition vias. The configured routing target is **45 Ω single-ended / 90 Ω differential**; this is a fabrication target, not measured or certified impedance.

USB VBUS passes through MF-PSMF035X-2 protection to VBUS_PROT. AP7361C-33Y5-13 generates 3.3 V; FB1 filters VDDA. LDO input current is **output current plus quiescent current**. Direct-VBUS loads such as the backlight and protection leakage are added separately. Bounded totals including 80 µA LDO IQ and 20 µA TVS leakage are 125.10 mA typical, 145.10 mA design, and 175.10 mA validation case.

## PCB implementation state

The PCB is 100 × 100 mm with four copper layers and a nominal 1.30 mm custom stackup. All 101 components are top-side. Routing is complete with zero airwires and zero DRC errors.

Two locked legacy protected dangling-trace warnings remain: `Trace a4ae` and `Trace b74f`. They require Gerber review and cleanup or an explicit waiver.

Fourteen fabricated 1.0 × 1.0 mm test pads are currently BOM elements and must be excluded from BOM and pick-and-place before SMT assembly export. TP_GND is a real Keystone 4952 component and remains included. The live DNI exclusion set is C16, C17, C23, and C24.

## Unmeasured prototype gates

Bandwidth, divider compensation, 15 V overload survival, backlight operating point, USB ESD/hot-plug behavior, ADC timing, thermals, and HSI accuracy remain open prototype validation gates.
