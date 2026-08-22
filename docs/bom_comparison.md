# Oscilloscope BOM Comparison

## Locked architectural conclusion

The comparison design's external LTC2267 ADC, high-speed amplifiers, bipolar rails, FPGA/PMOD-style host assumptions, and high placement count are **not adopted** for Sillyscope Rev 0.2 MVP. Sillyscope retains the STM32L432KCU6 internal ADC and original standalone LCD/buttons/LEDs/native-USB/two-BNC architecture.

The transferable lessons—defined attenuation, protection, buffering, deliberate filtering, calibration, test points, grounding, and power integrity—are incorporated in [MVP Design Decisions](https://www.flux.ai/nbean/oscilloscope/files/mvp-design-decisions).

## Rev 0.2 answer to the comparison

| Comparison topic | Rev 0.2 MVP choice |
|---|---|
| ADC/host | Keep STM32 internal 12-bit ADC, DMA, native USB, LCD/UI |
| Channels | Keep two sequentially sampled channels; disclose 312.5 ns nominal skew |
| Input | Two identical 1.10 MΩ, 5.5:1, 0–12 V normal channels |
| Protection | Split precision divider, BAV199 low-leakage clamps, buffered ADC input; ELV-only, no CAT rating |
| Amplifier | One TLV9062IDR dual 10 MHz RRIO SOIC-8 |
| Filter | 330 Ω/1 nF per channel plus compensated divider; ~200 kHz expected useful bandwidth |
| Trigger | Software rising/falling Auto/Normal/Single with circular pretrigger |
| Clock | HSI retained; USB-SOF/factory correction instead of HSE for MVP |
| Power | Existing AP7361C retained; backlight moved to protected VBUS |
| Calibration | External precision DC/square-wave procedure; coefficients stored in MCU flash |
| Complexity | No external ADC, FPGA, bipolar rail, GDT, PMOD host, or high-speed differential ADC driver |

## Important boundary

The Rev 0.1A schematic still has direct BNC-to-ADC paths and lacks these proposed additions. No Rev 0.2 capability or safety rating should be attributed to Rev 0.1A.

## Deferred comparison-design features

External ADC/FPGA, simultaneous sampling, bipolar inputs/rails, multiple ranges, AC coupling, hardware trigger, high-order filtering, onboard calibration source, and GDT/surge-rated protection remain deferred until a future product requirement justifies their cost and validation burden.
