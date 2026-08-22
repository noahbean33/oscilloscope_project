# Board Bring-Up Plan

## Safety boundary

**0–12 V DC RATED MEASUREMENT; 15 V OVERLOAD-SURVIVAL TARGET UNVERIFIED; GROUND REFERENCED; NOT ISOLATED; NO MAINS/CAT/HIGH ENERGY; NO NEGATIVE INPUT.**

## Power-off checks

1. Verify no short from VBUS_RAW, VBUS_PROT, 3V3 or VDDA to GND.
2. Confirm the fitted GND–AGND 0 Ω bond and USB-shield 0 Ω production bond.
3. Confirm BAV199 orientation on both channels: pin 1 AGND, pin 3 CHx_CLAMP, pin 2 VDDA.
4. Confirm LCD option and straps: pins 11–14, 19 and 20 high; pins 15–17 common MOSI; pin 18 SCK; pins 1–4 unused; pin 6 solid GND.

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
7. Perform the square-wave compensation procedure using the fitted 8.2 pF and DNI 2.2/4.7 pF trim locations.

## Prototype gates

Do not close the design on schematic evidence alone for analog bandwidth/settling, 15 V overload survival, ESD/hot-plug robustness, timebase accuracy, compensation selection, backlight value, or regulator/MOSFET/resistor thermal performance.
