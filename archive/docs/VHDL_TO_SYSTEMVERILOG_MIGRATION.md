# VHDL to SystemVerilog Migration Plan

**Project:** Digital Oscilloscope FPGA Design  
**Target Device:** Xilinx Artix-7 (Arty A7-100)  
**Estimated Time:** 4-6 hours  
**Difficulty:** 2/10 (Easy)

---

## Table of Contents

1. [Migration Overview](#migration-overview)
2. [Pre-Migration Checklist](#pre-migration-checklist)
3. [Files to Translate](#files-to-translate)
4. [Translation Reference Guide](#translation-reference-guide)
5. [Step-by-Step Migration Process](#step-by-step-migration-process)
6. [Testing and Validation](#testing-and-validation)
7. [Troubleshooting](#troubleshooting)
8. [Rollback Plan](#rollback-plan)

---

## Migration Overview

### Scope

This migration converts custom VHDL design files to SystemVerilog while maintaining all Xilinx IP blocks unchanged. The block design (`.bd` file) and all IP cores remain in their original form.

### Files Affected

**VHDL Files to Convert:**
- `oscilloscope-fpga/oscilloscope-fpga.srcs/sources_1/new/adc_interface.vhd` (51 lines)
- `oscilloscope-fpga/oscilloscope-fpga.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.vhd` (462 lines)

**Files Unchanged:**
- Block design: `design_1.bd`
- All IP cores (40+ modules)
- Constraints: `arty.xdc`
- Software project (Vitis C code)

### Why This Is Easy

✅ Xilinx Vivado natively supports mixed VHDL/SystemVerilog  
✅ Only 2 simple files to convert  
✅ No complex VHDL features (packages, generics, configurations)  
✅ Xilinx primitives work in both languages  
✅ Block design automatically adapts to language choice

---

## Pre-Migration Checklist

### Before You Start

- [ ] **Backup the entire project** (copy `oscilloscope_project/` folder)
- [ ] **Verify current project builds successfully** in Vivado
- [ ] **Commit to version control** if using Git
- [ ] **Install Xilinx Vivado 2019.2+** (supports SystemVerilog)
- [ ] **Read this entire document** before starting
- [ ] **Allocate 4-6 hours** for migration and testing

### Required Knowledge

- Basic SystemVerilog syntax
- Familiarity with Vivado GUI
- Understanding of FPGA build flow (Synthesis → Implementation → Bitstream)

### Environment Setup

```bash
# Create backup
cd /path/to/oscilloscope_project
cp -r ../oscilloscope_project ../oscilloscope_project_backup

# Create SystemVerilog source directory
mkdir -p oscilloscope-fpga/oscilloscope-fpga.srcs/sources_1/systemverilog
```

---

## Files to Translate

### File 1: `adc_interface.vhd`

**Purpose:** ADC encoder interface with clock divider  
**Complexity:** Low  
**Lines:** 51  
**Est. Time:** 30 minutes

**Key Features:**
- Simple clocked process (clock divider)
- Xilinx OBUF and OBUFDS primitives
- Basic integer counter
- No generics or complex types

### File 2: `design_1_wrapper.vhd`

**Purpose:** Top-level wrapper for block design  
**Complexity:** Low-Medium  
**Lines:** 462 (mostly repetitive)  
**Est. Time:** 2 hours (or 15 min if auto-generated)

**Key Features:**
- 59 port declarations
- Block design component instantiation
- 17 IOBUF primitives for bidirectional I/O
- Signal routing between top-level and internal components

**Note:** This file can be **auto-generated** by Vivado in SystemVerilog.

---

## Translation Reference Guide

### VHDL to SystemVerilog Syntax Mapping

#### Basic Constructs

| VHDL | SystemVerilog | Notes |
|------|---------------|-------|
| `entity ... is` | `module` | Module declaration |
| `architecture ... of ... is` | (implicit in module) | No separate architecture |
| `end entity;` | `endmodule` | End of module |
| `std_logic` | `logic` | Single-bit signal |
| `std_logic_vector(N downto 0)` | `logic [N:0]` | Bit vector |
| `signal sig : type;` | `logic sig;` | Internal signal |
| `variable var : type;` | `logic var;` (or just use in always) | Local variable |
| `constant C : integer := 5;` | `localparam int C = 5;` | Constant |
| `port (a : in std_logic)` | `input logic a` | Input port |
| `port (b : out std_logic)` | `output logic b` | Output port |
| `port (c : inout std_logic)` | `inout logic c` | Bidirectional port |

#### Operators

| VHDL | SystemVerilog |
|------|---------------|
| `NOT a` | `~a` |
| `a AND b` | `a & b` |
| `a OR b` | `a \| b` |
| `a XOR b` | `a ^ b` |
| `a <= b` | `a <= b` (non-blocking) |
| `a := b` | `a = b` (blocking, in always) |

#### Process/Always Blocks

**VHDL Process:**
```vhdl
process(clk) is
  variable count : integer := 0;
begin
  if rising_edge(clk) then
    count := count + 1;
    output <= count;
  end if;
end process;
```

**SystemVerilog Equivalent:**
```systemverilog
int count = 0;

always_ff @(posedge clk) begin
  count = count + 1;  // or count <= count + 1;
  output <= count;
end
```

#### Component Instantiation

**VHDL:**
```vhdl
component_name : component_type
  generic map (
    PARAM => VALUE
  )
  port map (
    port1 => signal1,
    port2 => signal2
  );
```

**SystemVerilog:**
```systemverilog
component_type #(
  .PARAM(VALUE)
) component_name (
  .port1(signal1),
  .port2(signal2)
);
```

---

## Step-by-Step Migration Process

### Phase 1: Preparation (30 minutes)

#### Step 1.1: Create Backup
```bash
cd /path/to/oscilloscope_project
tar -czf ../oscilloscope_backup_$(date +%Y%m%d).tar.gz .
```

#### Step 1.2: Open Vivado Project
```bash
# Launch Vivado
vivado oscilloscope-fpga/oscilloscope-fpga.xpr &
```

#### Step 1.3: Verify Current Build
1. In Vivado: **Flow Navigator → Run Synthesis**
2. Wait for completion (10-15 minutes)
3. Confirm: "Synthesis completed successfully"
4. **Do NOT proceed if synthesis fails**

#### Step 1.4: Document Current Configuration
Take screenshots of:
- Project settings (Language preferences)
- Hierarchy window
- IP integrator block design
- Constraints (XDC file)

---

### Phase 2: Translate `adc_interface.vhd` (1 hour)

#### Step 2.1: Create SystemVerilog File

Create file: `oscilloscope-fpga/oscilloscope-fpga.srcs/sources_1/systemverilog/adc_interface.sv`

**Complete SystemVerilog Translation:**

```systemverilog
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Module Name: adc_interface
// Description: ADC encoder interface with 1kHz clock output
// 
// Ports:
//   adc_enc      - Input ADC encode clock
//   enc_p        - Output differential encoder positive
//   enc_n        - Output differential encoder negative
//   clk_1khz_out - Output 1kHz calibration clock
//////////////////////////////////////////////////////////////////////////////////

module adc_interface (
    input  logic adc_enc,
    output logic enc_p,
    output logic enc_n,
    output logic clk_1khz_out
);

    // Internal signals
    logic clk_1khz = 1'b0;
    
    // Clock divider counter
    int count = 0;
    localparam int CLK_1KHZ_PERIOD = 10 * 1000;  // Divide by 10,000

    // Clock divider process
    // Generates 1kHz clock from ADC encode clock
    always_ff @(posedge adc_enc) begin
        count <= count + 1;
        
        if (count >= (CLK_1KHZ_PERIOD / 2)) begin
            clk_1khz <= ~clk_1khz;
            count <= 0;
        end
    end
    
    // Output buffer for 1kHz clock
    // Uses LVCMOS33 I/O standard
    OBUF #(
        .IOSTANDARD("LVCMOS33")
    ) CAL_1KHZ_OBUF (
        .I(clk_1khz),
        .O(clk_1khz_out)
    );

    // Differential output buffer for ADC encoder
    // Uses TMDS_33 differential I/O standard
    OBUFDS #(
        .IOSTANDARD("TMDS_33")
    ) ADC_ENC_OBUFDS (
        .I(adc_enc),
        .O(enc_p),
        .OB(enc_n)
    );

endmodule
```

#### Step 2.2: Add to Vivado Project

1. **In Vivado GUI:**
   - Right-click "Design Sources" → **Add Sources**
   - Select "Add or create design sources"
   - Click **Create File**
   - File type: **SystemVerilog**
   - File name: `adc_interface`
   - Location: Select the `systemverilog` directory
   - **Or** add existing file if you created it manually

2. **Copy the code above** into the file

3. **Save** (Ctrl+S)

#### Step 2.3: Update Block Design

1. **Open block design:**
   - Design Sources → design_1 → design_1.bd (double-click)

2. **Right-click on `adc_interface_0` block** (if it exists)
   - Select **Delete**
   - Confirm deletion

3. **Add SystemVerilog version:**
   - Click **+** icon or right-click canvas → **Add Module**
   - Select `adc_interface` (SystemVerilog version)
   - Click **OK**

4. **Reconnect ports** (if necessary):
   - Connect `adc_enc` input
   - Connect `enc_p`, `enc_n`, `clk_1khz_out` outputs
   - Match original connections from deleted block

5. **Validate design:**
   - Tools → **Validate Design** (F6)
   - Confirm no errors

6. **Save block design** (Ctrl+S)

#### Step 2.4: Remove Old VHDL File

1. In Sources panel, find `adc_interface.vhd`
2. Right-click → **Remove File from Project**
3. **Do NOT delete** from disk yet (keep as backup)
4. Alternatively: Move to `_backup` folder

---

### Phase 3: Translate `design_1_wrapper.vhd` (2 hours)

#### Option A: Auto-Generate (RECOMMENDED - 15 minutes)

**This is the easiest and safest approach.**

##### Step 3A.1: Delete Current Wrapper

1. In Sources panel, locate `design_1_wrapper.vhd`
2. Right-click → **Remove File from Project**
3. Move file to backup location

##### Step 3A.2: Generate SystemVerilog Wrapper

1. **In Vivado, open block design** (`design_1.bd`)
2. In "Sources" panel, right-click on **design_1.bd**
3. Select **Create HDL Wrapper...**
4. Dialog appears: "Create HDL Wrapper"
5. **Important:** Select **"Let Vivado manage wrapper and auto-update"**
6. **Select Language:** SystemVerilog
7. Click **OK**

Vivado will generate: `design_1_wrapper.sv`

##### Step 3A.3: Verify Generated File

1. Open `design_1_wrapper.sv` in Vivado
2. Verify all ports are present
3. Check IOBUF instantiations
4. Confirm proper connections

**Generated file location:**
```
oscilloscope-fpga/oscilloscope-fpga.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.sv
```

---

#### Option B: Manual Translation (2 hours)

**Only use if auto-generation fails or you want full control.**

##### Step 3B.1: Create SystemVerilog Wrapper

Create file: `design_1_wrapper.sv`

**Template (abbreviated - full version in next section):**

```systemverilog
`timescale 1ns / 1ps

module design_1_wrapper (
    // Clock and Reset
    input  logic        sys_clock,
    input  logic        reset,
    output logic        clk_1khz_out,
    
    // DDR3 SDRAM
    output logic [13:0] ddr3_sdram_addr,
    output logic [2:0]  ddr3_sdram_ba,
    output logic        ddr3_sdram_cas_n,
    output logic [0:0]  ddr3_sdram_ck_n,
    output logic [0:0]  ddr3_sdram_ck_p,
    output logic [0:0]  ddr3_sdram_cke,
    output logic [0:0]  ddr3_sdram_cs_n,
    output logic [1:0]  ddr3_sdram_dm,
    inout  logic [15:0] ddr3_sdram_dq,
    inout  logic [1:0]  ddr3_sdram_dqs_n,
    inout  logic [1:0]  ddr3_sdram_dqs_p,
    output logic [0:0]  ddr3_sdram_odt,
    output logic        ddr3_sdram_ras_n,
    output logic        ddr3_sdram_reset_n,
    output logic        ddr3_sdram_we_n,
    
    // Inputs
    input  logic [3:0]  dip_switches_4bits_tri_i,
    input  logic [3:0]  push_buttons_4bits_tri_i,
    input  logic        usb_uart_rxd,
    
    // Ethernet MII
    input  logic        eth_mii_col,
    input  logic        eth_mii_crs,
    output logic        eth_mii_rst_n,
    input  logic        eth_mii_rx_clk,
    input  logic        eth_mii_rx_dv,
    input  logic        eth_mii_rx_er,
    input  logic [3:0]  eth_mii_rxd,
    input  logic        eth_mii_tx_clk,
    output logic        eth_mii_tx_en,
    output logic [3:0]  eth_mii_txd,
    output logic        eth_ref_clk,
    
    // Bidirectional I/O
    inout  logic        eth_mdio_mdc_mdio_io,
    output logic        eth_mdio_mdc_mdc,
    inout  logic [3:0]  led_4bits_tri_io,
    inout  logic [11:0] rgb_led_tri_io,
    inout  logic        spi_io0_io,
    inout  logic        spi_io1_io,
    inout  logic        spi_sck_io,
    inout  logic        spi_ss_io,
    
    // Encoder outputs
    output logic        enc_n,
    output logic        enc_p,
    output logic        usb_uart_txd
);

    // Internal signals for tristate buffers
    logic eth_mdio_mdc_mdio_i, eth_mdio_mdc_mdio_o, eth_mdio_mdc_mdio_t;
    
    // LED signals (4 bits)
    logic [3:0] led_4bits_tri_i, led_4bits_tri_o, led_4bits_tri_t;
    
    // RGB LED signals (12 bits)
    logic [11:0] rgb_led_tri_i, rgb_led_tri_o, rgb_led_tri_t;
    
    // SPI signals
    logic spi_io0_i, spi_io0_o, spi_io0_t;
    logic spi_io1_i, spi_io1_o, spi_io1_t;
    logic spi_sck_i, spi_sck_o, spi_sck_t;
    logic spi_ss_i, spi_ss_o, spi_ss_t;

    // Instantiate block design
    design_1 design_1_i (
        .sys_clock(sys_clock),
        .eth_ref_clk(eth_ref_clk),
        .reset(reset),
        .clk_1khz_out(clk_1khz_out),
        .enc_n(enc_n),
        .enc_p(enc_p),
        
        // DDR3 connections
        .ddr3_sdram_addr(ddr3_sdram_addr),
        .ddr3_sdram_ba(ddr3_sdram_ba),
        .ddr3_sdram_cas_n(ddr3_sdram_cas_n),
        .ddr3_sdram_ck_n(ddr3_sdram_ck_n),
        .ddr3_sdram_ck_p(ddr3_sdram_ck_p),
        .ddr3_sdram_cke(ddr3_sdram_cke),
        .ddr3_sdram_cs_n(ddr3_sdram_cs_n),
        .ddr3_sdram_dm(ddr3_sdram_dm),
        .ddr3_sdram_dq(ddr3_sdram_dq),
        .ddr3_sdram_dqs_n(ddr3_sdram_dqs_n),
        .ddr3_sdram_dqs_p(ddr3_sdram_dqs_p),
        .ddr3_sdram_odt(ddr3_sdram_odt),
        .ddr3_sdram_ras_n(ddr3_sdram_ras_n),
        .ddr3_sdram_reset_n(ddr3_sdram_reset_n),
        .ddr3_sdram_we_n(ddr3_sdram_we_n),
        
        // Ethernet
        .eth_mii_col(eth_mii_col),
        .eth_mii_crs(eth_mii_crs),
        .eth_mii_rst_n(eth_mii_rst_n),
        .eth_mii_rx_clk(eth_mii_rx_clk),
        .eth_mii_rx_dv(eth_mii_rx_dv),
        .eth_mii_rx_er(eth_mii_rx_er),
        .eth_mii_rxd(eth_mii_rxd),
        .eth_mii_tx_clk(eth_mii_tx_clk),
        .eth_mii_tx_en(eth_mii_tx_en),
        .eth_mii_txd(eth_mii_txd),
        
        // Tristate connections
        .eth_mdio_mdc_mdc(eth_mdio_mdc_mdc),
        .eth_mdio_mdc_mdio_i(eth_mdio_mdc_mdio_i),
        .eth_mdio_mdc_mdio_o(eth_mdio_mdc_mdio_o),
        .eth_mdio_mdc_mdio_t(eth_mdio_mdc_mdio_t),
        
        // Simple inputs
        .push_buttons_4bits_tri_i(push_buttons_4bits_tri_i),
        .dip_switches_4bits_tri_i(dip_switches_4bits_tri_i),
        
        // LED tristate
        .led_4bits_tri_i(led_4bits_tri_i),
        .led_4bits_tri_o(led_4bits_tri_o),
        .led_4bits_tri_t(led_4bits_tri_t),
        
        // RGB LED tristate
        .rgb_led_tri_i(rgb_led_tri_i),
        .rgb_led_tri_o(rgb_led_tri_o),
        .rgb_led_tri_t(rgb_led_tri_t),
        
        // UART
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd),
        
        // SPI tristate
        .spi_io0_i(spi_io0_i),
        .spi_io0_o(spi_io0_o),
        .spi_io0_t(spi_io0_t),
        .spi_io1_i(spi_io1_i),
        .spi_io1_o(spi_io1_o),
        .spi_io1_t(spi_io1_t),
        .spi_sck_i(spi_sck_i),
        .spi_sck_o(spi_sck_o),
        .spi_sck_t(spi_sck_t),
        .spi_ss_i(spi_ss_i),
        .spi_ss_o(spi_ss_o),
        .spi_ss_t(spi_ss_t)
    );

    // IOBUF instances for bidirectional I/O
    
    // Ethernet MDIO
    IOBUF eth_mdio_mdc_mdio_iobuf (
        .I(eth_mdio_mdc_mdio_o),
        .IO(eth_mdio_mdc_mdio_io),
        .O(eth_mdio_mdc_mdio_i),
        .T(eth_mdio_mdc_mdio_t)
    );
    
    // LED array (4 instances)
    genvar i;
    generate
        for (i = 0; i < 4; i = i + 1) begin : led_iobuf_gen
            IOBUF led_iobuf (
                .I(led_4bits_tri_o[i]),
                .IO(led_4bits_tri_io[i]),
                .O(led_4bits_tri_i[i]),
                .T(led_4bits_tri_t[i])
            );
        end
    endgenerate
    
    // RGB LED array (12 instances)
    generate
        for (i = 0; i < 12; i = i + 1) begin : rgb_led_iobuf_gen
            IOBUF rgb_led_iobuf (
                .I(rgb_led_tri_o[i]),
                .IO(rgb_led_tri_io[i]),
                .O(rgb_led_tri_i[i]),
                .T(rgb_led_tri_t[i])
            );
        end
    endgenerate
    
    // SPI IO buffers
    IOBUF spi_io0_iobuf (
        .I(spi_io0_o),
        .IO(spi_io0_io),
        .O(spi_io0_i),
        .T(spi_io0_t)
    );
    
    IOBUF spi_io1_iobuf (
        .I(spi_io1_o),
        .IO(spi_io1_io),
        .O(spi_io1_i),
        .T(spi_io1_t)
    );
    
    IOBUF spi_sck_iobuf (
        .I(spi_sck_o),
        .IO(spi_sck_io),
        .O(spi_sck_i),
        .T(spi_sck_t)
    );
    
    IOBUF spi_ss_iobuf (
        .I(spi_ss_o),
        .IO(spi_ss_io),
        .O(spi_ss_i),
        .T(spi_ss_t)
    );

endmodule
```

##### Step 3B.2: Add to Project and Set as Top

Follow same process as for `adc_interface.sv`

---

### Phase 4: Update Project Configuration (30 minutes)

#### Step 4.1: Set Top Module

1. In Sources panel, find `design_1_wrapper.sv`
2. Right-click → **Set as Top**
3. Verify it appears in bold

#### Step 4.2: Check Language Settings

1. **Tools → Settings**
2. Navigate to **Project Settings → General**
3. **Target language:** Can leave as "Mixed" or set to "SystemVerilog"
4. **Simulator language:** Mixed or SystemVerilog
5. Click **OK**

#### Step 4.3: Verify Source Hierarchy

In "Sources" panel, hierarchy should be:
```
design_1_wrapper (SystemVerilog) - TOP
├── design_1_i : design_1 (Block Design)
│   ├── [40+ IP blocks]
│   └── adc_interface_0 : adc_interface (SystemVerilog)
└── [IOBUF instances]
```

---

### Phase 5: Build and Verify (1-2 hours)

#### Step 5.1: Run Synthesis

1. **Flow Navigator → Synthesis → Run Synthesis**
2. Wait for completion (~10-15 minutes)
3. **Check for errors:**
   - Open "Messages" tab
   - Filter for Errors (should be 0)
   - Review warnings (some are OK)

**Common warnings to ignore:**
- Timing constraints not met (expected at synthesis stage)
- Unused ports
- Inferred latches (if intentional)

#### Step 5.2: Review Synthesis Results

1. Click **Open Synthesized Design**
2. Check schematic view
3. Verify module instantiations
4. Check resource utilization

#### Step 5.3: Run Implementation

1. **Flow Navigator → Implementation → Run Implementation**
2. Wait for completion (~15-20 minutes)
3. Check for errors

#### Step 5.4: Generate Bitstream

1. **Flow Navigator → Program and Debug → Generate Bitstream**
2. Wait for completion (~5 minutes)
3. Confirm success

#### Step 5.5: Compare with Original

**Critical checks:**
- Resource utilization similar to original (±5%)
- Timing similar (check worst negative slack)
- No new critical warnings
- Bitstream generated successfully

---

## Testing and Validation

### Hardware Testing

#### Test 1: FPGA Programming

1. Connect Arty A7-100 board via USB
2. Power on board
3. In Vivado: **Flow Navigator → Program and Debug → Open Hardware Manager**
4. Click **Open Target → Auto Connect**
5. Right-click on device → **Program Device**
6. Select `.bit` file
7. Click **Program**
8. **Success indicator:** DONE LED lights up

#### Test 2: Basic Functionality

1. **Check UART communication:**
   - Open serial terminal (115200 baud, 8N1)
   - Connect to Arty board's COM port
   - Press reset button
   - Verify boot messages

2. **Test GPIO:**
   - Toggle DIP switches
   - Observe LED changes
   - Test push buttons

3. **Test SPI (if accessible):**
   - Run software tests from Vitis

#### Test 3: Software Integration

1. **Open Vitis workspace**
2. **Build software project** (may need to regenerate BSP)
3. **Program FPGA with bitstream**
4. **Run/Debug software application**
5. **Verify all features work:**
   - ADC interface
   - Data capture
   - Display/output

### Comparison Testing

**Create test plan spreadsheet:**

| Feature | Original (VHDL) | New (SystemVerilog) | Pass/Fail |
|---------|-----------------|---------------------|-----------|
| FPGA programs | ✓ | ? | |
| UART communication | ✓ | ? | |
| GPIO works | ✓ | ? | |
| SPI communication | ✓ | ? | |
| Software runs | ✓ | ? | |
| ADC interface | ✓ | ? | |
| Data capture | ✓ | ? | |
| No critical warnings | ✓ | ? | |
| Resource usage similar | ✓ | ? | |
| Timing met | ✓ | ? | |

---

## Troubleshooting

### Common Issues and Solutions

#### Issue 1: Synthesis Fails with Module Not Found

**Error:** `Module 'design_1' not found`

**Solutions:**
1. Ensure block design is up-to-date
2. In Sources panel, right-click `design_1.bd` → **Reset Output Products**
3. Right-click again → **Generate Output Products**
4. Wait for completion, then retry synthesis

---

#### Issue 2: IOBUF Not Recognized

**Error:** `Unknown module 'IOBUF'`

**Solutions:**
1. Add library reference at top of file:
```systemverilog
`ifndef SYNTHESIS
    `include "glbl.v"
`endif
```

2. Or explicitly import UNISIM:
```systemverilog
// Not typically needed, but if required:
(* dont_touch = "true" *)
```

---

#### Issue 3: Port Connection Mismatch

**Error:** `Port 'xxx' not found in module 'design_1'`

**Solutions:**
1. Open block design
2. Check port names in block design exactly match wrapper
3. Regenerate wrapper if using auto-generation
4. Case sensitivity: SystemVerilog is case-sensitive, VHDL is not

---

#### Issue 4: Timing Violations After Migration

**Problem:** Timing was met before, now failing

**Solutions:**
1. Check if constraints (`.xdc`) still apply correctly
2. Verify clock constraints are still present
3. May need to adjust I/O buffer placement
4. Try re-running implementation with different strategies

---

#### Issue 5: Simulation Fails

**Error:** Testbench doesn't work with SystemVerilog modules

**Solutions:**
1. Update testbench to SystemVerilog
2. Or create mixed-language simulation
3. Check `timescale` directives match
4. Verify library paths in simulation settings

---

## Rollback Plan

### If Migration Fails

#### Quick Rollback (5 minutes)

1. **Restore from backup:**
```bash
cd /path/to
rm -rf oscilloscope_project
cp -r oscilloscope_project_backup oscilloscope_project
```

2. **Or revert files in Vivado:**
   - Remove SystemVerilog files from project
   - Re-add original VHDL files
   - Set `design_1_wrapper.vhd` as top
   - Run synthesis

#### Partial Rollback

**If only one file has issues:**

1. Remove problematic SystemVerilog file
2. Re-add original VHDL file
3. Update block design if needed
4. Continue with other files that work

---

## Post-Migration Tasks

### Documentation Updates

- [ ] Update project README to note SystemVerilog migration
- [ ] Document any behavioral differences (if any)
- [ ] Update build instructions
- [ ] Tag version in Git (e.g., `v2.0-systemverilog`)

### Code Cleanup

- [ ] Remove backup VHDL files once stable
- [ ] Add comments to SystemVerilog files
- [ ] Format code consistently
- [ ] Run linter if available

### Repository Organization

```
oscilloscope-fpga/
├── oscilloscope-fpga.srcs/
│   ├── sources_1/
│   │   ├── systemverilog/       # NEW: SystemVerilog sources
│   │   │   └── adc_interface.sv
│   │   ├── bd/
│   │   │   └── design_1/
│   │   │       └── hdl/
│   │   │           └── design_1_wrapper.sv  # Auto-generated
│   │   └── _vhdl_backup/        # OLD: Archived VHDL
│   │       ├── adc_interface.vhd
│   │       └── design_1_wrapper.vhd
│   └── constrs_1/
│       └── new/
│           └── arty.xdc         # Unchanged
```

---

## Success Criteria

### Migration is Complete When:

✅ All VHDL files converted to SystemVerilog  
✅ Project synthesizes without errors  
✅ Implementation completes successfully  
✅ Bitstream generates  
✅ FPGA programs successfully  
✅ All hardware tests pass  
✅ Software application runs correctly  
✅ Resource utilization within 5% of original  
✅ Timing requirements met  
✅ No new critical warnings  

---

## Additional Resources

### Xilinx Documentation

- [UG901: Vivado Design Suite User Guide - Synthesis](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2020_1/ug901-vivado-synthesis.pdf)
- [UG949: UltraFast Design Methodology](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2020_1/ug949-vivado-design-methodology.pdf)
- [Language Templates in Vivado](Tools → Language Templates)

### SystemVerilog Resources

- [SystemVerilog IEEE 1800-2017 Standard](https://ieeexplore.ieee.org/document/8299595)
- [Doulos SystemVerilog Tutorial](https://www.doulos.com/knowhow/systemverilog/)
- [ASIC World SystemVerilog](http://www.asic-world.com/systemverilog/)

### Support Forums

- [Xilinx Community Forums](https://forums.xilinx.com/)
- [Stack Overflow - SystemVerilog Tag](https://stackoverflow.com/questions/tagged/systemverilog)
- [Reddit r/FPGA](https://www.reddit.com/r/FPGA/)

---

## Appendix: Quick Reference Card

### SystemVerilog Module Template

```systemverilog
`timescale 1ns / 1ps

module module_name #(
    parameter int WIDTH = 8
)(
    input  logic             clk,
    input  logic             rst_n,
    input  logic [WIDTH-1:0] data_in,
    output logic [WIDTH-1:0] data_out
);

    // Internal signals
    logic [WIDTH-1:0] internal_reg;
    
    // Sequential logic
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            internal_reg <= '0;
        else
            internal_reg <= data_in;
    end
    
    // Combinational logic
    always_comb begin
        data_out = internal_reg;
    end

endmodule
```

### Vivado TCL Commands

```tcl
# Useful TCL commands for migration

# Reset synthesis
reset_run synth_1

# Run synthesis
launch_runs synth_1
wait_on_run synth_1

# Run implementation
launch_runs impl_1 -to_step write_bitstream
wait_on_run impl_1

# Report utilization
report_utilization -file utilization.rpt

# Report timing
report_timing_summary -file timing_summary.rpt
```

---

**Migration Document Version:** 1.0  
**Last Updated:** November 2024  
**Author:** Technical Documentation Team  
**Status:** Ready for Use
