# Simulation Results

## Scope and claim boundary

This file records Rev 0.2 analog-front-end SPICE simulations and power-path bounded calculations using the current schematic values and selected MPN properties. **These are modeled results, not measured prototype performance, safety certification, surge/ESD verification, or proof of component survival.**

Simulation date: 2026-08-24.

## Executive reconciliation

| Locked target / question | Result | Disposition |
|---|---|---|
| 0–12 V normal range | **Pass with limitation** | Nominal 12 V produces about 2.1817 V at ADC. TLV9062 worst-case output swing implies a possible ~20 mV zero floor (~0.11 V input-equivalent), requiring calibration/bench characterization. |
| 15 V overload-survival target | **Pass in electrical operating-point model; survival unverified** | 15 V produces about 2.7271 V; divider/clamp currents and resistor powers are small and the op-amp remains in its recommended common-mode range. Hardware overload/recovery testing remains mandatory. |
| ±1 dB DC–100 kHz | **Fail with fitted 8.2 pF only; pass with trim** | Baseline peaks about +2 to +4 dB depending BAV199 capacitance. Robust modeled choice is 15.1 pF total low-arm capacitance (8.2+2.2+4.7 pF); 12.9 pF is flattest if diode/stray capacitance is near 4 pF. |
| Expected ~200 kHz usable bandwidth | **Conditional pass** | 15.1 pF passes 100 kHz robustly but can be slightly beyond −1 dB near 200 kHz with high diode capacitance/source resistance. 12.9 pF gives ~298 kHz ±1 dB span in the 4 pF diode-cap case. Bench trim remains required. |
| ADC settling at 80 MHz, 12.5 cycles, original 330 Ω/1 nF | **Failed; corrected** | The original network produced about 9.5–9.9 LSB external acquisition error. R12/R18 are now 47 Ω with C18/C25 retained at 1 nF. |
| Implemented 47 Ω/1 nF network | **Pass in bounded model** | Worst modeled external acquisition error is 0.450 mV = 0.558 LSB for RADC bounds 0–680 Ω. Schematic and production MPN were updated before layout. |
| USB 125/145/175 mA budget cases | **Pass** | Including 80 µA LDO IQ and 20 µA TVS leakage gives approximately 125.10/145.10/175.10 mA. |
| AP7361C headroom/thermal | **Pass, layout-dependent** | Worst protected VBUS 4.3648 V; no dropout rows. Maximum LDO dissipation 0.1919 W, corresponding to ~28.8 °C rise with datasheet 150 °C/W test-board θJA. |
| Backlight current | **Unresolved by design evidence** | 60/75 mA were imposed load cases. Required LCD LED Vf is calculated, not verified. Retain the 27 Ω value-selection prototype gate. |

## Authoritative source parameters and model limitations

The following source-identified values were extracted from the manufacturer datasheets attached to the current project components. The extraction path returned OCR evidence without immutable inline citation IDs, so these facts are identified by datasheet document/table rather than represented as citation-resolved claims.

