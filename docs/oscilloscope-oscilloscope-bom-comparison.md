# Oscilloscope BOM Comparison

## Locked architectural conclusion

The comparison design's external LTC2267 ADC, high-speed amplifiers, bipolar rails, FPGA/PMOD-style host assumptions, and high placement count are **not adopted** for the implemented Oscilloscope MVP. The live design uses 101 components and 57 nets and retains the STM32L432KCU6 internal ADC, standalone LCD/buttons/LEDs, native USB, and two BNC channels.

The transferable lessons—defined attenuation, protection, buffering, deliberate filtering, calibration, test points, grounding, and power integrity—are incorporated in [MVP Design Decisions](https://www.flux.ai/nbean/oscilloscope/files/mvp-design-decisions).

## Rev 0.2 answer to the comparison

| Comparison topic | Rev 0.2 MVP choice |
|---|---|
| ADC/host | Keep STM32 internal 12-bit ADC, DMA, native USB, LCD/UI |
| Channels | Keep two sequentially sampled channels; disclose 312.5 ns nominal skew |
| Input | Two identical 1.10 MΩ, 5.5:1, 0–12 V normal channels |
| Protection | Split precision divider, BAV199 low-leakage clamps, buffered ADC input; ELV-only, no CAT rating |
| Amplifier | One TLV9062IDGKR dual 10 MHz RRIO VSSOP-8 |
| Filter | 47 Ω/1 nF per channel plus compensated divider; bandwidth remains unmeasured |
| Trigger | Software rising/falling Auto/Normal/Single with circular pretrigger |
| Clock | HSI retained; USB-SOF/factory correction instead of HSE for MVP |
| Power | Existing AP7361C retained; backlight moved to protected VBUS |
| Calibration | External precision DC/square-wave procedure; coefficients stored in MCU flash |
| Complexity | No external ADC, FPGA, bipolar rail, GDT, PMOD host, or high-speed differential ADC driver |

## Implementation and verification boundary

The internal-ADC MVP architecture and 47 Ω ADC isolation resistors are implemented. R12 and R18 are Yageo RC0603FR-0747RL. This does not establish measured bandwidth, compensated-divider performance, overload survival, timing accuracy, thermals, USB robustness, or certified impedance.

The PCB is 100 × 100 mm, four copper layers, nominal 1.30 mm custom stackup, with all 101 components top-side. Routing is complete with zero airwires and zero DRC errors; two protected legacy dangling-trace warnings remain (`Trace a4ae`, `Trace b74f`).

## Deferred comparison-design features

External ADC/FPGA, simultaneous sampling, bipolar inputs/rails, multiple ranges, AC coupling, hardware trigger, high-order filtering, onboard calibration source, and GDT/surge-rated protection remain deferred until a future product requirement justifies their cost and validation burden.
