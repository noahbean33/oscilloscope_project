# Block Diagram

## Rev 0.2 status

The following architecture is locked for schematic capture but is not yet implemented or tested.

```mermaid
flowchart LR
  B1[CH1 BNC\n0–12 V rated] --> D1[1.10 MΩ low-frequency nominal\n5.5:1 compensated divider]
  B2[CH2 BNC\n0–12 V rated] --> D2[1.10 MΩ low-frequency nominal\n5.5:1 compensated divider]
  D1 --> C1[CH1_CLAMP\nBAV199: P1 AGND, P3 node, P2 VDDA]
  D2 --> C2[CH2_CLAMP\nBAV199: P1 AGND, P3 node, P2 VDDA]
  C1 --> R1[10 kΩ] --> O1[TLV9062 buffer A] --> F1[330 Ω + 1 nF] --> A1[PA0 / ADC1_IN5\nCH1 rank 1]
  C2 --> R2[10 kΩ] --> O2[TLV9062 buffer B] --> F2[330 Ω + 1 nF] --> A2[PA1 / ADC1_IN6\nCH2 rank 2]
  A1 --> MCU[STM32L432KCU6\n80 MHz ADC clock target\n3.2 Msps aggregate target]
  A2 --> MCU

  USB[USB-C VBUS + USB FS] --> ESD[USBLC6-2SC6]
  ESD --> MCU
  USB --> PF[MF-PSMF035X-2] --> V5[Protected VBUS]
  V5 --> LDO[AP7361C 3.3 V]
  V5 --> PM[AO3401A P-MOS high-side switch\n100 kΩ gate pull-up = default OFF]
  PM --> BR[27 Ω, 0.5 W provisional] --> BL[ERM19296-1 LEDA pin 5]
  BL --> GND[LCD pin 6 VSS\nsolid common ground/return]
  LDO --> MCU
  LDO --> VDDA[Filtered VDDA]
  VDDA --> C1
  VDDA --> C2
  LDO --> OP[TLV9062]
  LDO --> LCD[ERM19296-1 exact\n4-wire SPI, 1x20 2.54 mm]
  MCU --> LCD
  MCU -->|PWM high = ON\n100 Ω gate series| NM[2N7002KT1G\n100 kΩ gate pulldown]
  NM -->|pulls P-MOS gate low| PM

  MTX[MCU TX] -->|VCP_TARGET_TX_TO_PROBE_RX| PRX[STDC14 probe RX]
  PTX[STDC14 probe TX] -->|VCP_PROBE_TX_TO_TARGET_RX| MRX[MCU RX]
```

## Timing and rating annotations

- ADC sequence target: CH1 rank 1 then CH2 rank 2.
- Target timing: 12.5-cycle sample + 12.5-cycle conversion at 80 MHz, giving 312.5 ns skew and 1.6 Msps/channel at 3.2 Msps aggregate.
- These timing values require firmware and bench confirmation.
- Normal measurement rating is 0–12 V DC. Fifteen-volt survival is an unverified overload target, not a rating.

## Ground and safety

BNC shells, AGND/PCB ground, MCU ground, and USB ground are common. Rev 0.2 remains non-isolated and has no CAT, mains, high-energy, or negative-input rating.

Detailed interface pin dispositions and verification gates: [MVP Design Decisions](https://www.flux.ai/nbean/oscilloscope/files/mvp-design-decisions).

The high-side switch preserves the required solid VSS/backlight return on pin 6. The 27 Ω value remains provisional pending exact module Vf/current measurement, 60–75 mA acceptance, and resistor/MOSFET thermal verification. **No true schematic-capture blocker remains identified.**