- **TLV9062, TI SBOS839N:** 10 MHz gain-bandwidth, 55° typical phase margin, 6.5 V/µs typical slew rate, common-mode range V−−0.1 V to V++0.1 V, output swing within 20 mV maximum of the rails at 10 kΩ, 0.5 pA typical input bias, 100 pF stated capacitive-load drive, signal-input absolute maximum V−−0.5 V to V++0.5 V and ±10 mA.
- **STM32L432, ST DS11451 Rev 4 Tables 63/64 and Figure 22:** CADC=5 pF typical; PA0/PA1 are fast channels; 12-bit 12.5-cycle sampling at 80 MHz is 156.25 ns; maximum external input impedance is 680 Ω.
- **BAV199LT1G, onsemi electrical characteristics:** each diode has ≤2 pF capacitance at 0 V/1 MHz, ≤5 nA leakage at 70 V/25 °C, and maximum Vf anchors 0.9 V at 1 mA and 1.0 V at 10 mA.
- **AP7361C, Diodes Inc. DS37274:** IQ 60 µA typical/80 µA maximum; dropout ≤140 mV at 300 mA for 2.6–3.3 V versions; output capacitor ≥2.2 µF with 10–300 mΩ ESR; 150 µs typical startup test; SOT-89-5 θJA=150 °C/W on the specified 1×1 inch minimum-pad FR-4 board.
- **MF-PSMF035X-2, Bourns:** 0.35 A hold, 0.75 A trip, 0.25 Ω Rmin, 1.2 Ω R1max, 0.14 A hold at 85 °C.
- **SMAJ5.0A-TR, ST:** 5.0 V stand-off, 20 µA maximum leakage at the stated test point, 6.4 V minimum breakdown, 13.4 V maximum clamp at the specified pulse current. TVS capacitance was not numerically resolved from the graph.
- **NCE3401:** 75 mΩ maximum RDS(on) at VGS=−4.5 V; θJA=104 °C/W under its stated board condition.
- **2N7002KT1G:** 2.5 Ω maximum RDS(on) at 4.5 V/200 mA; steady-state θJA depends strongly on copper area (300–417 °C/W stated conditions). Its steady load here is only the ~50 µA pull-up current.
- Selected passives are the exact MPNs in [Passive Sourcing Report](../files/passive-sourcing-report): Vishay TNPW 300 kΩ/200 kΩ precision parts, KEMET C0G compensation/filter capacitors, TDK 4.7 µF X7R regulator capacitors, and Yageo resistors.

No directly integrated vendor TLV9062 or AP7361C ngspice macromodel was available in the simulation invocation. The TLV9062 was therefore represented by a behavioral model constrained by datasheet GBW, phase margin, slew rate, common-mode, and output swing. AP7361C dynamic behavior is reported as bounded capacitor/ESR/dropout analysis rather than a fabricated loop model. TDK 4.7 µF DC-bias derating was not numerically available; transient calculations use nominal and tolerance-minimum 4.23 µF and must be rechecked against measured effective capacitance.

## Analog modeled circuit

One channel represents both identical channels:

`Vin—Rs—R7 300k—R8 300k—R9 300k—CH1_CLAMP; R10 200k to AGND; 12 pF across each 300k; 8.2 pF plus optional 2.2/4.7 pF across R10; BAV199 pin1 AGND, pin3 clamp, pin2 VDDA; R11 10k to TLV9062 follower; R12 47 Ω to ADC; C18 1 nF to AGND; STM32 CADC 5 pF.`

Source resistance cases were 50 Ω, 1 kΩ, and 100 kΩ. BAV199 off-capacitance was bounded from 0 to 4 pF total at the clamp node.

### DC and gain/linearity

| Vin | Ideal divider | Simulated ADC, nominal Rs=50 Ω |
|---:|---:|---:|
| 0 V | 0 V | ~20 mV behavioral output-floor bound |
| 0.2 V | 36.364 mV | 36.362 mV |
| 2 V | 0.363636 V | 0.363617 V |
| 6 V | 1.090909 V | 1.090849 V |
| 10 V | 1.818182 V | 1.818081 V |
| 12 V | 2.181818 V | 2.181697 V |
| 15 V | 2.727273 V | 2.727121 V |

At +20 V and low source resistance, CH1_CLAMP is about 3.636 V: above the 3.4 V recommended common-mode limit but below the 3.8 V absolute-maximum input-voltage limit. This is functional overdrive, not a verified safe operating condition. At −15 V, the external clamp model gives about −0.71 V, beyond the TLV9062 −0.5 V absolute-maximum voltage bound before its internal input clamp is included. Negative inputs remain explicitly prohibited.

Worst divider resistor dissipation over −15 to +20 V was ~0.10 mW per 300 kΩ part versus 0.52 W rating. This does not constitute a high-energy-source safety assessment.

### AC compensation

For Rs=50 Ω and BAV199=4 pF total:

| Low-arm C | Peak | ±1 dB span | −3 dB BW | 100 kHz result |
|---:|---:|---:|---:|---|
| 8.2 pF | +1.98 dB | 41 kHz | 774 kHz | Fail |
| 10.4 pF | +1.08 dB | 76.8 kHz | 645 kHz | Marginal/fail |
| 12.9 pF | +0.22 dB | 298 kHz | 523 kHz | Pass |
| 15.1 pF | no positive peak | 183 kHz | 429 kHz | Pass |

