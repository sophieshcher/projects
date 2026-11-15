# Engineering, Physics & Scientific Computing Portfolio

Central repository containing hardware engineering designs, astrophysical and numerical simulations in **C++ (OpenMP)**, **Python**, and **MATLAB**, CAD models, physics laboratory reports, and engineering competition certificates.

---

## Quick Navigation by Section

1. [Featured Engineering & Simulation Projects](#1-featured-engineering--simulation-projects)
2. [Computational & Numerical Methods (C++)](#2-computational--numerical-methods-c)
3. [Programming in C++ & Python (`прога`)](#3-programming-in-c--python-%D0%BF%D1%80%D0%BE%D0%B3%D0%B0)
4. [MATLAB Modeling & Signal Analysis](#4-matlab-modeling--signal-analysis)
5. [AutoCAD 3D Models & Technical Drawings](#5-autocad-3d-models--technical-drawings)
6. [Physics Lab Reports: Electromagnetism & Optics](#6-physics-lab-reports-electromagnetism--optics)
7. [Certificates & Competitions](#7-certificates--competitions)

---

## 1. Featured Engineering & Simulation Projects

### 1.1 [2D N-Body Gravitational Simulation](./N-body%20Gravitational%20Simulation)
High-performance C++ simulation of gravitational cold collapse comparing direct $O(N^2)$ summation, multi-threaded **OpenMP** parallelism, and the **Barnes-Hut Quadtree** $O(N \log N)$ algorithm using symplectic Leapfrog integration.
- **Documentation & Benchmarks:** [`N-body Gravitational Simulation/README.md`](./N-body%20Gravitational%20Simulation/README.md)
- **C++ Solvers:**
  - [`nbody_sequential.cpp`](./N-body%20Gravitational%20Simulation/nbody_sequential.cpp) — Direct $O(N^2)$ baseline solver
  - [`nbody_openmp.cpp`](./N-body%20Gravitational%20Simulation/nbody_openmp.cpp) — Multi-threaded OpenMP solver
  - [`nbody_barneshut.cpp`](./N-body%20Gravitational%20Simulation/nbody_barneshut.cpp) & [`nbody_barneshut_algo.h`](./N-body%20Gravitational%20Simulation/nbody_barneshut_algo.h) — Barnes-Hut Quadtree solver ($78.77\times$ speedup at $N=50,000$)
  - [`benchmark.cpp`](./N-body%20Gravitational%20Simulation/benchmark.cpp) — Automated scaling benchmark suite
- **Post-Processing & Visualizations:**
  - [`matlab_scripts/`](./N-body%20Gravitational%20Simulation/matlab_scripts) — MATLAB scripts for energy conservation, radial density profile, and scaling plots
  - [`data/`](./N-body%20Gravitational%20Simulation/data) — Raw benchmark and trajectory CSV datasets
  - [`plots/`](./N-body%20Gravitational%20Simulation/plots) — Generated figures and [`nbody_animation.gif`](./N-body%20Gravitational%20Simulation/plots/nbody_animation.gif)

---

### 1.2 [Smart Closed-Loop DC Motor Control Module (`proj-module-for-dc-motor`)](./proj-module-for-dc-motor)
Self-contained actuator control module developed for **EPS Engineering Project Sprint 2026 (Case 1.1.3)** featuring **STM32F103C8**, **DRV8874** H-Bridge driver with current regulation, **AS5600** 12-bit contactless magnetic encoder, and **TPS54302** buck converter.
- **Full Technical Specification:** [`proj-module-for-dc-motor/README.md`](./proj-module-for-dc-motor/README.md)
- **Schematics & PCB Layout (`Gerber/`):**
  - [Electrical Schematics (PDF)](./proj-module-for-dc-motor/Gerber/1.1.3%20module%20for%20DC%20motor%20electrical%20drawings.pdf)
  - [PCB Layout & Fabrication Drawings (PDF)](./proj-module-for-dc-motor/Gerber/PCB_Project%20(2).pdf)
- **3D Mechanical CAD:** [`3D model/CAD.zip`](./proj-module-for-dc-motor/3D%20model/CAD.zip)
- **Component Datasheets:** [`datasheet components/`](./proj-module-for-dc-motor/datasheet%20components) ([STM32F103C8](./proj-module-for-dc-motor/datasheet%20components/DS_stm32f103c8-1.pdf), [AS5600](./proj-module-for-dc-motor/datasheet%20components/Magnetic%20Rotary%20Position%20Sensor%20AS5600%20Datasheet.pdf), [DRV8874](./proj-module-for-dc-motor/datasheet%20components/drv8874.pdf), [TPS54302](./proj-module-for-dc-motor/datasheet%20components/tps54302.pdf))
- **Defense Slide Deck:** [Presentation (PDF)](./proj-module-for-dc-motor/%D0%9A%D0%BE%D0%BC%D0%B0%D0%BD%D0%B4%D0%B0_1.1.3_%D0%A0%D0%BE%D0%B7%D1%80%D0%BE%D0%B1%D0%BA%D0%B0%20%D0%BC%D0%BE%D0%B4%D1%83%D0%BB%D1%8F%20%D0%BA%D0%B5%D1%80%D1%83%D0%B2%D0%B0%D0%BD%D0%BD%D1%8F%20DC-%D0%BC%D0%BE%D1%82%D0%BE%D1%80%D0%BE%D0%BC.pptx.pdf)

---

### 1.3 [Decimal-to-Binary Hardware Trainer (`Decimal-to-Binary Decoder`)](./Decimal-to-Binary%20Decoder)
Arduino-based (ATmega328P) hardware trainer with an HD44780 16×2 LCD display for practicing rapid 8-bit decimal-to-binary conversion.
- **Documentation & Pinout:** [`Decimal-to-Binary Decoder/README.md`](./Decimal-to-Binary%20Decoder/README.md)
- **Arduino Firmware:** [`decimal_to_binary1.ino`](./Decimal-to-Binary%20Decoder/decimal_to_binary1.ino)
- **Electrical Schematic:** [`decimal-to-binary.pdf`](./Decimal-to-Binary%20Decoder/decimal-to-binary.pdf)
- **Bill of Materials:** [`bom.csv`](./Decimal-to-Binary%20Decoder/bom.csv)
- **Hardware Video Demo:** [`Screen Recording 2026-09-09 151458.mp4`](./Decimal-to-Binary%20Decoder/Screen%20Recording%202026-09-09%20151458.mp4)

---

## 2. [Computational & Numerical Methods (C++)](./computational%20methods)

Implementation of numerical methods in C++ along with analytical reports:

| Lab | Source Code (C++) | Report (`.docx`) |
| :--- | :--- | :--- |
| **Lab 1** | [`computational methods/laba_1/main.cpp`](./computational%20methods/laba_1/main.cpp) | [`laba1.docx`](./computational%20methods/laba1.docx) |
| **Lab 2** | [`computational methods/laba_2/main.cpp`](./computational%20methods/laba_2/main.cpp) | [`laba2.docx`](./computational%20methods/laba2.docx) |
| **Lab 3** | [`computational methods/laba_3/main.cpp`](./computational%20methods/laba_3/main.cpp) | [`laba3.docx`](./computational%20methods/laba3.docx) |
| **Lab 4** | [`computational methods/laba_4/main.cpp`](./computational%20methods/laba_4/main.cpp) | [`laba4.docx`](./computational%20methods/laba4.docx) |
| **Lab 5** | [`computational methods/laba_5/main.cpp`](./computational%20methods/laba_5/main.cpp) | [`laba5.docx`](./computational%20methods/laba5.docx) |
| **Lab 6** | [`computational methods/laba_6/main.cpp`](./computational%20methods/laba_6/main.cpp) | [`laba6.docx`](./computational%20methods/laba6.docx) |

---

## 3. [Programming in C++ & Python (`прога`)](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0)

### Python Scripts & Simulations
- **Thermal Modeling:** [`therma.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/therma.py)
- **Python Labs:**
  - [`shcherbyna-lab-1.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/shcherbyna-lab-1.py)
  - [`shcherbyna-lab-2.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/shcherbyna-lab-2.py)
  - [`shcherbyna-lab-3.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/shcherbyna-lab-3.py) ([Report PDF](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab-3.pdf))
  - [`shcherbyna-lab-4.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/shcherbyna-lab-4.py) ([Report PDF](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab-4.pdf))
  - [`shcherbyna_lab_5.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/shcherbyna_lab_5.py) ([Report PDF](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab-5.pdf))
  - [`shcherbyna-lab-6.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/shcherbyna-lab-6.py) ([Report PDF](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab-6.pdf))
  - [`Shcherbyna-lab-7.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/Shcherbyna-lab-7.py) ([Report PDF](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab-7.pdf))
  - [`shcherbyna_lab-8.py`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/shcherbyna_lab-8.py)

### C++ Labs
- [`lab_3/main.cpp`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab_3/main.cpp) • [`lab_3-5/main.cpp`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab_3-5/main.cpp) • [`lab_4/main.cpp`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab_4/main.cpp) • [`lab_5/main.cpp`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab_5/main.cpp) • [`lab_6/main.cpp`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab_6/main.cpp) • [`lab_7/main.cpp`](./%D0%BF%D1%80%D0%BE%D0%B3%D0%B0/lab_7/main.cpp)

---

## 4. [MATLAB Modeling & Signal Analysis](./matlab)

- [`proj_rlc_loop.mlx`](./matlab/proj_rlc_loop.mlx) — RLC circuit oscillatory and transient simulation
- [`proj_signal.mlx`](./matlab/proj_signal.mlx) — Signal processing and spectral analysis
- [`laba3.mlx`](./matlab/laba3.mlx) & [`laba__4.mlx`](./matlab/laba__4.mlx) — Computational modeling scripts

---

## 5. [AutoCAD 3D Models & Technical Drawings](./autocad-model)

- [Final 3D Model Layout (`3dmodel-Layout3finananananleeee.pdf`)](./autocad-model/3dmodel-Layout3finananananleeee.pdf)
- [3D Assembly Layout (`3dmodel2222-Layout3.pdf`)](./autocad-model/3dmodel2222-Layout3.pdf)
- [Views & Cross-Sections (`види і розрізи-Layout3.pdf`)](./autocad-model/%D0%B2%D0%B8%D0%B4%D0%B8%20%D1%96%20%D1%80%D0%BE%D0%B7%D1%80%D1%96%D0%B7%D0%B8-Layout3.pdf)
- [Parametric Drawing (`араметризація-Layout1.pdf`)](./autocad-model/%D0%B0%D1%80%D0%B0%D0%BC%D0%B5%D1%82%D1%80%D0%B8%D0%B7%D0%B0%D1%86%D1%96%D1%8F-Layout1.pdf)
- [Pattern Array Modeling (`Massiveeekehgfdwsuydjeglfvwuehlrgid-Layout3.pdf`)](./autocad-model/Massiveeekehgfdwsuydjeglfvwuehlrgid-Layout3.pdf)
- [Geometric Primitives (`примітиви – копія-Layout3.pdf`)](./autocad-model/%D0%BF%D1%80%D0%B8%D0%BC%D1%96%D1%82%D0%B8%D0%B2%D0%B8%20%E2%80%93%20%D0%BA%D0%BE%D0%BF%D1%96%D1%8F-Layout3.pdf)

---

## 6. Physics Lab Reports: Electromagnetism & Optics

### [Electromagnetism (`elmag/`)](./elmag)
- [Lab 1](./elmag/untitled-1lab.pdf) • [Lab 3](./elmag/laba3.pdf) • [Lab 4](./elmag/laba4.pdf) • [Lab 5 (`EM_Lab5.pdf`)](./elmag/EM_Lab5.pdf) • [Labs 6–7 (`laba6_7.pdf`)](./elmag/laba6_7.pdf) • [Lab 8](./elmag/laba8.pdf)

### [Optics (`optics_lab/`)](./optics_lab)
- [Optics Lab 1](./optics_lab/opt_lab_1.pdf) • [Optics Lab 3](./optics_lab/opt_lab_3.pdf) • [Optics Lab 4](./optics_lab/opt_lab_4.pdf) • [Optics Lab 6](./optics_lab/opt_laba_6.pdf) • [Optics Lab 7](./optics_lab/opt-laba7.pdf) • [Optics Lab 8](./optics_lab/opt-lab8.pdf)

---

## 7. Certificates & Competitions

- [CodePlasma 2026 Certificate (with translation)](./CodePlasma2026_Certificate_with_translation.pdf)
- [EPS 2026 Engineering Project Sprint Certificate (with translation)](./EPS2026_Certificate_with_translation.pdf)
- [PolyTECH EDU 2025 Certificate (with translation)](./PolyTECH_EDU_2025_Certificate_with_translation.pdf)
- [PolyTECH EDU Original Certificate](./certificate_polytech_edu.pdf)
