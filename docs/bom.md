# Bill of Materials

## Revision boundary

This is the Rev 0.2 delta approved for schematic capture. It is not yet implemented or tested.

## Retained principal parts

- STM32L432KCU6, UFQFPN32.
- AP7361C-33Y5-13 3.3 V LDO.
- Exact **ERM19296-1, “4-Wire SPI Pin Header Connection”** procurement option.
- Existing USB-C receptacle, two BNCs, STDC14, buttons, and LEDs.

## Locked Rev 0.2 additions/changes

| Block | Exact part/value or approved requirement | Qty | Note |
|---|---|---:|---|
| Dual input buffer | TLV9062IDR, TI, SOIC-8 | 1 | One channel per input |
| High divider | 300 kΩ, 0.1%, ≤25 ppm/°C, 1206, ≥100 V working, pulse-rated | 6 | Three in series per channel; combined arm remains ±0.1% |
| Low divider | 200 kΩ, 0.1%, ≤25 ppm/°C, 1206 | 2 | One per channel |
| High compensation | 12 pF C0G, ≥50 V | 6 | Across each 300 kΩ |
| Low compensation | 8.2 pF C0G fitted; 2.2 pF and 4.7 pF C0G DNI | 2 fitted + 4 DNI | Prototype trim gate |
| Rail clamps | BAV199LT1G, onsemi, SOT-23 | 2 | Pin 1→AGND; pin 3→CHx_CLAMP; pin 2→VDDA; both diodes used |
| Clamp-to-buffer | 10 kΩ, 1% | 2 | CHx_CLAMP to TLV9062 non-inverting input |
| ADC isolation/filter | 330 Ω, 1% + 1.0 nF C0G | 2 each | To PA0/ADC1_IN5 and PA1/ADC1_IN6 |
| Op-amp decoupling | 100 nF X7R | 1 | Close to TLV9062 supply |
| USB data ESD | USBLC6-2SC6 | 1 | Prototype ESD performance gate |
| VBUS PPTC | MF-PSMF035X-2 | 1 | 0.35 A hold |
| VBUS TVS | SMAJ5.0A-TR | 1 | Hot-plug behavior remains a gate |
| VBUS input capacitor | 4.7 µF X7R, ≥16 V | 1 replacement | Account for DC bias |
| Backlight high-side MOSFET | AO3401A, Alpha & Omega Semiconductor, P-channel, SOT-23 | 1 | Source to protected VBUS; drain to backlight resistor; −30 V VDS, ±12 V VGS, ≤60 mΩ at VGS = −4.5 V |
| Backlight gate-pull N-MOS | 2N7002KT1G, onsemi, SOT-23 | 1 | Drain pulls AO3401A gate low; source to GND; 60 V VDS, ±20 V VGS |
| Backlight resistor | 27 Ω, 0.5 W initial prototype value | 1 | Select final value for 60–75 mA; value is not architecture-locked |
| P-MOS gate pull-up | 100 kΩ | 1 | AO3401A gate to protected VBUS; default OFF |
| N-MOS gate parts | 100 Ω series, 100 kΩ pulldown | 1 each | MCU PWM through 100 Ω; 2N7002 gate to GND through 100 kΩ; PWM high = ON |
| NRST capacitor | 100 nF X7R | 1 | — |
| VDDA ferrite | BLM21PG221SN1D-class | 1 | — |
| VDDA/VREF capacitors | VDDA 100 nF + 1 µF; VREF+ 10 nF + 1 µF | as required | — |
| Button RC | 100 Ω series + 100 nF to ground | 3 each | — |

## LCD connector/interface capture

The exact 4-wire SPI module option uses a **1×20, 2.54 mm-pitch pin header**. Capture: pins 1–4 optional font-chip interface (NC unless ordered/used); pin 5 LEDA; pin 6 VSS, also the documented backlight return; pin 7 VDD; pin 8 A0/RS; pin 9 RSTB; pin 10 CSB; pins 11–14 DB7–DB4 tied to VDD; pins 15–17 DB3–DB1 tied together as MOSI/SDA; pin 18 DB0 as SCK/SCL; pins 19–20 ERD/RWR tied to VDD. Ordered interface selection is IF2:IF1:IF0 = L:L:L. No separate LEDK connector pin is documented.

## Removed items

- **Remove both TS-107-T-AA headers.** They are not present in the Rev 0.2 BOM or schematic.
- Remove the invalid AO3400A low-side backlight switch and any switched-return/LEDK net. Retain LCD pin 6 as solid ground.

## Net labels affecting capture

- CH1 ADC: PA0 / ADC1_IN5, rank 1.
- CH2 ADC: PA1 / ADC1_IN6, rank 2.
- VCP: `VCP_TARGET_TX_TO_PROBE_RX` and `VCP_PROBE_TX_TO_TARGET_RX`.

**Remaining capture blocker:** none identified. Capture the protected-5-V AO3401A/2N7002KT1G high-side PWM circuit exactly as specified; 27 Ω selection and thermal acceptance remain prototype gates.