# MVP Design Decisions

## Status and revision boundary

This file locks the **Sillyscope Rev 0.2 MVP** decisions for schematic capture. **Proposed/locked means approved for capture; it does not mean implemented, tested, measured, or certified.** Rev 0.2 circuitry is not yet present in the schematic. Firmware-confirmation and prototype-verification gates do not block capture unless explicitly identified as a capture blocker.

## Fixed product requirements and exclusions

- Retain STM32L432KCU6, internal ADC, native USB FS, 192×96 LCD, three buttons, four LEDs, two BNC channels, and STDC14 SWD/VCP.
- **CH1 = PA0 / ADC1_IN5. CH2 = PA1 / ADC1_IN6.** ADC regular sequence is **CH1 rank 1, then CH2 rank 2**.
- Two identical DC-coupled, unipolar channels. No AC coupling, gain/range switching, external ADC, FPGA, bipolar supply, hardware comparator trigger, isolation, battery, or probe-compensation output in MVP.
- Normal rated measurement range is **0–12 V DC**, ground referenced. **15 V is only an unverified overload-survival design target**; it is not bench-tested, safety certified, or part of the normal measurement rating.
- Prohibited: negative inputs, floating measurements, mains, CAT environments, motor/inverter power nodes, high-energy sources, or any source whose ground cannot safely be bonded to USB/PCB ground.
- Low-frequency nominal input impedance: **1.10 MΩ** per channel.
- BNC shells, PCB signal ground, MCU ground, and USB ground are common.

## Locked decision table

| Item | Rev 0.2 MVP decision | Status / verification gate |
|---|---|---|
| ADC pins and sequence | CH1 = PA0/ADC1_IN5, rank 1; CH2 = PA1/ADC1_IN6, rank 2 | Locked for capture; firmware register/readback confirmation required |
| ADC clock/rate | ADC asynchronous clock target 80 MHz; timer-triggered 3.2 Msps aggregate; 12.5-cycle sample + 12.5-cycle conversion | **Current 330 Ω/1 nF drive fails modeled acquisition settling (~9.5–9.9 LSB).** Change R12/R18 to 47 Ω is recommended before layout freeze; then re-simulate and bench-confirm. |
| Channel timing | CH1 then CH2, nominal 312.5 ns sample-time offset and 1.6 Msps/channel | Configuration targets, not measured facts |
| Input range | 0–12 V DC normal rated measurement | Locked |
| Overload | 15 V non-destructive survival target | Unverified prototype gate; not a user rating or certification |
| Input impedance | 1.10 MΩ low-frequency nominal | Locked |
| Analog architecture | Divider → CHx_CLAMP → 10 kΩ → TLV9062 non-inverting input → follower → 330 Ω/1 nF ADC RC | Locked |
| Divider | 3 × 300 kΩ high arm + 200 kΩ low arm; all 0.1%, ≤25 ppm/°C | Locked |
| Compensation | 12 pF C0G across each 300 kΩ; 8.2 pF C0G across 200 kΩ plus 2.2 pF/4.7 pF trim pads | 8.2 pF-only baseline fails modeled ±1 dB-to-100 kHz flatness. Expected robust population is both trims (15.1 pF total); 12.9 pF remains the measured-parasitic alternate. |
| Clamp | One BAV199LT1G per channel, both internal diodes used | Locked; exact mapping below |
| Buffer/filter | TLV9062IDR; 1.0 nF C0G at ADC pin; **47 Ω series recommended in place of captured 330 Ω** | Simulation-proven change required to retain 12.5-cycle target; not yet implemented |
| Clock accuracy | HSI timebase with USB-SOF/factory correction | Locked approach; HSI accuracy over temperature is a prototype gate |
| VCP | Explicit crossed target/probe UART nets | Locked; continuity/loopback confirmation required |
| LCD procurement | Exact **ERM19296-1, “4-Wire SPI Pin Header Connection”** option | Locked; connector/interface captured below |
| Backlight resistor | 27 Ω, 0.5 W initial prototype tuning value | Bounded value-selection gate: accept only if measured current is 60–75 mA |
| Backlight switching | Protected-5-V high-side PWM: AO3401A P-MOS plus 2N7002KT1G gate pull-down; LCD pin 6 remains solid ground | Locked for capture; PWM high = backlight ON; thermal verification required |
| Spare headers | Remove both TS-107-T-AA headers | Locked |
| Protection/thermal/ESD | Existing specified USB and input-protection architecture | Thermal, hot-plug, overload, and ESD performance remain prototype gates |

## Analog front end

### Per-channel connection

`BNC center → 300 kΩ → 300 kΩ → 300 kΩ → divider node/CHx_CLAMP; 200 kΩ CHx_CLAMP-to-AGND; compensation capacitors across divider resistors; BAV199 clamps at CHx_CLAMP; 10 kΩ CHx_CLAMP-to-TLV9062 non-inverting input; TLV9062 follower; 330 Ω output series; 1 nF ADC-pin-to-VSSA; MCU ADC pin.`