For BAV199=0 pF, only 15.1 pF passes the ±1 dB through 100 kHz target robustly. The 100 kΩ source reduces low-frequency gain as expected and narrows bandwidth, but 15.1 pF still passes 100 kHz.

**Expected population:** fit both trim parts for **15.1 pF total** as the robust simulation default, then select 12.9 pF instead only if prototype measurements show clamp/stray capacitance near the upper modeled bound. The current 8.2 pF-only fitted baseline does not meet the locked flatness target in the model.

[Exact raw-vector Bode plot](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-cfeb2e3c91774b860bef7d3d6ae9f5212f864f685733756fa9281511fd41daad.png?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214127&Signature=Lf%2FrHdgx5H%2FaD0%2Bh8X5FtGj8jlMVGmq4o36fgcOu%2FSzjUYUHSWTv3LIZNlh9x4mfsiNdUhpTI1EniIzSkVaIt8fDaemKt0i2XtxrKTOhvezT%2BIvJ91Su%2FnPs3HD%2FONSdaexcjndIYEs1bCyzdoOQEeCwq1%2FAjzsmxSIkcHGgy4oqO4Z1Sle6TLD47l0iRo7%2BbLTO%2Bw7qK4Bhwvx4uGpuLIC8IE%2FQtXFb9Dm1asg%2BWck6KpUc%2BBNM0IcCop%2BHA521tUffq9bvrVpW1E2A4mScubDFkGIsrQpKpP54GAFYq0hNBk4xkzvwrcMPB6Gt1oZPjiZrHJDmJ1jCEdfS3fZXhw%3D%3D)

[Exact plotted AC vectors CSV](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-6255a8716985bdaef2be784c032de7df9cbc3f936438c63883aa5ea522067ec8.csv?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214128&Signature=Eph0%2FTa4QgNJkYL%2By1CLoeYtUBGbh2dLCn4WuchoSGOGMOR4qQMluw1GugfAaDNVybmKFhTec3LaEFtih5LLdHPzgVwKbwojDmT4P99sWUQ8eqQHu5WbXEJuYkcxpccbCtIX6RCXGK%2FlB9Y3E7WX0VzBCHfKJLITtgbj1kVQKkG3Mr8oYzMKp71XAgR9YYQcyNTfao6BqBS%2BjFiQoC93WamLhVDTE6AoYE1LrtSRxnWablDg6qFRWJdSac5yp1AqBM8aZPNIGz5A8OqeL9YvQjoJSP4voxMKCDl3eEEXXlN2WT9dc%2FW%2FzgoWgypgPKfoRWnl%2F96SsN5fug0lqtM%2BXg%3D%3D)

### Transient behavior

The exact four-case AC sweep is authoritative for trim selection. A separate transient run used a simulator-selected ~14 pF low-arm case, not an exact production population; it is retained only as a behavioral illustration. It reduced rising-edge overshoot from ~26–27% with 8.2 pF to ~3%. Full-scale rising-step settling to ±1 ADC LSB remained ~17.5 µs in that behavioral model. Detailed THD and exact settling require the vendor macromodel and prototype measurement.

[100 kHz behavioral transient plot](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-f514b4784acf8fbf21e6c81846089c4d95705bf9c503eca6c23d4357e78c8a13.png?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214017&Signature=ilPBvf60ybSt9gNA6Pc1m6TRukcFVMTuNxoe5g7JNQqR0MhTuzj%2BZ%2BtlnB08%2B5I%2BGWqMkvv3PUrRydqrEpRBsKopaHgpfZVUCoKDVIXWZHrBKq74%2BkyEXNTnqxPcJzNHFQWCOPWuTRegY1N7cILDCrPEL6woH9a%2FIcwrZHIK0h5GpApzvdf6LjXglfBf30uDCcODJTwH2Tee%2BcphyU9sGBcGcWTe6FVly8nQCGUQ%2BaiXzxoj53rOWKT7mmSBJiw2QDGziB8LyB9el36S%2BTEBuDfVkJd2pEUpPokeCwtQdx2J1OZS%2B2m%2FY3Frxa4dUOBDxwk4yRVaFrI5c1XBssieUA%3D%3D)

