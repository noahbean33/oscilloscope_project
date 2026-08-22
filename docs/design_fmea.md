# Design FMEA

## Status convention

Locked/proposed dispositions are approved for schematic capture; they are not implemented, tested, measured, or certified until verified on hardware.

| Risk | Locked Rev 0.2 disposition | Residual prototype verification gate |
|---|---|---|
| Wrong ADC channel/order | CH1 = PA0/ADC1_IN5 rank 1; CH2 = PA1/ADC1_IN6 rank 2 | Firmware register/readback and injected-signal confirmation |
| ADC timing claim error | 80 MHz asynchronous clock target; 12.5+12.5 cycles; timer-triggered 3.2 Msps aggregate | Confirm 312.5 ns skew and 1.6 Msps/channel on firmware/bench; do not present as measured beforehand |
| BNC overvoltage/ADC damage | 1.10 MΩ low-frequency nominal divider; BAV199 pin 1→AGND, pin 3→CHx_CLAMP, pin 2→VDDA; 10 kΩ to buffer | 15 V overload-survival test, recovery, latch-up, and cross-channel corruption |
| Overstated input rating | 0–12 V DC is the normal rated range | 15 V remains an unverified overload target, not certification |
| Divider tolerance misstatement | Three 300 kΩ ±0.1% parts form 900 kΩ ±0.1%; guaranteed ratio error +0.1637/−0.1635% | Calibration and temperature characterization; RSS kept separate from worst case |
| Aliasing/source-dependent ADC error | Buffered 330 Ω/1 nF ADC drive and 12.5-cycle sample target | Settling, crosstalk, bandwidth, flatness, and alias characterization |
| Divider compensation mismatch | Fixed compensation plus DNI trim pads | Prototype square-wave trim gate |
| HSI timebase error | USB-SOF/factory correction approach | Accuracy over voltage and 0–50 °C |
| VCP direction error | `VCP_TARGET_TX_TO_PROBE_RX` and `VCP_PROBE_TX_TO_TARGET_RX` | Continuity and loopback |
| LCD interface mismatch | Exact ERM19296-1 4-wire SPI pin-header option; 1×20, 2.54 mm interface and serial straps documented | Incoming inspection, cold-start, and strap verification |
| Backlight ground interruption | High-side AO3401A switch feeds LEDA pin 5; LCD pin 6 VSS remains permanently grounded | Continuity inspection confirms no switch or series element in pin-6 ground path |
| Backlight unintended ON / wrong polarity | AO3401A gate has 100 kΩ pull-up; 2N7002 gate has 100 kΩ pulldown; MCU PWM high = backlight ON | Verify reset/boot default OFF and PWM polarity on bench |
| Backlight overcurrent/overheating | 27 Ω, 0.5 W is provisional; acceptance is 60–75 mA | Measure exact module Vf/current; verify resistor, MOSFET, LCD, and enclosure temperatures |
| Backlight MOSFET overstress | AO3401A: −30 V VDS, ±12 V VGS, ≤60 mΩ at −4.5 V; 2N7002KT1G: 60 V VDS, ±20 V VGS, ≥230 mA stated continuous capability | Confirm fitted exact MPNs and source/drain/gate pin mapping during capture and assembly review |
| USB ESD/hot-plug | USBLC6-2SC6, PPTC, TVS, rated input capacitor | ESD, hot-plug, and nuisance-trip bench tests |
| LDO/MOSFET/LCD overheating | 70 mA 3.3 V design load and 75 mA backlight budget | Thermal tests at room and elevated ambient |
| Unsafe floating/high-energy use | Common-ground warnings and 0–12 V normal rating | Label/manual/UI inspection; no safety certification claim |
| Unassigned headers | Remove both TS-107-T-AA headers | BOM and schematic audit |

## Divider statistical note

Guaranteed worst-case divider ratio is approximately **+0.163741% / −0.163532%**. First-order RSS is **0.0945% (1σ)** only if each 0.1% tolerance is treated as an independent 1σ value, or **0.0545% (1σ)** if ±0.1% is treated as a rectangular bound. RSS values are statistical estimates and do not replace guaranteed limits.

## Capture blockers

**No true schematic-capture blocker remains identified after the protected-5-V high-side PWM correction.** Remaining items are firmware, incoming-inspection, current tuning, thermal, or prototype verification gates.