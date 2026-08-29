# Passive Sourcing Report

## Scope and result

All **63 generic passives** have specified manufacturer part numbers: **33 resistors** and **30 capacitors**. Existing values, connectivity, and footprints are preserved. The live DNI set is **C16, C17, C23, and C24**, excluded from BOM and pick-and-place.

## Grouped production selections

| Manufacturer | MPN | Value / rating / dielectric | Package | Designators |
|---|---|---|---|---|
| Yageo | RC0603FR-075K1L | 5.1 kΩ, 1%, 0.1 W, thick film | 0603 | R1, R2 |
| Yageo | RC0603JR-070RL | 0 Ω jumper, 0.1 W | 0603 | R3, R33 |
| Yageo | RC0603FR-07100KL | 100 kΩ, 1%, 0.1 W, thick film | 0603 | R4, R20, R22 |
| Yageo | RC0603FR-0710KL | 10 kΩ, 1%, 0.1 W, thick film | 0603 | R5, R6, R11, R17, R23, R25, R27 |
| Vishay | TNPW1206300KBYEA | 300 kΩ, 0.1%, 10 ppm/°C, 200 V, 0.52 W, thin film | 1206 | R7–R9, R13–R15 |
| Vishay | TNPW1206200KBYEA | 200 kΩ, 0.1%, 10 ppm/°C, 200 V, 0.52 W, thin film | 1206 | R10, R16 |
| Yageo | RC0603FR-0747RL | 47 Ω, 1%, 0.1 W, thick film | 0603 | R12, R18 |
| Yageo | RC2010FK-0727RL | 27 Ω, 1%, 0.75 W, thick film; value remains provisional in Role Details | 2010 | R19 |
| Yageo | RC0603FR-07100RL | 100 Ω, 1%, 0.1 W, thick film | 0603 | R21, R24, R26, R28 |
| Yageo | RC0603FR-07200RL | 200 Ω, 1%, 0.1 W, thick film | 0603 | R29–R32 |
| TDK | C2012X7R1E475K125AB | 4.7 µF, 25 V, ±10%, X7R | 0805 | C1, C2, C5 |
| TDK | C1608X7R1H104K080AA | 100 nF, 50 V, ±10%, X7R | 0603 | C3, C4, C6, C10, C11, C26, C28–C30 |
| TDK | C1608X7R1E105K080AB | 1 µF, 25 V, ±10%, X7R | 0603 | C7, C9, C27 |
| TDK | C1608X7R1H103K080AA | 10 nF, 50 V, ±10%, X7R | 0603 | C8 |
| KEMET | C0603C120J5GACTU | 12 pF, 50 V, ±5%, C0G/NP0 | 0603 | C12–C14, C19–C21 |
| KEMET | C0603C829C5GACTU | 8.2 pF, 50 V, ±0.25 pF, C0G/NP0 | 0603 | C15, C22 |
| KEMET | C0603C229C5GACTU | 2.2 pF, 50 V, ±0.25 pF, C0G/NP0, DNI | 0603 | C16, C23 |
| KEMET | C0603C479C5GACTU | 4.7 pF, 50 V, ±0.25 pF, C0G/NP0, DNI | 0603 | C17, C24 |
| KEMET | C0603C102J5GACTU | 1.0 nF, 50 V, ±5%, C0G/NP0 | 0603 | C18, C25 |

## Selection notes

- Precision divider parts exceed the locked requirements: 0.1% tolerance, 10 ppm/°C TCR, 200 V rating, and 0.52 W in the existing 1206 footprint. Substitution is disabled.
- C1 and C2 retain the 4.7 µF nominal value and 0805 footprint, but use a 25 V X7R part to improve DC-bias margin relative to a 16 V selection. Substitution is disabled for the regulator input/output pair.
- Analog compensation and ADC-filter C0G parts have substitution disabled.
- R12 and R18 use 47 Ω after ADC acquisition-settling simulation showed the original 330 Ω value produced approximately 9.5–9.9 LSB of external settling error at the locked 12.5-cycle sample time. The implemented 47 Ω/1 nF network models at 0.558 LSB worst case.
- Commodity pull, damping, LED, gate, debounce, and decoupling parts allow same-spec substitutes.
- C16, C17, C23, and C24 are the live DNI parts and must remain excluded from BOM and pick-and-place.

## Final verification

| Review | Open error/warning findings |
|---|---:|
| Manufacturer Part Number manufacturing review | 0 |
| Capacitor voltage rating review | 0 |
| Resistor power rating review | 0 |
| Parts availability review | 0 |
| Missing footprint review | 0 |
| Current PCB routing | 0 airwires; 0 DRC errors; 2 protected dangling-trace warnings |

The intended assembly population is **33 resistors and 26 capacitors**, with C16/C17/C23/C24 excluded. Fourteen fabricated 1.0 × 1.0 mm PCB test pads are also currently BOM elements and must be excluded from BOM and pick-and-place; TP_GND stays included.
