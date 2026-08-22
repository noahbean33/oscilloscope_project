# Complete Digital Oscilloscope Project Guide

**Build a Professional High-Performance Digital Oscilloscope System**

From zero to working oscilloscope with FPGA, custom PCB, and embedded software.

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Prerequisites](#prerequisites)
3. [Phase 1: FPGA Development](#phase-1-fpga-development)
4. [Phase 2: PCB Design and Manufacturing](#phase-2-pcb-design-and-manufacturing)
5. [Phase 3: Hardware Assembly](#phase-3-hardware-assembly)
6. [Phase 4: Hardware Bring-Up](#phase-4-hardware-bring-up)
7. [Phase 5: Software Development](#phase-5-software-development)
8. [Phase 6: System Integration and Testing](#phase-6-system-integration-and-testing)
9. [Troubleshooting Guide](#troubleshooting-guide)
10. [Performance Optimization](#performance-optimization)

---

## Project Overview

### What You'll Build

A **complete digital oscilloscope system** featuring:
- **100 MHz+ sampling rate** analog-to-digital conversion
- **FPGA-based digital signal processing** with Xilinx Artix-7
- **Custom 4-layer PCB** with high-speed analog front-end
- **MicroBlaze soft processor** running embedded firmware
- **Real-time waveform capture** and display
- **Ethernet connectivity** for data transfer
- **Professional instrument-grade** design

### System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                  Digital Oscilloscope                    │
├─────────────────────────────────────────────────────────┤
│                                                          │
│  Signal Input (BNC/Probe)                               │
│         ↓                                               │
│  Analog Conditioning (Amp/Atten/Filter)                 │
│         ↓                                               │
│  High-Speed ADC (SPI Interface)                         │
│         ↓                                               │
│  FPGA Logic (Artix-7)                                   │
│    ├─ Data Capture State Machine                       │
│    ├─ High-Speed Buffer (BRAM)                         │
│    ├─ Trigger Detection                                │
│    └─ MicroBlaze Processor                             │
│         ↓                                               │
│  External DDR3 Memory                                   │
│         ↓                                               │
│  Processing & Display                                   │
│    ├─ UART Terminal Output                             │
│    ├─ Ethernet Data Export                             │
│    └─ Waveform Analysis                                │
│                                                          │
└─────────────────────────────────────────────────────────┘
```

### Time and Cost Estimate

| Phase | Duration | Cost (USD) |
|-------|----------|------------|
| FPGA Development | 2-4 weeks | $0 (software is free) |
| PCB Design | 1-2 weeks | $0 (KiCad is free) |
| PCB Manufacturing | 1-2 weeks | $50-150 (3 boards) |
| Component Procurement | 1-2 weeks | $100-200 |
| Assembly | 1 week | $0-50 (tools if needed) |
| Bring-Up & Testing | 1-2 weeks | $0 |
| Software Development | 2-4 weeks | $0 |
| **Total** | **2-4 months** | **$150-400** |

### Required Hardware

**Essential:**
- Xilinx Arty A7-100 FPGA board (~$200-250)
- USB cable for programming
- 5V power supply (usually included)
- Computer (Windows/Linux/Mac)

**For Complete System:**
- Custom PCB (designed, ordered, assembled)
- Electronic components (see BOM)
- Soldering equipment
- Basic test equipment (multimeter, power supply)

---

## Prerequisites

### Knowledge Requirements

**Essential Skills:**
- ✅ Basic electronics (Ohm's law, voltage/current)
- ✅ Programming in C (moderate level)
- ✅ Command-line comfort (file navigation, running commands)
- ✅ Patience and problem-solving attitude

**Helpful But Not Required:**
- HDL experience (VHDL or Verilog/SystemVerilog)
- PCB design experience
- Soldering experience
- FPGA development background
- Embedded systems knowledge

### Software Installation

#### 1. Xilinx Vivado Design Suite

**Version:** 2019.2 or later (2020.1 recommended for this project)

**Download:**
1. Go to [Xilinx Downloads](https://www.xilinx.com/support/download.html)
2. Create free account (required)
3. Download **Vivado ML Edition - WebPACK** (free version)
4. Size: ~35 GB download, 100 GB installed

**Installation (Linux):**
```bash
# Extract installer
tar -xvf Xilinx_Unified_2020.1_*.tar.gz
cd Xilinx_Unified_2020.1_*/

# Run installer
sudo ./xsetup

# Follow GUI installer:
# - Select "Vivado"
# - Edition: "Vivado ML Standard" or "WebPACK"
# - Include: Artix-7 device support
# - Install to: /tools/Xilinx (or your preference)
```

**Installation (Windows):**
1. Run installer executable
2. Follow on-screen instructions
3. Select WebPACK edition
4. Include Artix-7 devices
5. Takes 2-4 hours to install

**Post-Installation:**
```bash
# Add to PATH (Linux)
echo 'source /tools/Xilinx/Vivado/2020.1/settings64.sh' >> ~/.bashrc
source ~/.bashrc

# Verify installation
vivado -version
```

#### 2. Xilinx Vitis (for embedded software)

**Included with Vivado 2019.2+**

If separate installation needed:
```bash
# Usually at same location as Vivado
source /tools/Xilinx/Vitis/2020.1/settings64.sh
```

#### 3. KiCad EDA (for PCB design)

**Version:** 5.x or 6.x

**Installation (Linux):**
```bash
# Ubuntu/Debian
sudo add-apt-repository ppa:kicad/kicad-6.0-releases
sudo apt update
sudo apt install kicad

# Arch Linux
sudo pacman -S kicad kicad-library kicad-library-3d
```

**Installation (Windows/Mac):**
1. Download from [kicad.org](https://www.kicad.org/download/)
2. Run installer
3. Include all libraries (symbols, footprints, 3D models)

#### 4. Serial Terminal Software

**Linux:**
```bash
sudo apt install minicom
# Or
sudo apt install screen
```

**Windows:**
- PuTTY: https://www.putty.org/
- Tera Term: https://ttssh2.osdn.jp/

**Mac:**
```bash
brew install minicom
```

#### 5. Git (Version Control)

```bash
# Linux
sudo apt install git

# Windows
# Download from git-scm.com

# Mac
brew install git
```

---

## Phase 1: FPGA Development

**Goal:** Create and program the FPGA bitstream  
**Duration:** 2-4 weeks (first-time), 3-5 days (experienced)  
**Prerequisites:** Vivado installed, Arty board available

### Step 1.1: Open Existing Project

```bash
# Navigate to project
cd /path/to/oscilloscope_project/oscilloscope-fpga

# Launch Vivado
vivado oscilloscope-fpga.xpr &
```

**First Time Opening:**
- Vivado will auto-upgrade project if needed
- May take 5-10 minutes to index
- All IP cores will be validated

### Step 1.2: Understand Project Structure

**In Vivado GUI:**

1. **Sources Panel** (left side)
   - Design Sources
     - `design_1` (block design)
     - `adc_interface` (custom ADC interface)
     - `design_1_wrapper` (top-level)
   - Constraints
     - `arty.xdc` (pin assignments)

2. **Block Design** (double-click `design_1.bd`)
   - MicroBlaze processor
   - DDR3 memory controller
   - AXI interconnect
   - Peripherals (GPIO, UART, SPI, Ethernet, Timer)
   - Custom `adc_interface` module

### Step 1.3: Review and Customize Design

#### Customize ADC Interface (Optional)

**File:** `sources_1/new/adc_interface.vhd`

```vhdl
-- Current: 1kHz calibration clock
-- Modify CLK_1KHZ_PERIOD to change frequency
constant clk_1khz_period : integer := 10 * 1000;  -- Divide by 10,000

-- For 10kHz: change to 1000
-- For 100Hz: change to 100,000
```

#### Modify Block Design (If Needed)

**Add Custom Logic:**
1. Open block design (`design_1.bd`)
2. Right-click canvas → **Add Module**
3. Select your custom module
4. Connect to AXI bus or GPIO
5. **Validate Design** (F6)
6. **Save** (Ctrl+S)

**Add IP Cores:**
1. Click **+** icon or IP Integrator → **Add IP**
2. Search for IP (e.g., "FIFO", "DMA", "Timer")
3. Double-click to add
4. Configure parameters
5. Connect ports

### Step 1.4: Synthesize Design

**Method 1: GUI**
1. Flow Navigator → **Synthesis → Run Synthesis**
2. Wait 10-15 minutes
3. Review results

**Method 2: TCL Console**
```tcl
# In Vivado TCL console
reset_run synth_1
launch_runs synth_1 -jobs 4
wait_on_run synth_1

# Check status
get_property STATUS [get_runs synth_1]
# Should return: "synth_design Complete!"
```

**Check for Errors:**
- Messages tab → Filter "Error"
- Should be 0 errors
- Warnings are often OK, but review critical ones

### Step 1.5: Implement Design

```tcl
# In Vivado TCL console
launch_runs impl_1 -jobs 4
wait_on_run impl_1

# Or use GUI: Flow Navigator → Implementation → Run Implementation
```

**Duration:** 15-25 minutes

**Check Results:**
```tcl
# Timing summary
open_run impl_1
report_timing_summary -file timing_summary.rpt

# Utilization
report_utilization -file utilization.rpt -hierarchical

# Power
report_power -file power.rpt
```

### Step 1.6: Generate Bitstream

```tcl
# TCL command
launch_runs impl_1 -to_step write_bitstream
wait_on_run impl_1

# Or GUI: Flow Navigator → Generate Bitstream
```

**Output:** `oscilloscope-fpga.runs/impl_1/design_1_wrapper.bit`

### Step 1.7: Program FPGA

**Connect Hardware:**
1. Connect Arty A7-100 via USB (JTAG port)
2. Power on board (LED should light)
3. Wait for enumeration (10 seconds)

**Program via Vivado:**
```tcl
# Open Hardware Manager
open_hw_manager
connect_hw_server
open_hw_target

# Program device
set_property PROGRAM.FILE {./oscilloscope-fpga.runs/impl_1/design_1_wrapper.bit} [get_hw_devices xc7a100t_0]
program_hw_devices [get_hw_devices xc7a100t_0]

# Or use GUI: Flow Navigator → Program and Debug → Open Hardware Manager
```

**Verify Programming:**
- DONE LED lights up (green)
- User LEDs may show activity
- Board is now running FPGA design

### Step 1.8: Export Hardware for Software

**This creates the hardware description for Vitis:**

```tcl
# In Vivado
write_hw_platform -fixed -include_bit -force -file design_1_wrapper.xsa

# Or GUI:
# File → Export → Export Hardware
# - Include bitstream: YES
# - Output: design_1_wrapper.xsa
```

**Save to:** `oscilloscope-fpga/design_1_wrapper.xsa`

---

## Phase 2: PCB Design and Manufacturing

**Goal:** Design, order, and receive custom PCB  
**Duration:** 3-5 weeks total (1 week design, 2-3 weeks manufacturing)  
**Cost:** $50-150 for 3 boards

### Step 2.1: Review Existing Design

**Open KiCad Project:**
```bash
cd /path/to/oscilloscope_project/oscilloscope-circuit
kicad Oscilloscope.pro
```

**Add Required Libraries (IMPORTANT):**

1. **Symbol Libraries:**
   - KiCad → Preferences → Manage Symbol Libraries
   - Add: `Schematic Symbols.lib` (in project folder)
   - Scope: "Project" (not global)

2. **Footprint Libraries:**
   - KiCad → Preferences → Manage Footprint Libraries
   - Add: `Footprints.pretty/` directory
   - Scope: "Project"

### Step 2.2: Understand Schematic

**Hierarchical Design:**

- **Oscilloscope.sch** - Top sheet (interconnections)
  - **ADC.sch** - ADC interface (SPI, power)
  - **Analog Inputs.sch** - Input conditioning, protection
  - **Calibration.sch** - Calibration DAC and reference
  - **PMOD Ports.sch** - FPGA interface connectors
  - **Power Supply.sch** - Regulators (3.3V, 5V, analog)
  - **SPI.sch** - SPI communication bus

**Key Components:**
- High-speed ADC (likely AD9226 or similar)
- Operational amplifiers for signal conditioning
- Voltage regulators
- PMOD connectors (interface to FPGA)
- Protection diodes

### Step 2.3: Modify Design (Optional)

**Common Modifications:**

**Change Input Range:**
1. Open `Analog Inputs.sch`
2. Modify resistor divider values
3. Update op-amp gain
4. Update annotations

**Add Features:**
- Additional input channels
- Different ADC
- Trigger circuit improvements
- Power supply changes

**Always:**
- Run **ERC** (Electrical Rules Check)
- Update BOM
- Test in simulation if possible

### Step 2.4: Review PCB Layout

**Open PCB Editor:**
- From schematic: Tools → Update PCB from Schematic
- Or directly: `Oscilloscope.kicad_pcb`

**Check Layout:**
- **4-layer stackup:** Signal / Ground / Power / Signal
- High-speed traces (ADC → FPGA) are length-matched
- Ground pour on all layers
- Power decoupling caps near ICs
- Proper clearances for high voltage

**Important Considerations:**
- Analog and digital grounds separated, joined at one point
- Bypass capacitors close to IC power pins
- Trace impedance controlled for high-speed signals
- No ground plane splits under critical paths

### Step 2.5: Design Rule Check (DRC)

```
In PCB Editor:
- Inspect → Design Rules Checker
- Run DRC
- Fix all errors
- Review warnings
```

**Common Issues:**
- Trace too close to edge
- Via too close to pad
- Copper clearance violation
- Unconnected nets

### Step 2.6: Generate Manufacturing Files

#### Gerber Files (for fabrication)

```
In PCB Editor:
1. File → Plot
2. Settings:
   - Format: Gerber
   - Layers: All copper, soldermask, silkscreen, edge cuts
   - Options:
     ☑ Use Protel filename extensions
     ☑ Plot footprint values
     ☑ Plot footprint references
3. Plot Format Settings:
   - Coordinate format: 4.6, unit mm
4. Click "Plot"
5. Click "Generate Drill Files"
   - Format: Excellon
   - Units: Millimeters
   - Zeros: Decimal format
6. Generate Drill File
```

**Output Files:**
```
Oscilloscope-F_Cu.gbr        (Top copper)
Oscilloscope-In1_Cu.gbr      (Inner layer 1 - Ground)
Oscilloscope-In2_Cu.gbr      (Inner layer 2 - Power)
Oscilloscope-B_Cu.gbr        (Bottom copper)
Oscilloscope-F_Mask.gbr      (Top soldermask)
Oscilloscope-B_Mask.gbr      (Bottom soldermask)
Oscilloscope-F_SilkS.gbr     (Top silkscreen)
Oscilloscope-B_SilkS.gbr     (Bottom silkscreen)
Oscilloscope-Edge_Cuts.gbr   (Board outline)
Oscilloscope.drl             (Drill file)
```

#### Bill of Materials (BOM)

```
In Schematic:
1. Tools → Generate Bill of Materials
2. Select BOM plugin
3. Or use existing: Oscilloscope.csv / Oscilloscope.xlsx
```

**Review BOM:**
- Check component availability (Digi-Key, Mouser)
- Note lead times
- Verify part numbers
- Check for obsolete parts

### Step 2.7: Order PCBs

#### Recommended Manufacturers

**Budget Option (2-3 week delivery):**
- **JLCPCB** - jlcpcb.com
  - ~$20 for 5 boards
  - Good quality
  - Slow shipping (2-3 weeks)

**Fast Option (1 week delivery):**
- **OSH Park** - oshpark.com
  - ~$10/sq.in for 3 boards
  - High quality (Purple!)
  - Made in USA
  - 10-12 day turnaround

**Professional Option:**
- **PCBWay** - pcbway.com
- **Sunstone Circuits** - sunstone.com

#### Specifications to Order

```
Board Specifications:
- Layers: 4
- Dimensions: ~100mm x 70mm (check your design)
- Thickness: 1.6mm
- Copper weight: 1 oz (35μm)
- Surface finish: ENIG (best) or HASL
- Soldermask: Green (or your choice)
- Silkscreen: White
- Min trace/space: 6/6 mil
- Min hole: 0.3mm
```

#### Upload Files

1. Zip all Gerber files:
```bash
cd gerber_output/
zip oscilloscope_gerbers.zip *.gbr *.drl
```

2. Upload to manufacturer website
3. Review automatically generated preview
4. **CRITICAL:** Check preview carefully:
   - All layers present
   - Board outline correct
   - No missing copper
   - Drill holes aligned

5. Add to cart and order

**Cost:** $20-60 for 3-5 boards + shipping

### Step 2.8: Order Components

**Use BOM file:** `Oscilloscope.xlsx` or `.csv`

**Recommended Suppliers:**
- Digi-Key (digikey.com)
- Mouser (mouser.com)
- Newark/Element14

**Ordering Strategy:**
1. Copy BOM to spreadsheet
2. Search each part on supplier website
3. Check stock and price
4. Order 10% extra of small components (resistors, caps)
5. Order exact quantity for ICs

**Expected Cost:** $100-200 depending on ADC and components chosen

---

## Phase 3: Hardware Assembly

**Goal:** Assemble populated PCB  
**Duration:** 1-2 days  
**Prerequisites:** PCB received, components received

### Step 3.1: Gather Tools and Materials

#### Essential Tools

- **Soldering iron** (temperature controlled, 350°C)
  - Recommendation: Hakko FX-888D or similar
  - Fine tip (0.5mm conical)
- **Solder** (60/40 or 63/37 tin/lead, or lead-free)
  - 0.5mm or 0.8mm diameter
- **Flux** (rosin flux pen or paste)
- **Tweezers** (ESD-safe, fine tip)
- **Magnification**
  - Magnifying glass
  - Or microscope (10x-40x)
- **Multimeter**
- **Wire cutters** (flush cut)
- **Solder wick** (desoldering braid)
- **Isopropyl alcohol** (IPA, 90%+) and brush

#### Optional But Helpful

- Hot air rework station
- Solder paste and squeegee
- Stencil (for solder paste)
- Reflow oven (toaster oven conversion)
- Helping hands / PCB holder
- ESD mat and wrist strap

### Step 3.2: Prepare Workspace

1. **Clean work area**
2. **Good lighting** (LED desk lamp)
3. **ESD protection** (wrist strap grounded)
4. **Organize components:**
   - Sort by type
   - Label bags
   - Print BOM with checkboxes

### Step 3.3: Inspect PCB

**Before soldering:**
- [ ] Check for manufacturing defects
- [ ] Verify copper traces are intact
- [ ] Check board outline and dimensions
- [ ] Test continuity between power pins
- [ ] Check for shorts between power and ground

```bash
# Use multimeter in continuity mode
# Test all power nets for shorts to ground
# Should be open circuit (no beep)
```

### Step 3.4: Assembly Strategy

**Order of Assembly:**

1. ✅ Smallest components first (0402, 0603 resistors/caps)
2. ✅ ICs (ADC, op-amps, regulators)
3. ✅ Larger passives (electrolytic caps, inductors)
4. ✅ Connectors (PMOD, BNC, power)
5. ✅ Through-hole components last

**Reasoning:** Small parts are hard to access after large ones are placed

### Step 3.5: SMT Component Soldering

#### Method 1: Hand Soldering (No Special Equipment)

**For each component:**

1. **Apply flux** to pads
2. **Pre-tin one pad** (add small amount of solder)
3. **Place component** with tweezers
4. **Reheat tinned pad**, press component down
5. **Solder other pad(s)**
6. **Clean with IPA**

**Tips:**
- Work quickly but carefully
- Don't overheat (< 3 seconds per joint)
- Use minimal solder (less is more)
- Inspect with magnifier after each component

#### Method 2: Reflow Soldering (Recommended for Entire Board)

**Equipment needed:**
- Solder paste (SAC305 or similar)
- Stencil (can order with PCB)
- Squeegee (or old credit card)
- Toaster oven or reflow oven

**Process:**

1. **Apply solder paste:**
   - Align stencil over PCB
   - Apply paste with squeegee
   - Remove stencil carefully

2. **Place components:**
   - Use tweezers
   - Components will "stick" to solder paste
   - Check orientation (IC pin 1, electrolytic cap polarity)

3. **Reflow profile:**
```
Temperature Profile:
- Preheat: 150°C for 60-90 seconds
- Soak: 150-180°C for 60-120 seconds
- Reflow: 220-250°C for 30-60 seconds (peak)
- Cool down: Natural cooling
```

4. **Inspect joints:**
   - Shiny, smooth fillets
   - No bridges between pins
   - No cold joints (dull appearance)

### Step 3.6: Through-Hole Components

**Standard soldering:**

1. Insert component from top
2. Bend leads on bottom to hold in place
3. Solder from bottom
4. Trim excess lead
5. Clean flux

**Large connectors:**
- May need more heat
- Use larger tip
- Take time to fully heat pad and pin

### Step 3.7: Post-Assembly Inspection

#### Visual Inspection

**Check for:**
- [ ] Solder bridges between pins
- [ ] Cold solder joints (dull, grainy)
- [ ] Missing components
- [ ] Reversed polarity (electrolytic caps, diodes)
- [ ] IC orientation (pin 1 marker)

**Use microscope/magnifier for:**
- IC pin inspection (all pins soldered)
- Fine pitch components (< 0.5mm)
- BGA components if present

#### Electrical Testing

**Before applying power:**

```bash
# Multimeter in resistance mode

# 1. Check power supply shorts
#    Measure between each power rail and ground
#    Should read > 10kΩ typically (depends on design)

# 2. Check supply rails for continuity
#    Verify 3.3V net is continuous across board
#    Verify 5V net is continuous

# 3. Check critical signals
#    Verify PMOD pins are not shorted

# 4. Check component values (if uncertain)
#    Measure resistors in-circuit (approximate)
```

### Step 3.8: Clean PCB

```bash
# Materials:
- Isopropyl alcohol (IPA), 90%+
- Soft brush (toothbrush)
- Lint-free wipes
- Compressed air (optional)

# Process:
1. Apply IPA liberally with brush
2. Scrub gently to remove flux residue
3. Wipe with lint-free cloth
4. Repeat if needed
5. Dry with compressed air or let air dry
6. Inspect under magnification
```

**Result:** Clean, professional-looking board ready for testing

---

## Phase 4: Hardware Bring-Up

**Goal:** Verify PCB functions correctly  
**Duration:** 2-5 days  
**Prerequisites:** Assembled PCB, test equipment

### Step 4.1: Pre-Power Inspection

**Critical Checklist:**
- [ ] All components soldered correctly
- [ ] No solder bridges
- [ ] Correct component orientations
- [ ] No shorts between power and ground
- [ ] Visual inspection complete
- [ ] PCB cleaned of flux residue

### Step 4.2: First Power-On

**Setup:**
1. **Current-limited power supply** (bench supply)
2. Set voltage to 5V (or board's input voltage)
3. **Set current limit to 100mA** (important!)
4. Connect to board power input
5. Multimeter ready to measure voltages

**Power-On Sequence:**

```bash
# 1. Connect power supply, DO NOT turn on yet
# 2. Set current limit to 100mA
# 3. Set voltage to 5.0V
# 4. Turn on supply

# Observe:
# - Current draw should be < 100mA typically
# - If current limit trips immediately: POWER OFF, check for shorts
# - Normal draw: 50-100mA without FPGA, 200-500mA with FPGA

# 5. Measure voltages:
Measure 5V rail: Should be 5.0V ± 0.1V
Measure 3.3V rail: Should be 3.3V ± 0.05V
Measure analog supply (if separate): Should match design voltage
```

**Troubleshooting First Power:**

| Symptom | Likely Cause | Fix |
|---------|--------------|-----|
| Current limit trips immediately | Short circuit | Find and fix short |
| No voltage on outputs | Regulator not soldered | Check regulator |
| Low voltage (< spec) | Excessive load or regulator damaged | Check components |
| Voltage oscillating | Instability, missing decoupling caps | Add/fix caps |

### Step 4.3: Test Power Supply Subsystem

**Measure all voltage rails:**

```bash
# Multimeter in DC voltage mode

# Expected voltages (your design may vary):
V_IN (input):          5.0V ± 0.1V
V_3V3 (digital):       3.3V ± 0.05V
V_ANALOG (analog):     3.3V or 5.0V ± 0.05V
V_ADC (ADC supply):    Check datasheet

# Test points:
- At regulator output
- At IC power pins (verify voltage arrives)
- At far end of board (check for voltage drop)

# Load testing:
# Increase current limit to 500mA or 1A
# Monitor voltage stability under load
```

**Ripple measurement (optional but good):**
```bash
# Oscilloscope required
# AC couple, 10mV/div
# Measure ripple on each supply rail
# Should be < 50mV peak-to-peak typically
```

### Step 4.4: Connect FPGA via PMOD

**PMOD Connector:**
- Verify pin assignments match FPGA board
- Check continuity from PMOD pins to circuit
- Verify no shorts between adjacent pins

**Connection to Arty:**
1. Power off both boards
2. Connect PMOD cable (use correct port on Arty)
3. Verify pinout: GND on pin 5, 11
4. Power on Arty first
5. Power on oscilloscope board

### Step 4.5: Test SPI Communication

**Using FPGA:**

Create simple SPI test in Vitis (or use ILA in Vivado):

```c
// Simple SPI loopback test in Vitis
#include "xspi.h"

// Initialize SPI
XSpi_Initialize(&spi_instance, SPI_DEVICE_ID);
XSpi_SetOptions(&spi_instance, XSP_MASTER_OPTION);
XSpi_Start(&spi_instance);

// Send test pattern
uint8_t tx_data[4] = {0xAA, 0x55, 0xF0, 0x0F};
uint8_t rx_data[4];

XSpi_Transfer(&spi_instance, tx_data, rx_data, 4);

// Check rx_data with oscilloscope or logic analyzer
```

**Expected:**
- SPI clock toggles (verify with scope)
- MOSI shows data pattern
- MISO responds (ADC dependent)
- CS asserts low during transfer

### Step 4.6: Test ADC

**ADC Power:**
- Verify ADC power pins have correct voltage
- Check reference voltage (if external)

**ADC Configuration:**
```c
// Example for typical ADC
// Check your ADC datasheet for actual commands

// Read ADC ID register
uint8_t cmd_read_id[] = {0x00, 0x00};  // Example command
uint8_t response[2];
XSpi_Transfer(&spi_instance, cmd_read_id, response, 2);

// response should match ADC ID (check datasheet)
xil_printf("ADC ID: 0x%02X\r\n", response[1]);

// Configure ADC for data capture
// Send configuration commands per datasheet
```

**Test analog input:**
1. Apply known voltage to input (use function generator or DC source)
2. Read ADC values
3. Verify correct readings
4. Test multiple voltage levels
5. Check linearity

### Step 4.7: Signal Path Testing

**End-to-end test:**

```
Signal Generator → Oscilloscope Input → ADC → FPGA → UART Output
```

**Test procedure:**
1. Apply 1kHz, 1Vpp sine wave to input
2. Capture samples in FPGA
3. Output to UART or store in memory
4. Verify waveform shape and frequency

**Python script to analyze UART output:**
```python
import serial
import numpy as np
import matplotlib.pyplot as plt

# Open serial port
ser = serial.Serial('/dev/ttyUSB1', 115200)

# Read samples
samples = []
for i in range(1000):
    line = ser.readline().decode('utf-8').strip()
    try:
        value = int(line, 16)  # If hex format
        samples.append(value)
    except:
        pass

# Plot
plt.plot(samples)
plt.xlabel('Sample')
plt.ylabel('ADC Value')
plt.title('Captured Waveform')
plt.savefig('waveform.png')
plt.show()
```

---

## Phase 5: Software Development

**Goal:** Develop and test embedded firmware  
**Duration:** 2-4 weeks  
**Prerequisites:** Hardware working, Vitis installed

### Step 5.1: Set Up Vitis Workspace

```bash
# Create or open existing workspace
cd /path/to/oscilloscope_project/oscilloscope-software

# Launch Vitis
vitis -workspace ./ &

# Or if not in PATH:
/tools/Xilinx/Vitis/2020.1/bin/vitis -workspace ./ &
```

### Step 5.2: Create Platform Project

**If not already present:**

1. **File → New → Platform Project**
2. Name: `oscilloscope_platform`
3. Hardware Specification: Browse to `design_1_wrapper.xsa`
4. Operating System: `standalone` (bare-metal)
5. Processor: `microblaze_0`
6. Click **Finish**

**Platform will generate:**
- Board support package (BSP)
- Driver libraries
- Memory map
- Interrupt handlers

### Step 5.3: Create Application Project

**If starting from scratch:**

1. **File → New → Application Project**
2. Name: `oscilloscope_app`
3. Platform: Select `oscilloscope_platform`
4. Template: `Empty Application` or `Hello World`
5. Click **Finish**

### Step 5.4: Software Architecture

**Recommended structure:**

```
oscilloscope_app/
├── src/
│   ├── main.c              # Main application loop
│   ├── adc_driver.c        # ADC communication
│   ├── adc_driver.h
│   ├── capture.c           # Data capture state machine
│   ├── capture.h
│   ├── trigger.c           # Trigger detection
│   ├── trigger.h
│   ├── display.c           # Output formatting
│   ├── display.h
│   └── config.h            # Configuration constants
└── README.md
```

### Step 5.5: Implement Core Functionality

#### Main Loop Structure

```c
// main.c
#include <stdio.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "adc_driver.h"
#include "capture.h"
#include "trigger.h"

// System configuration
#define ADC_SAMPLE_RATE     100000000  // 100 MSPS
#define CAPTURE_DEPTH       4096       // Number of samples
#define TRIGGER_LEVEL       2048       // Mid-scale

int main() {
    int status;
    
    // Initialize hardware
    xil_printf("=== Digital Oscilloscope ===\r\n");
    
    // Initialize ADC
    status = adc_init();
    if (status != XST_SUCCESS) {
        xil_printf("ADC initialization failed!\r\n");
        return XST_FAILURE;
    }
    
    // Initialize capture buffer
    capture_init(CAPTURE_DEPTH);
    
    // Configure trigger
    trigger_set_level(TRIGGER_LEVEL);
    trigger_set_edge(TRIGGER_RISING);
    
    xil_printf("Initialization complete. Waiting for trigger...\r\n");
    
    // Main loop
    while (1) {
        // Wait for trigger
        if (trigger_detected()) {
            // Capture samples
            capture_arm();
            
            // Wait for capture complete
            while (!capture_complete()) {
                // Could add timeout here
            }
            
            // Process and display data
            uint16_t *samples = capture_get_buffer();
            display_waveform(samples, CAPTURE_DEPTH);
            
            // Reset for next capture
            trigger_reset();
        }
        
        // Handle user input (commands over UART)
        if (uart_data_available()) {
            char cmd = uart_getchar();
            handle_command(cmd);
        }
    }
    
    return 0;
}
```

#### ADC Driver Implementation

```c
// adc_driver.c
#include "adc_driver.h"
#include "xspi.h"
#include "xparameters.h"

static XSpi spi_instance;

int adc_init(void) {
    int status;
    
    // Initialize SPI driver
    status = XSpi_Initialize(&spi_instance, XPAR_SPI_0_DEVICE_ID);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    
    // Set options
    XSpi_SetOptions(&spi_instance, 
                    XSP_MASTER_OPTION | 
                    XSP_CLK_ACTIVE_LOW_OPTION);
    
    // Start SPI
    XSpi_Start(&spi_instance);
    
    // Configure ADC (device-specific)
    // Send configuration commands per datasheet
    adc_configure();
    
    return XST_SUCCESS;
}

uint16_t adc_read_sample(void) {
    uint8_t tx_buf[2] = {0x00, 0x00};  // Read command
    uint8_t rx_buf[2];
    
    XSpi_Transfer(&spi_instance, tx_buf, rx_buf, 2);
    
    // Combine bytes (MSB first typically)
    uint16_t sample = (rx_buf[0] << 8) | rx_buf[1];
    
    return sample;
}

void adc_configure(void) {
    // Device-specific configuration
    // Example: Set ADC to continuous conversion mode
    uint8_t config_cmd[] = {ADC_CONFIG_REG, ADC_CONTINUOUS_MODE};
    XSpi_Transfer(&spi_instance, config_cmd, NULL, 2);
}
```

#### Capture Engine

```c
// capture.c
#include "capture.h"
#include <stdlib.h>

static uint16_t *capture_buffer;
static uint32_t capture_size;
static uint32_t capture_index;
static bool capturing;

void capture_init(uint32_t size) {
    capture_size = size;
    capture_buffer = (uint16_t*)malloc(size * sizeof(uint16_t));
    capture_index = 0;
    capturing = false;
}

void capture_arm(void) {
    capture_index = 0;
    capturing = true;
}

void capture_sample(uint16_t sample) {
    if (capturing && capture_index < capture_size) {
        capture_buffer[capture_index++] = sample;
        
        if (capture_index >= capture_size) {
            capturing = false;
        }
    }
}

bool capture_complete(void) {
    return !capturing;
}

uint16_t* capture_get_buffer(void) {
    return capture_buffer;
}
```

### Step 5.6: Build and Test

**Build Project:**
```bash
# In Vitis:
Project → Build Project

# Or right-click project → Build Project

# Check console for errors
```

**Program FPGA and Run:**
```bash
# 1. Program FPGA (if not already done)
Xilinx → Program FPGA

# 2. Run application
Run → Run Configurations
Select configuration → Run

# Or use Debug for step-through debugging
```

**Monitor Output:**
```bash
# Open serial terminal
minicom -D /dev/ttyUSB1 -b 115200

# Or in Vitis:
Window → Show View → Vitis Serial Terminal
Click + to add connection (115200 baud)
```

### Step 5.7: Implement Advanced Features

**Features to add:**

1. **Trigger modes:**
   - Rising edge
   - Falling edge
   - Both edges
   - Level trigger

2. **Capture modes:**
   - Single shot
   - Continuous/auto
   - Normal (wait for trigger)

3. **Signal processing:**
   - FFT (frequency domain)
   - RMS calculation
   - Peak detection
   - Frequency measurement

4. **User interface:**
   - UART command interpreter
   - Settings adjustment
   - Waveform export

5. **Data storage:**
   - Save to DDR3 memory
   - Export via Ethernet
   - Log to SD card (if available)

---

## Phase 6: System Integration and Testing

**Goal:** Complete, tested oscilloscope system  
**Duration:** 1-2 weeks

### Step 6.1: System-Level Testing

**Test Plan:**

| Test ID | Description | Expected Result | Status |
|---------|-------------|-----------------|--------|
| SYS-001 | Power-on, all rails correct | 5V, 3.3V within spec | [ ] |
| SYS-002 | FPGA programs successfully | DONE LED on | [ ] |
| SYS-003 | UART communication works | Boot message received | [ ] |
| SYS-004 | ADC responds to SPI | ID register read | [ ] |
| SYS-005 | Capture DC input | Correct ADC value | [ ] |
| SYS-006 | Capture 1kHz sine wave | Correct frequency | [ ] |
| SYS-007 | Trigger on rising edge | Capture aligned | [ ] |
| SYS-008 | Sample rate accuracy | Within 1% | [ ] |
| SYS-009 | Input range correct | ±5V captured | [ ] |
| SYS-010 | Continuous operation | No crashes, 1hr+ | [ ] |

### Step 6.2: Performance Characterization

**Measurements to take:**

```python
# Use Python scripts to automate testing

import serial
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal, fft

# 1. Sample Rate Accuracy
# Apply known frequency, measure captured frequency

# 2. Bandwidth
# Sweep frequency, measure -3dB point

# 3. Noise Floor
# Short input, measure noise

# 4. Dynamic Range
# Measure SNR at various input levels

# 5. Linearity
# Apply various DC voltages, check ADC response
```

### Step 6.3: Calibration

**Offset Calibration:**
```c
// Short input to ground
// Measure average value
int16_t offset_calibration(void) {
    int32_t sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += adc_read_sample();
    }
    return sum / 1000;
}

// Apply offset in real-time
uint16_t calibrated_sample = raw_sample - offset;
```

**Gain Calibration:**
```c
// Apply known voltage (e.g., 1.000V from calibrated source)
// Measure ADC value
float gain_calibration(float known_voltage) {
    float measured_voltage = adc_read_sample() * ADC_LSB;
    return known_voltage / measured_voltage;
}
```

### Step 6.4: Create User Documentation

**Minimum documentation:**
- Operating instructions
- Specifications (bandwidth, sample rate, etc.)
- Calibration procedure
- Troubleshooting guide
- Known limitations

### Step 6.5: Final Validation

**Before declaring "complete":**

✅ All tests pass  
✅ Performance meets specifications  
✅ Calibration performed  
✅ Documentation written  
✅ Code committed to version control  
✅ PCB files archived  
✅ Hardware photos taken  

---

## Troubleshooting Guide

### FPGA Issues

#### Vivado Won't Open Project
- Check Vivado version compatibility
- Try: Tools → Report → Report IP Status → Upgrade IP
- Regenerate output products

#### Synthesis Fails
- Check Messages tab for first error
- Common: Missing IP, incorrect version
- Solution: Regenerate IP, update to compatible version

#### Implementation Fails - Timing
- Relax timing constraints (temporary)
- Add pipeline stages
- Reduce clock frequency
- Check for combinational loops

#### Bitstream Won't Program
- Check JTAG connection
- Verify board power
- Try: Cable → Auto Connect
- Check USB drivers (Windows)

### Hardware Issues

#### No Power Output
- Verify input voltage
- Check regulator solder joints
- Measure regulator input
- Replace regulator if damaged

#### ADC Not Responding
- Check power at ADC pins
- Verify SPI connections (continuity)
- Check CS, CLK, MOSI signals with scope
- Review ADC initialization code

#### Noise/Instability
- Check ground plane integrity
- Add decoupling caps if missing
- Separate analog and digital grounds
- Check for ground loops

#### Incorrect Readings
- Calibrate offset and gain
- Check reference voltage
- Verify input conditioning circuit
- Test with known signal source

### Software Issues

#### Application Won't Build
- Clean and rebuild
- Check BSP is up to date
- Regenerate platform if hardware changed
- Verify all source files added to project

#### No UART Output
- Check baud rate (115200)
- Verify UART cable connected
- Check COM port number
- Try different terminal program

#### System Crashes/Hangs
- Check for stack overflow (increase stack size)
- Check for infinite loops
- Add watchdog timer
- Use debugger to find crash location

#### Data Corruption
- Check memory alignment
- Verify buffer sizes
- Check for array out-of-bounds
- Enable cache coherency if using DMA

---

## Performance Optimization

### Improving Sample Rate

**1. Optimize SPI Clock:**
```c
// Increase SPI clock speed to maximum supported by ADC
XSpi_SetOptions(&spi_instance, XSP_MASTER_OPTION);
// May need to adjust in block design
```

**2. Use DMA for Data Transfer:**
- Add AXI DMA IP to block design
- Configure to transfer ADC samples to DDR3
- Eliminates CPU overhead

**3. FPGA-Based Capture:**
- Move capture logic to FPGA (HDL)
- CPU only reads after capture complete
- Achieves maximum ADC sample rate

### Reducing Noise

**1. PCB Layout:**
- Minimize trace lengths (analog)
- Proper grounding
- Shielding if necessary

**2. Filtering:**
- Add anti-alias filter before ADC
- Digital filtering in FPGA or software

**3. Power Supply:**
- Use LDO regulators for analog supply
- Add ferrite beads
- Increase decoupling capacitors

### Improving Accuracy

**1. Better ADC:**
- Higher resolution (14-bit, 16-bit)
- Lower noise
- Better linearity

**2. Precision Reference:**
- Use precision voltage reference IC
- Temperature-compensated

**3. Calibration:**
- Implement full 2-point calibration
- Store calibration in EEPROM
- Periodic auto-calibration

---

## Next Steps and Enhancements

### Hardware Enhancements
- [ ] Multiple input channels (2-4)
- [ ] Programmable gain amplifier (PGA)
- [ ] AC/DC coupling switch
- [ ] Trigger output (for other instruments)
- [ ] Display interface (SPI/HDMI)

### Software Features
- [ ] GUI application (Qt/Python)
- [ ] Ethernet/TCP data streaming
- [ ] Waveform math functions
- [ ] Protocol decoders (I2C, SPI, UART)
- [ ] Automated measurements
- [ ] FFT/frequency analysis
- [ ] Waveform recording/playback

### Advanced Features
- [ ] Arbitrary waveform generator (AWG)
- [ ] Logic analyzer functionality
- [ ] Mixed-signal analysis
- [ ] Long-term data logging
- [ ] Remote web interface
- [ ] SCPI command compatibility

---

## Conclusion

You now have a complete guide to building a professional digital oscilloscope from scratch!

**Achievement Unlocked:**
- ✅ FPGA design and programming
- ✅ PCB design and manufacturing
- ✅ SMT assembly
- ✅ High-speed analog circuit design
- ✅ Embedded software development
- ✅ System integration and testing

**Skills Gained:**
- Digital signal processing
- Hardware-software co-design
- Professional debugging techniques
- Complete product development cycle

**What You've Built:**
A fully functional, custom digital oscilloscope that demonstrates real-world embedded systems engineering.

---

## Additional Resources

### Books
- "Architecting High-Performance Embedded Systems" by Jim Ledin
- "The Art of Electronics" by Horowitz & Hill
- "High-Speed Digital Design" by Howard Johnson

### Websites
- Xilinx Forums: forums.xilinx.com
- EEVblog Forum: eevblog.com/forum
- All About Circuits: allaboutcircuits.com
- Stack Exchange - Electrical Engineering

### Tools
- LTspice (circuit simulation)
- KiCad tutorials: kicad.org/help/tutorials
- Vivado tutorials: xilinx.com/training

---

**Congratulations on completing this advanced project!**

You've gained invaluable experience in modern embedded systems design. This oscilloscope can serve as a foundation for even more advanced projects.

**Share your success:**
- Document your build
- Post on forums/Reddit
- Help others who attempt this project
- Consider making it open-source

**Happy building!** 🎉🔬⚡