The divider node is the named **CH1_CLAMP** or **CH2_CLAMP** net. The 10 kΩ resistor is downstream of that net, between CHx_CLAMP and the corresponding TLV9062 non-inverting input.

### BAV199LT1G exact pin mapping

The authoritative onsemi BAV199LT1G datasheet, SOT-23 Case 318 Style 11, defines **pin 1 = ANODE, pin 2 = CATHODE, pin 3 = CATHODE-ANODE series junction**. Therefore connect each clamp as follows:

- **Pin 1 (lower-diode anode) → AGND**
- **Pin 3 (series junction: lower cathode / upper anode) → CHx_CLAMP**
- **Pin 2 (upper-diode cathode) → VDDA, the 3.3 V analog rail**

This uses both internal diodes. Do not leave either diode unused, and do not connect the upper clamp to noisy digital 3V3.

### Divider calculation and tolerance

Nominal values are `RH = 3 × 300 kΩ = 900 kΩ` and `RL = 200 kΩ`.

- Nominal ratio: `k = RL/(RH+RL) = 200/1100 = 0.1818181818`.
- Low-frequency nominal input impedance: `RH + RL = 1.10 MΩ`.
- 12 V input → 2.1818 V nominal at CHx_CLAMP.
- 15 V input → 2.7273 V nominal; this supports only the unverified overload-survival target.
- 3.3 V ADC full-scale corresponds to 18.15 V nominal input and is headroom, not a rating.

Three equal 300 kΩ, ±0.1% resistors in series form a 900 kΩ arm with **±0.1% guaranteed resistance tolerance**, because their absolute worst-case errors sum in the same proportion as their nominal values; the arm is not ±0.3%.

Exact opposing-arm worst case:

- `kMAX = 200.2/(899.1+200.2) = 0.1821158919`, or **+0.163741%** relative to nominal.
- `kMIN = 199.8/(900.9+199.8) = 0.1815208504`, or **−0.163532%** relative to nominal.

Thus the guaranteed divider-ratio limit is approximately **+0.1637% / −0.1635%** before calibration, commonly rounded to **±0.164%**.

For statistical/RSS estimates, keep assumptions explicit:

- If each resistor's 0.1% is treated as an independent 1σ value, first-order ratio RSS is **0.0945% (1σ)**.
- If ±0.1% is treated as a rectangular guaranteed bound, each resistor has σ = 0.1%/√3 and the first-order ratio RSS is **0.0545% (1σ)**.

Neither RSS figure replaces the guaranteed worst-case limit.

## ADC and firmware acquisition targets

- ADC asynchronous clock target: **80 MHz**.
- Regular conversion sequence: **ADC1_IN5/CH1 rank 1**, then **ADC1_IN6/CH2 rank 2**.
- Per-rank timing target: **12.5 ADC clock cycles sampling + 12.5 cycles conversion = 25 cycles = 312.5 ns** at 80 MHz.
- Timer-triggered sequence target: **3.2 Msps aggregate**, yielding **1.6 Msps/channel** for the two alternating ranks.
- The **312.5 ns channel skew** and **1.6 Msps/channel** values are configuration targets requiring firmware register verification and bench timing confirmation; they are not measured performance claims.
- Rev 0.2 simulation of the captured 330 Ω/1 nF ADC network produced approximately **9.5–9.9 LSB external acquisition error** for alternating 0.5 V/12 V channel-equivalent levels. A candidate sweep found **47 Ω/1 nF** gives **0.558 LSB worst-case modeled error** over RADC bounds 0–680 Ω. Treat R12/R18=47 Ω as the required pre-layout change unless a subsequent authoritative macromodel contradicts it.
- DMA circular acquisition, software trigger, and 10/50/90% pretrigger remain locked.
- HSI timebase accuracy, including temperature behavior after USB-SOF/factory correction, is a prototype verification gate.

## VCP net naming

Use direction-explicit nets and preserve the crossover:

- **VCP_TARGET_TX_TO_PROBE_RX:** MCU UART TX → STDC14 probe RX.
- **VCP_PROBE_TX_TO_TARGET_RX:** STDC14 probe TX → MCU UART RX.

Do not use ambiguous bare `TX`/`RX` names. Confirm with continuity and loopback during bring-up.

## LCD procurement and interface capture

Procurement is locked to the exact **ERM19296-1 “4-Wire SPI Pin Header Connection”** option. The module uses a **1×20, 2.54 mm-pitch pin header**. Capture the connector as follows for the ordered 4-wire SPI option:

| Pin | Module signal | Rev 0.2 disposition |
|---:|---|---|
| 1–4 | Optional font-chip SI, SO, SCLK, CS# | NC unless the exact ordered option includes and uses the font chip |
| 5 | LEDA | Backlight anode supply path through the prototype-selected series resistor |
| 6 | VSS | Common module ground and backlight return; the authoritative pin table exposes no separate LEDK connector pin |
| 7 | VDD | 3.3 V logic supply |
| 8 | A0(RS) | MCU LCD data/command select |
| 9 | RSTB | MCU LCD reset |
| 10 | CSB | MCU LCD chip select |
| 11–14 | DB7–DB4 | Tie high to VDD for serial mode |
| 15–17 | DB3–DB1 | Tie together and connect to MCU SPI MOSI/SDA |
| 18 | DB0 | Connect to MCU SPI SCK/SCL |
| 19 | ERD | Tie to VDD in serial mode |
| 20 | RWR | Tie to VDD in serial mode |

The ordered module's interface selection must be **IF2:IF1:IF0 = L:L:L for 4-wire serial SPI**. Capture A0/RS, CSB, RSTB, MOSI/SDA, and SCK/SCL explicitly. Incoming inspection must verify the ordered option and any factory straps.

The backlight uses a protected-5-V high-side PWM switch. Connect protected VBUS to the **source of AO3401A P-channel MOSFET**; connect its drain through the provisional **27 Ω, 0.5 W** resistor to LCD **LEDA pin 5**. LCD **pin 6 VSS remains continuously bonded to ground** for both logic and backlight return. Pull the AO3401A gate up to protected VBUS with **100 kΩ** for default OFF. A **2N7002KT1G** N-MOS has its drain at the P-MOS gate, source at ground, and gate driven by the MCU PWM through **100 Ω**, with **100 kΩ gate-to-ground** for default OFF.

Logic polarity is non-inverting end-to-end: **MCU PWM high turns the 2N7002 on, pulls the AO3401A gate low, turns the P-MOS on, and turns the backlight ON**. PWM low leaves both devices off through their pull resistors. AO3401A is rated −30 V VDS and ±12 V VGS, with RDS(on) maximum 60 mΩ at VGS = −4.5 V; the protected-5-V gate swing is within rating and provides negligible conduction loss at 60–75 mA. 2N7002KT1G is rated 60 V VDS, ±20 V VGS, and at least 230 mA continuous drain current under its stated minimum-pad/high-temperature condition; it only sinks the 100 kΩ pull-up current plus gate transient current. These ratings are suitable for this driver.

The 27 Ω value remains provisional: measure the exact ordered module and retain or change it only if maximum-brightness current is **60–75 mA** under the defined VBUS condition. Verify resistor and MOSFET temperatures at acceptance limits.

## Power budget and current derivation

An LDO input current is approximately its output current plus small quiescent current; it is **not** power-converted as it would be through an ideal switching regulator. The backlight is a separate direct-VBUS load.

Ignoring small LDO quiescent current in these rounded totals:

- Typical case: `50 mA 3.3 V rail + 75 mA direct-VBUS backlight ≈ 125 mA USB`.
- Design case: `70 mA + 75 mA = 145 mA USB`.
- Validation case: `100 mA + 75 mA = 175 mA USB`.

Backlight remains off before USB configuration; pre-enumeration target is <100 mA. AP7361C thermal performance, VBUS hot-plug behavior, PPTC behavior, and backlight heating remain prototype verification gates.

## Verification gates and claim limits

The following are **not measured facts** and must be labeled as prototype verification gates wherever mentioned:

- Analog bandwidth, amplitude flatness, −3 dB point, alias response, and ADC settling/crosstalk.
- HSI/timebase accuracy over voltage and temperature.
- 15 V overload survival and recovery.
- Divider compensation trim and square-wave response.
- Regulator, MOSFET, LCD/backlight, and enclosure thermal performance.
- USB and external-input ESD/hot-plug robustness.

Required warning wording: **“0–12 V DC RATED MEASUREMENT; 15 V OVERLOAD-SURVIVAL TARGET UNVERIFIED; GROUND REFERENCED; NOT ISOLATED; NO MAINS/CAT/HIGH ENERGY; NO NEGATIVE INPUT.”**

## Schematic-capture acceptance

- PA0/ADC1_IN5 is CH1 and PA1/ADC1_IN6 is CH2; rank order is documented.
- Both channels use the exact BAV199 pin mapping above and both internal diodes.
- CHx_CLAMP naming and the downstream 10 kΩ-to-op-amp connection are explicit.
- LCD connector, 4-wire SPI strapping, and signal dispositions are explicit; pin 6 remains solid ground and pin 5 is driven by the protected-5-V high-side PWM path.
- VCP crossover nets are direction-explicit.
- Both TS-107-T-AA headers are absent.
- Proposed/locked items are not described as implemented or tested.

**Remaining schematic-capture blocker:** none identified after adopting the high-side backlight switch. Firmware confirmation, compensation selection, bandwidth, HSI accuracy, overload, thermal, hot-plug, backlight-current tuning, and ESD tests remain post-capture prototype gates rather than capture blockers.