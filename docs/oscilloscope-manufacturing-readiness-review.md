# Manufacturing Readiness Review

**Review date:** 2026-08-28  
**Scope:** Pre-export validation for bare-board fabrication and SMT assembly. No manufacturing files were exported during this review.

## Release Decision

| Package | Status | Decision |
|---|---|---|
| Bare PCB fabrication | **Conditionally ready** | Mandatory connectivity/fabrication gates pass. Resolve or formally waive the two protected dangling-copper warnings before release. |
| SMT assembly | **Blocked pending BOM cleanup** | Fourteen fabricated PCB test pads are currently included as BOM elements and should be excluded from BOM and pick-and-place outputs. |

## Mandatory Fabrication Gates

| Gate | Result | Evidence |
|---|---|---|
| Airwires | **PASS** | 0 remaining airwires. |
| DRC errors | **PASS** | 0 current DRC errors. Two protected dangling-trace warnings remain and are tracked separately. |
| Overlapping copper / shorts | **PASS** | 0 open overlapping-copper or overlapping-trace errors. |
| Missing footprints | **PASS** | 101 schematic components are placed; no missing-footprint error. |
| Invalid layers | **PASS** | No invalid-layer error. |
| Board outline | **PASS** | Closed 100 mm × 100 mm rectangular outline with 2 mm corner radius. |
| Stackup definition | **PASS WITH FAB CONFIRMATION** | Four copper layers; nominal finished thickness 1.30 mm. Custom construction must be approved by the selected fabricator. |
| Components outside board | **PASS / MECHANICAL REVIEW REQUIRED** | No open component-outside-board DRC. J1, J2, and J3 intentionally overhang the board edge; confirm panelization and enclosure clearances. |
| Production MPN coverage | **PASS WITH EXCLUSIONS** | Active components and fitted passives have MPNs. DNI capacitors are marked excluded. |

## Fabrication and Assembly Blockers

### 1. Fabricated test pads are included in the assembly BOM

**Severity:** Assembly-package blocker  
**Affected:** TP_3V3, TP_CH1_DIV, TP_USB_DM, TP_CH1_IN, TP_VBUS_RAW, TP_BL_K, TP_USB_DP, TP_CH2_DIV, TP_NRST, TP_VDDA, TP_CH1_ADC, TP_CH2_ADC, TP_CH2_IN, TP_VBUS_PROT.

These are 1.0 mm × 1.0 mm PCB copper features but currently appear as a BOM line with MPN `PCB-TESTPAD-1.0X1.0`. Mark all fourteen **Exclude from BOM = true** and **Exclude from Pick & Place = true** before producing assembly files. TP_GND is a real Keystone 4952 component and should remain in the BOM.

### 2. Two protected dangling-copper groups remain

**Severity:** Fabrication release warning; waiver or cleanup required  
**DRC groups:** `Trace a4ae` and `Trace b74f`.

The board has zero airwires and no copper-short errors, so these warnings do not represent missing electrical connectivity. They are locked legacy protected copper. Remove them if the editor permits, or visually verify in the Gerber viewer that they do not create exposed stubs, unintended antennas, mask slivers, or copper near the board edge. Record an explicit waiver if retained.

## Required Fabricator Confirmations

1. **Stackup:** Approve the custom four-layer, 1.30 mm nominal construction rather than silently substituting a standard 1.6 mm stackup.
2. **USB impedance:** Request 90 Ω differential USB 2.0 routing (45 Ω single-ended target per leg) using the released stackup. The configured target is a design target, not measured or certified impedance.
   - The final USB_DP repair is a bottom-layer bypass with explicit top/bottom transition vias; inspect both transitions and reference-plane continuity in Gerbers.
3. **Minimum geometry:** Confirm support for 0.15 mm USB traces, 0.15 mm USB clearance rule, and 0.30 mm finished via drills with 0.60 mm pads.
4. **Edge connectors:** Confirm J1 USB-C and J2/J3 BNC overhangs are acceptable for panel rails, routing tabs, and assembly fixtures.
5. **Surface finish:** Select a finish compatible with the USB-C fine-pitch contacts and exposed test pads; ENIG is preferred for flatness and probe durability.

## BOM and Assembly Notes

- 101 placed components; all are on the top side.
- DNI capacitors C16, C17, C23, and C24 are the live exclusion set; verify all four are omitted from generated BOM and pick-and-place outputs.
- Several active parts are extended-class items for JLCPCB-style assembly; verify stock and substitution policy before ordering.
- Confirm polarity/orientation for LEDs, D1/D2/D3/D4, Q1/Q2, and connector pin-1 conventions in the pick-and-place preview.

## Pre-Order Checklist

- [ ] Exclude the fourteen fabricated test pads from BOM and pick-and-place.
- [ ] Verify C16/C17/C23/C24 are excluded from the generated assembly outputs.
- [ ] Remove or waive `Trace a4ae` and `Trace b74f` after Gerber inspection.
- [ ] Confirm the 1.30 mm four-layer stackup with the fabricator.
- [ ] Confirm controlled-impedance service and USB geometry.
- [ ] Inspect all Gerber layers, solder-mask openings, drill files, and the closed outline in an independent viewer.
- [ ] Verify J1/J2/J3 edge geometry against panelization and enclosure constraints.
- [ ] Review BOM quantities and pick-and-place rotations before release.

## Conclusion

There are **zero airwires and zero DRC errors**, with no hard bare-board connectivity or copper-short blockers. SMT assembly export is blocked until the fourteen fabricated test-pad entries are excluded. The two protected dangling-copper warnings, USB transition-via review, and the custom stackup require explicit review before the manufacturing package is released.
