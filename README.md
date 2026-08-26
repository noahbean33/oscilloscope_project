# Digital Oscilloscope Project

Complete project files for the digital oscilloscope system.

## Project Overview

This is a complete high-performance digital oscilloscope implementation featuring:
- **High-speed analog data acquisition** using ADC
- **FPGA-based signal processing** with Xilinx Artix-7
- **Real-time waveform capture and display**
- **Custom PCB design** with professional circuit layout
- **Embedded firmware** running on MicroBlaze soft processor

## Project Structure

```
oscilloscope_project/
├── oscilloscope-fpga/         # Xilinx Vivado FPGA project (Chapter 8 version)
│   ├── oscilloscope-fpga.xpr  # Main Vivado project file
│   ├── oscilloscope-fpga.srcs/# VHDL source files and constraints
│   └── oscilloscope-fpga.cache/# Vivado build cache
│
├── oscilloscope-software/     # Xilinx Vitis C/C++ software project (Chapter 8 version)
│   ├── oscilloscope-software/ # Application source code
│   └── oscilloscope-software_system/ # System configuration
│
├── oscilloscope-circuit/      # KiCad PCB design project (Chapter 6)
│   ├── Oscilloscope.sch       # Main schematic (hierarchical)
│   ├── Oscilloscope.kicad_pcb # PCB layout
│   ├── *.sch                  # Hierarchical schematic sheets
│   ├── Schematic Symbols.lib  # Custom schematic symbols
│   ├── Footprints.pretty/     # Custom PCB footprints
│   └── Oscilloscope.xlsx      # Bill of materials
│
├── set_drive_letter.bat       # Windows helper script for path length limits
└── README.md                  # This file
```

## Components

### 1. FPGA Project (`oscilloscope-fpga/`)
**Purpose**: Implements high-speed digital logic for ADC interface and data processing

**Key Features**:
- ADC interface logic (SPI communication)
- High-speed data capture buffer
- MicroBlaze soft processor integration
- AXI bus interconnect
- GPIO for status and control
- UART for debugging

**Tools Required**:
- Xilinx Vivado Design Suite (2019.2 or later recommended)
- Xilinx Artix-7 device support

**Opening the Project**:
1. Launch Xilinx Vivado
2. Open Project → Navigate to `oscilloscope-fpga/oscilloscope-fpga.xpr`
3. Generate bitstream or modify design as needed

### 2. Software Project (`oscilloscope-software/`)
**Purpose**: Embedded C firmware for MicroBlaze processor

**Key Features**:
- ADC configuration and control
- Data acquisition and buffering
- Waveform processing algorithms
- User interface logic
- Communication protocols

**Tools Required**:
- Xilinx Vitis (formerly SDK)
- Compatible with Vivado version

**Opening the Project**:
1. Launch Xilinx Vitis
2. Set workspace to `oscilloscope-software/`
3. Import existing application project
4. Build and debug on hardware

### 3. Circuit Design (`oscilloscope-circuit/`)
**Purpose**: Professional PCB schematic and layout

**Schematic Hierarchy**:
- `Oscilloscope.sch` - Top-level sheet
- `ADC.sch` - Analog-to-digital converter circuit
- `Analog Inputs.sch` - Input conditioning and protection
- `Calibration.sch` - Calibration circuitry
- `PMOD Ports.sch` - FPGA interface connectors
- `Power Supply.sch` - Power regulation circuits
- `SPI.sch` - SPI communication interface

**Key Features**:
- High-speed analog front-end
- Multi-layer PCB design
- Proper signal integrity considerations
- Power supply regulation
- FPGA interface via PMOD connectors

**Tools Required**:
- KiCad EDA 5.0 or later

**Opening the Project**:
1. Launch KiCad
2. Open Project → Navigate to `oscilloscope-circuit/Oscilloscope.pro`
3. **Important**: Add custom libraries first:
   - Open KiCad project manager
   - Go to *Preferences → Manage Symbol Libraries*
   - Add `Schematic Symbols.lib` as a project library
   - Go to *Preferences → Manage Footprint Libraries*
   - Add `Footprints.pretty/` directory as a project library
