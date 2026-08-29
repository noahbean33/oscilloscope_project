# Power Budget

## Revision boundary

These are planning and bounded-calculation values for the implemented design, not measured results. The LCD backlight is supplied directly from protected VBUS; logic and analog circuitry use the 3.3 V LDO.

## Rev 0.2 budget

| Load | Typical/planning | Design/validation |
|---|---:|---:|
| STM32L432KCU6 | 12 mA | 25 mA allowance |
| TLV9062 dual | 1.1 mA | 1.5 mA allowance |
| LCD logic | 5 mA planning | 15 mA allowance |
| Four indicator LEDs | up to 24 mA | 24 mA |
| Miscellaneous | 3–5 mA | included in rail totals |
| **3.3 V rail** | **about 50 mA** | **70 mA design; 100 mA validation case** |
| LCD backlight from protected VBUS | **60–75 mA acceptance band** | **75 mA budget** |

## Correct USB-current derivation

An LDO input current is approximately its output current plus its quiescent current. It is not calculated by ideal power conversion. The backlight current is separately drawn directly from VBUS.

Using AP7361C maximum quiescent current of 80 µA and adding the direct-source backlight and 20 µA TVS leakage separately:

- **Typical:** `50.00 mA LDO output + 0.08 mA LDO IQ + 75.00 mA backlight + 0.02 mA TVS = 125.10 mA USB`.
- **Design:** `70.00 + 0.08 + 75.00 + 0.02 = 145.10 mA USB`.
- **Validation case:** `100.00 + 0.08 + 75.00 + 0.02 = 175.10 mA USB`.

The rounded architecture totals are 125/145/175 mA. Other direct-VBUS loads, if added, must be included separately rather than reflected through the LDO.

Backlight is held off until USB configuration; pre-enumeration target remains <100 mA.

## LDO thermal planning

At 5.0 V input and 3.3 V output:

- 50 mA → `(5.0−3.3)×0.050 = 85 mW`.
- 70 mA → **119 mW**.
- 100 mA → **170 mW**.

These are calculated dissipation targets. AP7361C case temperature, closed-enclosure temperature rise, and 3.3 V regulation under maximum load remain prototype verification gates.

## Backlight value-selection gate

Protected VBUS feeds fitted **NCE3401 Q1** as the P-MOS high-side switch, then provisional **27 Ω R19** (Yageo RC2010FK-0727RL, 0.75 W), then ERM19296-1 LEDA pin 5.

At 75 mA, NCE3401 conduction loss using 75 mΩ maximum RDS(on) at −4.5 V is about `I²R = 0.075²×0.075 = 0.42 mW`.

**27 Ω remains an initial tuning value, not a final electrical acceptance.** Measure the exact ordered module Vf/current and retain or replace the resistor so maximum-brightness current is **60–75 mA**. Verify resistor, P-MOS, LCD, and enclosure temperatures. This correction removes the backlight schematic-capture blocker without changing the 75 mA current budget.

## Protected power path gates

PPTC voltage drop/nuisance trip, VBUS hot-plug overshoot, TVS behavior, regulator and backlight thermal performance, and USB ESD performance remain unmeasured prototype gates.