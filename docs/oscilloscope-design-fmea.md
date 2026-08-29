# Design FMEA

## Status convention

Status terms are evidence-specific: proposed/locked/target does not mean implemented, and implemented/modeled does not mean measured, tested, certified, or production-proven.

| Risk | Locked Rev 0.2 disposition | Residual prototype verification gate |
|---|---|---|
| Wrong ADC channel/order | CH1 = PA0/ADC1_IN5 rank 1; CH2 = PA1/ADC1_IN6 rank 2 | Firmware register/readback and injected-signal confirmation |
| ADC timing / acquisition error | Implemented R12/R18 = 47 Ω Yageo RC0603FR-0747RL with 1 nF; bounded model gives 0.558 LSB external error | ADC timing, rank skew, settling, and crosstalk remain unmeasured |
| BNC overvoltage/ADC damage | 1.10 MΩ low-frequency nominal divider; BAV199 pin 1→AGND, pin 3→CHx_CLAMP, pin 2→VDDA; 10 kΩ to buffer | 15 V overload-survival test, recovery, latch-up, and cross-channel corruption |
| Overstated input rating | 0–12 V DC is the normal rated range | 15 V remains an unverified overload target, not certification |
| Divider tolerance misstatement | Three 300 kΩ ±0.1% parts form 900 kΩ ±0.1%; guaranteed ratio error +0.1637/−0.1635% | Calibration and temperature characterization; RSS kept separate from worst case |
| Aliasing/source-dependent ADC error | Buffered ADC drive; recommended 47 Ω/1 nF and 12.5-cycle sample target | Settling, crosstalk, bandwidth, flatness, and alias characterization with alternating near-full-scale channels |
| Divider compensation mismatch / peaking | Fixed 8.2 pF plus 2.2/4.7 pF trim pads | 8.2 pF-only model peaks ~2–4 dB. Start prototype evaluation at 15.1 pF total and compare 12.9 pF; accept only after square-wave and swept-sine flatness tests. |
| HSI timebase error | USB-SOF/factory correction approach | Accuracy over voltage and 0–50 °C |
| VCP direction error | `VCP_TARGET_TX_TO_PROBE_RX` and `VCP_PROBE_TX_TO_TARGET_RX` | Continuity and loopback |
| LCD interface mismatch | Exact ERM19296-1 4-wire SPI pin-header option; 1×20, 2.54 mm interface and serial straps documented | Incoming inspection, cold-start, and strap verification |
| Backlight ground interruption | High-side NCE3401 switch feeds LEDA pin 5; LCD pin 6 VSS remains permanently grounded | Continuity inspection confirms no switch or series element in pin-6 ground path |
| Backlight unintended ON / wrong polarity | NCE3401 gate has 100 kΩ pull-up; 2N7002 gate has 100 kΩ pulldown; MCU PWM high = backlight ON | Verify reset/boot default OFF and PWM polarity on bench |
| Backlight overcurrent/overheating | 27 Ω, 0.75 W is provisional; acceptance is 60–75 mA | Measure exact module Vf/current; verify resistor, MOSFET, LCD, and enclosure temperatures |
| Backlight MOSFET overstress | Fitted Q1 is NCE3401; Q2 is 2N7002KT1G | Confirm assembly orientation; measure operating point and thermals |
| USB ESD/hot-plug | USBLC6-2SC6, PPTC, TVS, rated input capacitor | ESD, hot-plug, and nuisance-trip bench tests |
| LDO/MOSFET/LCD overheating | 70 mA 3.3 V design load and 75 mA backlight budget | Thermal tests at room and elevated ambient |
| Unsafe floating/high-energy use | Common-ground warnings and 0–12 V normal rating | Label/manual/UI inspection; no safety certification claim |
| Assembly BOM contamination | Fourteen fabricated 1.0 × 1.0 mm test pads are currently BOM elements | Block assembly export until excluded from BOM and pick-and-place; keep TP_GND included |
| Legacy dangling copper | Zero airwires and zero DRC errors, but protected warnings remain for Trace a4ae and Trace b74f | Gerber review, cleanup, or explicit waiver before fabrication release |
| USB impedance overclaim | 45 Ω single-ended / 90 Ω differential configured; USB_DP bottom-layer bypass uses explicit transition vias | Fabricator stackup confirmation; do not claim measured or certified impedance |

## Divider statistical note

Guaranteed worst-case divider ratio is approximately **+0.163741% / −0.163532%**. First-order RSS is **0.0945% (1σ)** only if each 0.1% tolerance is treated as an independent 1σ value, or **0.0545% (1σ)** if ±0.1% is treated as a rectangular bound. RSS values are statistical estimates and do not replace guaranteed limits.

## Current blockers and gates

The ADC resistor change is implemented. SMT assembly export remains blocked by the fourteen fabricated test-pad BOM elements. Bare-board release requires cleanup or waiver of `Trace a4ae` and `Trace b74f`. Bandwidth, divider compensation, 15 V overload survival, backlight operating point, USB ESD/hot-plug, ADC timing, thermals, and HSI accuracy remain unmeasured prototype gates.