4. Open schematic or PCB editor

## Getting Started

### Prerequisites

**Hardware**:
- Xilinx Arty A7-100 development board
- Custom oscilloscope PCB (optional - can be ordered from Gerber files)
- USB cable for FPGA programming
- Power supply (5V for Arty board)

**Software**:
- Xilinx Vivado Design Suite (free WebPACK edition supports Artix-7)
- Xilinx Vitis Unified Software Platform
- KiCad EDA (for viewing/modifying circuit design)

### Build and Run Instructions

#### Step 1: Build FPGA Design
```bash
# Open oscilloscope-fpga.xpr in Vivado
# In Vivado TCL Console or GUI:
# 1. Run Synthesis
# 2. Run Implementation
# 3. Generate Bitstream
```

#### Step 2: Export Hardware Platform
```bash
# In Vivado, after successful bitstream generation:
# File → Export → Export Hardware
# Include bitstream
# Export to a location accessible by Vitis
```

#### Step 3: Build Software Application
```bash
# Open Vitis and import the software project
# 1. Set hardware platform (exported from Vivado)
# 2. Build application (hammer icon or Project → Build All)
# 3. Connect Arty board via USB
# 4. Program FPGA and run application
```

#### Step 4: Program and Test
```bash
# In Vitis:
# Xilinx → Program FPGA (loads bitstream)
# Run → Debug or Run (launches firmware)
# Monitor via UART terminal (115200 baud, 8N1)
```

### Manufacturing PCB (Optional)

If you want to build the custom oscilloscope circuit:

1. **Generate Manufacturing Files**:
   - Open `Oscilloscope.kicad_pcb` in KiCad
   - File → Plot → Generate Gerbers
   - File → Fabrication Outputs → Generate Drill Files

2. **Order PCB**:
   - Upload Gerber files to PCB manufacturer (OSH Park, JLCPCB, PCBWay, etc.)
   - Recommended specs: 4-layer board, 1.6mm thickness, ENIG finish

3. **Order Components**:
   - Use `Oscilloscope.xlsx` or `Oscilloscope.csv` for BOM
   - Purchase from Digi-Key, Mouser, or other electronics distributors

4. **Assembly**:
   - Follow Chapter 7 guidelines for component placement
   - Use reflow soldering for SMT components
   - Hand solder through-hole components

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Oscilloscope System                      │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  Analog Input → Front-End Conditioning → ADC → FPGA        │
│                                                ↓            │
│                                         Data Capture        │
│                                         & Processing        │
│                                                ↓            │
│                                        MicroBlaze CPU       │
│                                                ↓            │
│                                    Display/Communication    │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Signal Flow:
1. **Analog Input** - External signal via BNC/scope probe
2. **Conditioning** - Amplification, attenuation, filtering
3. **ADC** - High-speed analog-to-digital conversion
4. **FPGA Logic** - Parallel data capture and buffering
5. **MicroBlaze** - Signal processing and control
6. **Output** - Display via UART/interface

## Configuration

### FPGA Clock Configuration
- System clock: Typically 100 MHz from Arty board
- ADC sampling clock: Configurable based on requirements
- Check constraints file (`.xdc`) in `oscilloscope-fpga.srcs/constrs_1/`

### Software Configuration
- UART baud rate: 115200 (default)
- ADC sample rate: Configured in firmware
- Buffer sizes: Adjustable in source code

## Troubleshooting

### Vivado Path Length Issues (Windows)
- Use `set_drive_letter.bat` to create shorter paths
- Edit script to match your installation location
- Maps directory to drive letter Z:

### Library Not Found (KiCad)
- Ensure custom libraries are added to project
- Check `sym-lib-table` and `fp-lib-table` files
- Re-add libraries via Preferences if needed

### FPGA Programming Fails
- Check USB cable connection
- Verify Arty board is powered
- Install Xilinx cable drivers
- Check Device Manager (Windows) or lsusb (Linux)

### Software Build Errors
- Ensure hardware platform matches FPGA design
- Regenerate BSP (Board Support Package)
- Clean and rebuild project