### ADC acquisition settling and implemented correction

The original 330 Ω/1 nF network failed the 12.5-cycle alternating-channel case:

- End-of-acquisition error: **7.63–7.99 mV = 9.48–9.91 LSB** for RADC bounds 0–1.5 kΩ.
- Dominant mechanism: 5 pF sample-cap charge sharing kicks the local 1 nF node by ~11–12 mV, followed by recovery through 330 Ω.
- This external dynamic error is additional to the STM32 ADC intrinsic TUE.

[ADC acquisition waveform](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-d196929d74555ab5165f09f0cc83d76e55a8e330eb5f28de092caae7130d685f.png?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214018&Signature=JGOoCEmHXWydq83cMLjXypKU6oEtmxP4Tfawdc1ohzSr453KSZfAq0Q1VkA%2Bm5LBpBQJvOqa2th9jVpxCIfWAehKBdApNH5ks0whKESJpja6c40SFvehtq%2Fj4xOjxQHnjjxdfUYJztMXOysQKHPXqQMZdL4n0IZeeYdKyW703JosMquAts3UxXacW9UKCFAtWNgWpDOTQwnfHpbIRlcoPwcbfVE9MMrFBYCvVDDwOblyUSpTu%2Fhkb1%2B4snHAcPF%2Bf2oXnwdEctpm7LV91CFM7jUpkPM%2Fxm9hkelT0oexuBQnIR%2BI0g4SZ0d6ckHD3Hy4kT4rSTwW687qt8fP51EFQw%3D%3D)

A candidate sweep showed **47 Ω with the existing 1 nF** is the least-invasive value change that preserves the filter capacitor and passes both RADC=0 and 680 Ω bounds: **0.450 mV = 0.558 LSB worst case**. 100 Ω/470 pF does not pass; 47 Ω/470 pF passes with larger margin but changes both parts.

[ADC candidate screening CSV](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-63e4213e45febfdf0c0494c0fe9b49147d5dc2ad50fa8f390a3d9c0cdbd51140.csv?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214858&Signature=bN7%2FflSVNu%2Fppvw7wr2fM%2Bu3sPGV4GM%2BkbI6Nrpd%2FdBr1Y7WyYtTJukbi6RUE0ADl%2BTBwErsVC9%2FFrR8UQNgYfjJAOgvt7YSx5yvflFU7SWVv2oiL5tRxgosK4t%2Fmz5EPPYuht5txpf3kdZY0zay9uk35fwy9RV3EnQ%2FIjU1D8koUb6vwAHv6yLvNOEduOUO%2BV%2F7hRB%2B%2Fc3sAlqmKj8o8CbGa0mKVOsFbv%2FSgNoxzb4u6VtM5Fr6YGzLj3ElFuBsEiwkBWx8kizgHZhmv0lmBny844aygKUyfgHGAFAu4%2BBwzLeT6VW0hHVd0a4%2FK1z%2B6zS0Kaz%2FyiFBweeUs8S2aQ%3D%3D)

**Implemented before layout freeze:** R12 and R18 were changed from **330 Ω to 47 Ω**, retaining C18/C25 at 1.0 nF. Production MPN is Yageo **RC0603FR-0747RL** for both channels. Focused hardware validation remains required for channel-to-channel acquisition settling and op-amp stability with the routed parasitics.

## Power-path model and results

### DC matrix

Modeled topology:

`USB source—cable resistance—MF-PSMF035X-2—VBUS_PROT`, with SMAJ5.0A leakage and C1=4.7 µF; AP7361C to 3.3 V with C2=4.7 µF and 100 kΩ EN pull-up; direct backlight load through NCE3401 and 27 Ω.

Matrix dimensions:

- USB: 4.75/5.0/5.25 V
- Cable: 0.1/0.5/1.0 Ω
- PPTC: 0.25 Ω cold-min and 1.2 Ω R1max
- 3.3 V load: 50/70/100 mA
- Backlight: 0/60/75 mA

