

# STM32F1xx-Based 895 DC Motor Control Module

> **Case 1.1 | Track: Electronics / Embedded**
> Developed as part of the **EPS Engineering Project Sprint 2026** by **Team 1.1.3**.

---

## Overview

This repository contains the hardware design, manufacturing files, CAD models, and technical documentation for a high-performance, compact servo-drive module designed to control a high-power **size-895** brushed DC motor (12 V rated voltage, 2 A continuous operating current, up to 3 A peak current).

The module is engineered to operate reliably in severe electrical noise environments where the motor and control logic share a common 12 V power supply. Key highlights include an integrated H-bridge driver with current mirror telemetry, an efficient synchronous step-down converter, active reverse-polarity protection, an off-board 12-bit magnetic absolute encoder, and a robust UART binary communication interface.

---

## Key Technical Specifications

| Parameter | Specification | Notes |
| --- | --- | --- |
| **Input Supply Voltage** | 12 V DC (Operating range: 9 V to 18 V) | Shared power rail for power stage and digital logic |
| **Load Current** | 2 A continuous, 3 A peak | Hardware overcurrent trip threshold configured to 3.08 A |
| **Microcontroller** | STM32F103C8T6 | ARM Cortex-M3 (72 MHz, 64 KB Flash, 20 KB SRAM) |
| **Motor Driver IC** | TI DRV8874PWPR | Integrated monolithic H-Bridge, PH/EN control mode |
| **H-Bridge On-Resistance** | 200 mOhm (High-side + Low-side) | Low thermal dissipation (0.8 W at 2 A) |
| **PWM Switching Frequency** | 20 kHz | Hardware timer generation, silent motor operation |
| **Logic Supply Rail** | TPS54302DDCR (12 V to 3.3 V) | Synchronous Buck converter (400 kHz, ~90% efficiency) |
| **Position Feedback** | AS5600 (Off-board module) | 12-bit absolute magnetic encoder (4096 positions, 0.088 deg) |
| **Current Sensing** | Integrated IPROPI current mirror | 0.81 V/A scale factor via 1.8 kOhm load resistor |
| **Host Interface** | UART (USART1, 3.3 V TTL) | Binary packet-based protocol with CRC-8 validation |
| **PCB Form Factor** | 60 x 40 mm | 2-layer FR-4, 1 oz copper (35 um) |
| **Terminals** | Screw terminal blocks (3.81 mm pitch) | Solderless connection for power supply and motor leads |

---

## System Architecture and Engineering Decisions

### 1. Power Distribution and Primary Protection Stage

* **Active Reverse-Polarity Protection:** Implemented using a P-channel power MOSFET (P-MOSFET `Q1`) in an "ideal diode" configuration. Unlike standard Schottky diodes that introduce a 0.5 V forward voltage drop and dissipate over 1.5 W of heat at 3 A, the P-MOSFET maintains a negligible on-state resistance, generating virtually no heat.
* **Overcurrent & Thermal Safety:** Protected by an onboard 3 A resettable PTC fuse (`F1`).
* **High-Efficiency Step-Down Conversion:** A Texas Instruments TPS54302 synchronous Buck converter steps down 12 V to 3.3 V with ~90% efficiency. It dissipates less than 0.05 W of heat compared to 0.87 W for conventional linear regulators (LDOs), eliminating hotspots inside sealed enclosures.

### 2. Motor Power Stage (DRV8874)

* **Thermal Efficiency:** With an on-resistance of 200 mOhm across both switches, total conduction losses at the 2 A nominal operating current are:

$$P_{\text{loss}} = I^2 \cdot R_{\text{DS(on)}} = 2^2 \cdot 0.2 = 0.8\text{ W}$$

This allows safe operation on a compact two-layer PCB without bulky external aluminum heatsinks.

* **Hardware Stall Protection:** The onboard analog divider on the `VREF` pin fixes the hardware current-chopping threshold:

$$I_{\text{trip}} = \frac{V_{\text{REF}}}{A_{\text{IPROPI}} \cdot R_{\text{IPROPI}}} = \frac{2.507}{0.00045 \cdot 1800} \approx 3.08\text{ A}$$

This automatically protects the switching MOSFETs from burn-out during rotor lockup.

### 3. Analog Current Sensing and Signal Conditioning

