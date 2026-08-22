# Theory of Operation

## Revision boundary

Rev 0.1A is the existing reviewed design. Rev 0.2 is **approved for schematic capture but not yet implemented or tested**. Detailed controls are in [MVP Design Decisions](https://www.flux.ai/nbean/oscilloscope/files/mvp-design-decisions).

## Rev 0.2 signal path

Each channel is a DC-coupled unipolar path: **1.10 MΩ low-frequency nominal, compensated 5.5:1 divider → CHx_CLAMP → BAV199LT1G rail clamp → 10 kΩ → TLV9062IDR unity buffer → 330 Ω/1 nF ADC filter → MCU ADC input**.

- CH1 terminates at **PA0 / ADC1_IN5**.
- CH2 terminates at **PA1 / ADC1_IN6**.
- ADC regular sequence is CH1 rank 1 followed by CH2 rank 2.

For each BAV199LT1G, pin 1 goes to AGND, pin 3 goes to CHx_CLAMP, and pin 2 goes to VDDA. This uses the lower diode from AGND to CHx_CLAMP and the upper diode from CHx_CLAMP to VDDA.

Normal rated measurement is **0–12 V DC**. The 15 V figure is only an **unverified overload-survival design target**, not a normal rating, bench result, or safety certification.

## Acquisition timing

The configuration target is an **80 MHz asynchronous ADC clock**, timer-triggered **3.2 Msps aggregate**, and **12.5-cycle sampling + 12.5-cycle conversion** per rank. This produces target values of **312.5 ns CH1-to-CH2 skew** and **1.6 Msps/channel** for the alternating sequence. These are firmware/bench confirmation targets, not measured facts.

Triggering remains software-based with selectable channel/edge/level, Auto/Normal/Single modes, and circular pretrigger buffering. HSI timebase accuracy after USB-SOF/factory correction remains a prototype verification gate.

## VCP

The UART crossover is explicit:

- `VCP_TARGET_TX_TO_PROBE_RX`: MCU TX → STDC14 probe RX.
- `VCP_PROBE_TX_TO_TARGET_RX`: STDC14 probe TX → MCU RX.

## LCD and backlight

Procure only the exact **ERM19296-1 “4-Wire SPI Pin Header Connection”** option. It uses a 1×20, 2.54 mm header. Four-wire SPI uses A0/RS, CSB, RSTB, DB3–DB1 tied as MOSI/SDA, and DB0 as SCK/SCL; DB7–DB4, ERD, and RWR are tied high, with IF2:IF1:IF0 = L:L:L.

The backlight is PWM-switched on the high side so LCD pin 6 remains a solid common ground. Protected VBUS feeds the source of an AO3401A P-MOS; its drain feeds the provisional 27 Ω, 0.5 W resistor and then LEDA pin 5. A 100 kΩ source-to-gate pull-up defaults the P-MOS OFF. A 2N7002KT1G pulls the P-MOS gate low; its source is grounded and its MCU-driven gate has 100 Ω series resistance and a 100 kΩ ground pulldown. **MCU PWM high turns the backlight ON.** The AO3401A −30 V VDS, ±12 V VGS and 60 mΩ maximum RDS(on) at −4.5 V are suitable at 5 V and 60–75 mA; the 2N7002KT1G 60 V VDS, ±20 V VGS and ≥230 mA stated continuous-current capability provide ample driver margin. The 27 Ω value remains provisional pending exact module Vf/current measurement and resistor/MOSFET thermal verification at the 60–75 mA acceptance band.

## Power and grounding

LDO input current is approximately LDO output current, while the backlight is a separate direct-VBUS load. Rounded USB totals are therefore 50+75 = **125 mA typical**, 70+75 = **145 mA design**, and 100+75 = **175 mA validation case**.

BNC shells, PCB ground, and USB ground are common. The instrument is non-isolated and limited to ground-referenced, current-limited extra-low-voltage sources.

## Prototype verification gates

Bandwidth/flatness, ADC settling and crosstalk, compensation trim, HSI accuracy, 15 V overload survival, thermal performance, hot-plug behavior, and ESD robustness are prototype verification gates rather than implemented or measured performance claims.