Key results across 162 points:

- Worst VBUS_PROT: **4.3648 V** at 4.75 V, 1 Ω cable, 1.2 Ω PPTC, 100 mA rail and 75 mA backlight.
- Worst LDO headroom after dropout bound: **1.0181 V**; regulation failures: **0/162**.
- Maximum LDO dissipation: **0.1919 W**.
- θJA=150 °C/W calculated rise: **28.8 °C**, strongly layout/ambient dependent.
- At 75 mA backlight: 27 Ω resistor dissipation **151.9 mW**; NCE3401 dissipation using 75 mΩ maximum **0.422 mW**.
- Required LCD Vf is `VBUS_PROT − IBL×(27 Ω+0.075 Ω)` and varies with source/path case; it is not an authoritative LED Vf or a predicted current.

[Power DC matrix CSV](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-99a8e3bc4f5de6fb43ae5db692d158c7e15c89d2a157067bf285e125756307b0.csv?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214023&Signature=dfq8EvIj74v8XUKEcdmC%2BzAhHd8pJloWqJvYxzYOALKT%2Fn3XdyIeWJ%2BIpQO2xwBXquze3LFGq8mRnk2HfmxXsvZvLXwPKAw6QdvU5GUR8ExBRUMfgRZGu7jyy2PwpDctTNYtbuz2P1px%2BfNdN%2BcZeXFUzMCtYJifJD%2BddtwOae3R22bQk6E1zFF0ZLhHKGaLL3CzQiMFoQTgdPE1HlzIDJIQgWUdd%2BK2HpE9ul9tZfsFnsU88x4rfqPqgGGE13NXxL%2BbxeOrpDeSVXC03A%2BFVupRYL6j%2FRPUIaF6gm4Lrfg1BLY%2Ba%2BtTFptgTRqtTaAS5Rw2JPQ4%2FBUerrVtNfS0Jw%3D%3D)

[Power matrix heatmap](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-6da55f42e82d7b7065366a02b43b1118be3c3d9007233b856f939d9c3bea7b9a.png?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214020&Signature=HIrC9%2BvgLydBzn1hJDfkqEueb%2FDmiwJVSKjuxH1Ds%2FZeY3VajfBap4sI52I7EhPMsW7ciUEl1IBm687%2FGQIz61NJstCQBd49tIsK5ZZQQNkd3pMqFMzhLpBfF8ihuc%2FkOUspMr79tJ3DuHqqHANJAMsa323Bgxn%2BYz7KT%2FhAkTNr2VYtlfBjAr9qHEzVjDtx2fI%2FsKYi2PiqDXKzsNF4AwcWfdOTElcB6jpXnGxM2nzNHCwy80ak8onZgeV4BG5pTQhS4xNrqUIPYB7amzTjz5Rzw%2FajFFfzr5JcE582PPcBe00%2FmqJ0MpFyeOY8D7%2B01jkBMraJdpdEqybH0MyhQw%3D%3D)

### USB budget and pre-enumeration

Including worst-case modeled IQ/leakage:

- 50 mA + 75 mA → **125.10 mA**
- 70 mA + 75 mA → **145.10 mA**
- 100 mA + 75 mA → **175.10 mA**

With backlight off, 50 mA and 70 mA rail cases remain below 100 mA. The 100 mA validation load becomes approximately **100.10 mA**, so it does not meet a strict pre-enumeration <100 mA criterion. Firmware must hold the actual pre-enumeration rail load below about 99.9 mA; the planned ~50 mA state passes.

### Load transients

No vendor dynamic SPICE model was used. Bounded calculations use C2=4.23 µF tolerance-min and ESR 10–300 mΩ:

- 20→70 mA: immediate ESR step 0.5–15 mV; capacitor-only droop 11.8 mV after 1 µs, 59.1 mV after 5 µs, 118 mV after 10 µs before regulator correction.
- 20→100 mA: immediate ESR step 0.8–24 mV; capacitor-only droop 18.9 mV after 1 µs, 94.6 mV after 5 µs, 189 mV after 10 µs before regulator correction.

