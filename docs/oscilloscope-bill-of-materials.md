# Bill of Materials

## Live design basis — 2026-08-28

The Oscilloscope design contains **101 schematic components and 57 nets**. It implements the STM32L432KCU6 internal-ADC MVP architecture. This file is a curated BOM guide; the generated BOM remains the export source of truth and must be reviewed before release.

## Principal fitted parts

- U1: STM32L432KCU6, UFQFPN-32.
- U2: AP7361C-33Y5-13 3.3 V LDO.
- U3: TLV9062IDGKR dual op amp, VSSOP-8.
- Q1: NCE3401 P-channel high-side backlight switch.
- Q2: 2N7002KT1G gate pull-down.
- J1: USB4215-03-A USB-C receptacle; J2/J3: Molex 0731000105 BNC; J4: SSW-120-01-T-S LCD socket; J5: FTSH-107-01-L-DV-K-P-TR debug header.
- TP_GND: Keystone 4952 physical test point; **include in BOM and pick-and-place**.

## Critical fitted passives

| Designators | Value / MPN | Status |
|---|---|---|
| R7–R9, R13–R15 | 300 kΩ Vishay TNPW1206300KBYEA | Fitted precision high arms |
| R10, R16 | 200 kΩ Vishay TNPW1206200KBYEA | Fitted precision low arms |
| R12, R18 | **47 Ω Yageo RC0603FR-0747RL** | Fitted ADC isolation resistors |
| C18, C25 | 1.0 nF KEMET C0603C102J5GACTU | Fitted ADC capacitors |
| R19 | 27 Ω Yageo RC2010FK-0727RL | Fitted provisional backlight value; validate operating point |

## DNI and fabricated-feature exclusions

- **DNI:** C16, C17, C23, and C24; exclude from BOM and pick-and-place.
- Fourteen 1.0 × 1.0 mm fabricated PCB test pads currently appear as BOM elements and are an **SMT assembly blocker**: TP_3V3, TP_CH1_DIV, TP_USB_DM, TP_CH1_IN, TP_VBUS_RAW, TP_BL_K, TP_USB_DP, TP_CH2_DIV, TP_NRST, TP_VDDA, TP_CH1_ADC, TP_CH2_ADC, TP_CH2_IN, TP_VBUS_PROT. Exclude all fourteen from BOM and pick-and-place.
- TP_GND is not one of the fabricated pads and remains included.

## Manufacturing status

The 100 × 100 mm PCB uses four copper layers and a nominal 1.30 mm custom stackup; all 101 components are top-side. Routing is complete with zero airwires and zero DRC errors. The configured USB target is 45 Ω single-ended / 90 Ω differential and requires fabricator confirmation; it is not measured or certified impedance. Protected dangling-trace warnings `Trace a4ae` and `Trace b74f` require Gerber cleanup or explicit waiver.

## Claim boundary

BOM selection and implementation do not establish measured bandwidth, divider compensation, 15 V overload survival, backlight operating point, USB ESD/hot-plug robustness, ADC timing, thermals, HSI accuracy, certification, or production readiness.
