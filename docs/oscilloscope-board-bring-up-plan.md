# Board Bring-Up Plan

## Safety boundary

**0–12 V DC RATED MEASUREMENT; 15 V OVERLOAD-SURVIVAL TARGET UNVERIFIED; GROUND REFERENCED; NOT ISOLATED; NO MAINS/CAT/HIGH ENERGY; NO NEGATIVE INPUT.**

## Power-off checks

1. Before assembly release, exclude the fourteen fabricated 1.0 × 1.0 mm test pads from BOM and pick-and-place; keep TP_GND (Keystone 4952) included.
2. Review Gerbers for protected dangling traces `Trace a4ae` and `Trace b74f`; clean up or record an explicit waiver.
3. Verify no short from VBUS_RAW, VBUS_PROT, 3V3 or VDDA to GND.
4. Confirm the fitted GND–AGND 0 Ω bond and USB-shield 0 Ω production bond.
5. Confirm BAV199 orientation on both channels: pin 1 AGND, pin 3 CHx_CLAMP, pin 2 VDDA.
6. Confirm LCD option and straps: pins 11–14, 19 and 20 high; pins 15–17 common MOSI; pin 18 SCK; pins 1–4 unused; pin 6 solid GND.
7. Confirm R12/R18 are 47 Ω Yageo RC0603FR-0747RL and fitted Q1 is NCE3401.

## Initial powered checks

1. Power from a current-limited USB source with the backlight disabled.
2. Measure TP_VBUS_RAW, TP_VBUS_PROT, TP_3V3, TP_VDDA and TP_GND.
3. Check LDO and ferrite temperatures and verify stable 3V3/VDDA rails.
4. Attach STDC14 and confirm SWDIO, SWCLK, NRST and VTref operation.
5. Program minimal firmware and verify the VCP crossover with a UART loopback test.

## Functional checks

1. Confirm USB enumeration and D+/D− continuity at the small test pads without long probe stubs.
2. Exercise RUN/STOP, MODE and SELECT; verify clean active-low transitions at the MCU-side RC nodes.
3. Test all four LEDs and verify intended roles.
4. Initialize the ERM19296-1 logic before enabling the backlight.
5. Sweep PA8 PWM and measure backlight current. Retain 27 Ω only if the maximum-brightness current is 60–75 mA under the defined VBUS condition.
6. Apply precision low-voltage DC sources to CH1 and CH2. Compare TP_CHx_IN, TP_CHx_DIV and TP_CHx_ADC; verify symmetry and ADC mapping.
7. Verify the fitted ADC isolation resistors are **47 Ω R12/R18** and C18/C25 remain 1.0 nF.
8. Alternate channel-equivalent inputs near 0.5 V and 12 V at the locked 80 MHz, 12.5-cycle sequence. Measure rank-end settling/crosstalk; acceptance is ≤1 LSB external acquisition error before ADC intrinsic TUE is added.
9. Perform square-wave and swept-sine compensation tests. Start with 15.1 pF total low-arm capacitance (8.2+2.2+4.7 pF), compare 12.9 pF, and reject 8.2 pF-only unless measured DC–100 kHz response remains within ±1 dB.
10. Measure the zero-input ADC floor and input-equivalent dead zone; characterize/calibrate the TLV9062 near-ground output limit rather than assuming a true zero code.
11. Repeat 0→12 V, 12→0 V, 1 kHz square, and 100 kHz sine tests on both channels and record overshoot, settling, gain, phase, and channel symmetry.
12. Perform current-limited 15 V positive overload/recovery tests. Negative-input and +20 V abuse tests are engineering stress tests only; keep them current-limited and do not convert them into user ratings.
13. Exercise 20→70 mA and 20→100 mA 3.3 V load steps and 0→75 mA backlight steps while recording VBUS_PROT/3V3 dip, overshoot, and AP7361C temperature.
14. Verify the actual pre-enumeration state remains below 100 mA with the backlight off; the 100 mA validation rail case plus regulator/TVS current is slightly above a strict 100 mA limit.

## Prototype gates

Do not close the design on schematic, layout, routing, or simulation evidence alone for bandwidth, divider compensation, 15 V overload survival, backlight operating point, USB ESD/hot-plug robustness, ADC timing, thermals, or HSI accuracy. The configured USB 45 Ω single-ended / 90 Ω differential target is not a measured or certified result.

## PCB baseline for bring-up records

Record that the tested article is the 100 × 100 mm, four-copper-layer, nominal 1.30 mm custom-stackup board with all components top-side. Pre-test CAD status is zero airwires and zero DRC errors. USB_DP includes a bottom-layer bypass with explicit top/bottom transition vias.