* The DRV8874 outputs an accurate current-mirror signal on the `IPROPI` pin ($A_{\text{IPROPI}} = 450\text{ }\mu\text{A/A}$).
* Terminated through $R_{11} = 1.8\text{ k}\Omega$, this generates a proportional analog voltage of 0.81 V/A (1.62 V at 2 A; 2.43 V at 3 A), matching the 0 V to 3.3 V input scale of the STM32 12-bit ADC.
* High-frequency PWM switching noise is filtered inside the MCU firmware using a 16-sample Moving Average cyclic buffer.

### 4. Off-Board Magnetic Absolute Encoder (AS5600)

* Due to the physical bulk of the 895 motor and gearbox, mounting the main 60 x 40 mm PCB directly behind the output shaft is mechanically impractical. The AS5600 sensor is placed on a dedicated miniature carrier board attached via a custom 3D-printed mounting bracket.
* The sensor reads a diametrically magnetized neodymium magnet positioned with an air gap of 1.5 mm.
* Communicates over an I2C bus in Fast Mode (400 kHz) with dedicated on-board 4.7 kOhm pull-up resistors.
* Provides instantaneous absolute shaft angle measurement upon startup without requiring homing sequences or limit switches.

---

## UART Communication Protocol

The module interfaces with host systems (e.g., Companion Computer, Flight Controller, Mission Planner, or PLC) via USART1 (115200 baud, 8N1).

### Frame Structure

* `[0xAA]` — Start Byte (Header)
* `[CMD]` — Command ID (1 byte)
* `[LEN]` — Data Payload Length (1 byte)
* `[DATA]` — Command Payload (1 to 4 bytes)
* `[CRC-8]` — Checksum (Polynomial: 0x07)
* `[0x55]` — Stop Byte (Footer)

### Command Set

* `0x01` (`SET_TARGET_ANGLE`): Sets target output shaft angle (0.0 deg to 360.0 deg) and rotational speed.
* `0x02` (`SET_SPEED_DIRECT`): Direct open-loop PWM duty cycle (0% to 100%) and direction (CW/CCW).
* `0x03` (`ENABLE_MOTOR`): Driver enable / low-power sleep toggle.
* `0x04` (`EMERGENCY_STOP`): Immediate regenerative dynamic braking.
* `0x05` (`GET_ANGLE`): Queries current absolute angle from the encoder.
* `0x06` (`GET_CURRENT`): Queries instantaneous motor current draw in mA.
* `0x07` (`SET_ZERO_POSITION`): Stores current position as zero-reference offset in non-volatile memory.
* `0x08` (`SET_CURRENT_LIMIT`): Updates software overcurrent threshold dynamically.

### Automated Module Frames

* `0xBB` — **Periodic Telemetry** (transmitted every 10 s): Contains real-time angle, current draw, and driver state flags.
* `0xEE` — **Asynchronous Fault Frame** (transmitted immediately upon fault detection): Reports stall conditions, driver `nFAULT` triggers, or voltage anomalies.

---

## Repository Structure

* **`3D model/`** — CAD models (STEP and STL formats) of the complete module assembly and the 895 motor encoder bracket.
* **`datasheet components/`** — Official component datasheets (STM32F103, DRV8874, TPS54302, AS5600).
* **`Gerber/`** — Production-ready Gerber RS-274X and NC Drill fabrication files.
* **`Презентація Команди кейсу 1.1.3/`** — Final sprint defense slide deck (PDF/PPTX).
* **`README.md`** — Project documentation and engineering summary.

---

## Practical Applications

1. **Long-Range FPV Antenna Trackers (Pan-Tilt Systems):**
* Uses the high torque of the 895 motor and reduction gearbox to keep heavy directional patch and helical antennas aimed against severe wind loads.
* Instantaneous 12-bit absolute position recovery via AS5600 without homing calibration after field power loss.
* Software current trip protects expensive RF coaxial feeder cables from twisting or tearing if mechanical travel limits are exceeded.


2. **Unmanned Ground Vehicles (UGV / AGV Wheel Actuators):**
* Distributed wheel drives where each module functions as a smart, self-protecting actuator receiving velocity commands via UART.


3. **Industrial Actuators and Automated Valves:**
* Virtual limit switching: uses current threshold detection to detect fully seated valve/gate positions without mechanical endstops.



---

## Development Team (Team 1.1.3)

* **Oleksandr Illarionov**
* **Sofiia Shcherbyna**
* **Daryna Burdak**
* **Robert Kish**
* **Anastasiia Horiacheva**

*EPS Engineering Project Sprint 2026*
