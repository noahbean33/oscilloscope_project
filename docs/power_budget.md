# Power Budget

## Revision boundary

Rev 0.2 values are approved planning targets for schematic capture, not measured results. The LCD backlight is supplied directly from protected VBUS; only logic and analog circuitry use the 3.3 V LDO.

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

Using rounded rail values and neglecting the small LDO quiescent current:

- **Typical:** `50 mA LDO input + 75 mA backlight = 125 mA USB`.
- **Design:** `70 mA + 75 mA = 145 mA USB`.
- **Validation case:** `100 mA + 75 mA = 175 mA USB`.

Backlight is held off until USB configuration; pre-enumeration target remains <100 mA.

## LDO thermal planning

At 5.0 V input and 3.3 V output:

- 50 mA → `(5.0−3.3)×0.050 = 85 mW`.
- 70 mA → **119 mW**.
- 100 mA → **170 mW**.

These are calculated dissipation targets. AP7361C case temperature, closed-enclosure temperature rise, and 3.3 V regulation under maximum load remain prototype verification gates.

## Backlight value-selection gate

Protected VBUS feeds an AO3401A P-MOS high-side switch, then the provisional **27 Ω, 0.5 W** resistor, then ERM19296-1 LEDA pin 5. Pin 6 VSS remains solid ground. A 2N7002KT1G level-shift/inverter pulls the P-MOS gate low; **MCU PWM high means backlight ON**. Static pull-up current while on is approximately `5 V / 100 kΩ = 50 µA`, negligible in the rounded USB totals.

At 75 mA, AO3401A conduction loss using its 60 mΩ maximum RDS(on) at −4.5 V is about `I²R = 0.075²×0.060 = 0.34 mW`. The 2N7002 carries only the approximately 50 µA pull-up current in steady state, so its conduction loss is negligible. The resistor worst-case planning dissipation at 75 mA is `I²R = 0.075²×27 = 152 mW`, below its 0.5 W rating but still subject to board/enclosure thermal verification.

**27 Ω remains an initial tuning value, not a final electrical acceptance.** Measure the exact ordered module Vf/current and retain or replace the resistor so maximum-brightness current is **60–75 mA**. Verify resistor, P-MOS, LCD, and enclosure temperatures. This correction removes the backlight schematic-capture blocker without changing the 75 mA current budget.

## Protected power path gates

PPTC voltage drop/nuisance trip, VBUS hot-plug overshoot, TVS behavior, regulator thermal performance, and USB ESD performance remain unverified prototype gates.