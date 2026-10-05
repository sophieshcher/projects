# Decimal to Binary Trainer

Pocket Arduino-based trainer for practicing fast decimal-to-binary conversion (0–255, 1 byte). Designed for perfboard or breadboard assembly.

## How It Works

1. On boot, the board generates a random number from 0 to 255 (seeded by analog noise on pin `A0`) and displays:
```text
Generated: 142
Pass: 

```


2. Enter the 8-bit binary value from MSB to LSB using two buttons:
* Left button: `0`
* Right button: `1`


3. After entering all 8 bits, the result is evaluated:
* Green LED: Correct
* Red LED: Incorrect


4. Press the on-board **Reset** button to start a new round.

## Components

* Arduino Uno, Nano, or any ATmega328P compatible board
* HD44780 16x2 LCD
* 2x Tactile buttons
* 2x LEDs (Red, Green)
* 2x 10k Ohm resistors (pull-down)
* 2x 220 Ohm resistors (current limiting)
* Breadboard or perfboard and jumper wires

## Wiring

Detailed schematic: `decimal-to-binary.pdf`

* **16x2 LCD:**
* RS -> Pin 7
* E -> Pin 8
* D4 -> Pin 9
* D5 -> Pin 10
* D6 -> Pin 11
* D7 -> Pin 12
* VSS, RW, K -> GND
* VDD, A -> 5V
* V0 -> GND (or 10k potentiometer for contrast)


* **Buttons (Pull-Down):**
* One leg to 5V, other leg to digital pin and 10k resistor to GND.
* Button 0 -> Pin 2
* Button 1 -> Pin 3


* **LEDs:**
* Red LED -> 220 Ohm resistor -> Pin 5 (cathode to GND)
* Green LED -> 220 Ohm resistor -> Pin 6 (cathode to GND)



## Flashing

1. Open `decimal_to_binary1.ino` in Arduino IDE.
2. Ensure the built-in `` library is available.
3. Select board and port, then click **Upload**.

## Files

* `decimal_to_binary1.ino` — Source code
* `decimal-to-binary.pdf` — Schematic
* `bom.csv` — Bill of materials