These are bounds, not predicted rail dips. The AP7361C datasheet includes 4.7 µF transient plots, but exact excursion was not digitized.

[Load-transient bounds CSV](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-ed3b74ce3b14fb3c72d937722a3c89047e048957841160ed969658c58d3ece9b.csv?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214024&Signature=IVnZZjjiguSxvJ%2BscpCfV4xHN8fogvK3TXhsCu4K5aCYB0sHanyy8Wo0dD5boVDPvoJ4o%2BMe%2B7Tsv1e7ASq9DPfAZE3UR3DgM5zF4Oy74FkrbJPxWAgL6O%2FirM7jkSuGfAM3gPQIkOXH6Y%2FEJDvyXrmspXtG7kjOnTWqQOzktxqNEimWL%2Fc50PEiHOBF2W7AEgK4%2Fn2lzutK%2BuMSq6uiuugGYoIZYdh3VSASh2NazEMtmykFOFuq7opVGinX2wDojTPpj55FOa3%2BDr%2FCKNap7CpscL9d2QfcmcD%2FEGopnIhak5uzElFuh9%2B%2FTpaonp3eyY%2F48VPa9ZaNuYzEdzVX9A%3D%3D)

### Startup/hot-plug

An ideal source/R/C bound using C1=4.23–4.7 µF gives 10–90% VBUS_PROT rise times of **3.25–22.72 µs** and mathematical initial currents of **2.16–15 A**. Real USB source current limiting, connector inductance, source ramp, capacitor DC bias, PPTC heating, and TVS nonlinear dynamics are not represented. AP7361C typical startup is ~150 µs under its stated test condition. No surge or ESD verification is claimed.

[Startup/hot-plug RC bounds CSV](https://cdn2.flux.ai/flux-comment-files/files/commentFileId-6d89e029df5736dd46f3270ef9f8027f01d8ae552f2b194ab310b0825ab3b982.csv?GoogleAccessId=autopilot-sa-d60c62b%40graviton-mvp.iam.gserviceaccount.com&Expires=4941214024&Signature=i0wGYiAGMPhLl7bDMxkpef0Uqbh9LI0agDyCxGwBUS8WlTA%2FAa%2FBli6X6h712t2ktuID2UrvSeh1AhtdWCPyxp%2BnHcjl2FZSKZfT8XNblk2lFdP3bTqu88Gy6ZZ7xt3UHfGqHrKQgjIuNTvO%2BTuL6udBXXLZAOQFsZtOHCpXKlRvCjO0QskmAiMRYTDwUR2cxVzFiJeI9rID%2BZXEe%2BSKuzrSL2MrxgbVrnLz5uVOpNgXm%2BTWy%2BkEyo99KOErVUdeI2D8dDo7iKscdxVTkMBK73vliGBVU5Ue%2B7fRrcscujfXDWdsXgoDdBs8uHPTNsJ%2B1rC5ypu6z80L3bnTcDByXw%3D%3D)

## Required actions before layout freeze

1. **Completed:** R12 and R18 are 47 Ω, retaining C18/C25=1 nF. Rerun final ADC acquisition and analog stability simulations if an actual TLV9062 macromodel becomes available, and validate on the prototype.
2. Keep both compensation trim footprints. Prototype default is expected to be 15.1 pF total low-arm capacitance, with 12.9 pF as the likely alternate after measured parasitic capacitance and square-wave response.
3. Preserve the existing backlight current-tuning and power/thermal prototype gates.
4. Bench-check the near-zero output floor, 15 V overload/recovery, negative-input behavior under current limitation, ADC crosstalk, and the final firmware sampling sequence.

## Project-file reconciliation

- **MVP Design Decisions:** updated to mark 12.5-cycle settling as failed with the current 330 Ω/1 nF network and record the 47 Ω recommendation.
- **Design FMEA:** updated with the acquisition-settling failure and compensation-peaking risk.
- **Board Bring-Up Plan:** updated with explicit acquisition, compensation, zero-floor, overload, power-transient, and pre-enumeration tests.
- **Power Budget:** not changed; its 125/145/175 mA rounded architecture remains correct and was confirmed by the matrix.
