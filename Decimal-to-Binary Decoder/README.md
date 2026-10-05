Decimal to Binary Trainer
My small Arduino hardware project — a pocket trainer to practice fast decimal-to-binary conversion (1 byte, from 0 to 255).

The circuit and prototype were designed to be easily built and soldered on physical hardware (perfboard or breadboard).

How It Works
On boot, the Arduino picks a random number from 0 to 255 (seeded by noise on the unconnected pin A0) and displays it on the screen:

Plaintext
Generated: 142
Pass: 
Enter the answer bit by bit using two pushbuttons (from MSB to LSB, 8 bits total):

Left button — 0

Right button — 1

As soon as all 8 bits are entered, the system immediately checks the result:

Green LED lights up — correct.

Red LED lights up — mistake detected.

To start a new round and generate the next number, simply press the on-board Reset button.

Bill of Materials (BOM)
Arduino Uno (or Nano / any ATmega328P-compatible board)

16x2 LCD Display (HD44780-compatible)

2 tactile pushbuttons

2 LEDs (Green and Red)

Resistors:

2× 10 kΩ (pull-down resistors for buttons)

2× 220 Ω (current-limiting resistors for LEDs)

Breadboard or perfboard, jumper wires, and a soldering iron.

Pinout & Wiring
The complete wiring schematic can be found in decimal-to-binary.pdf. A quick pinout reference:

16x2 LCD Display
RS → Pin 7

E → Pin 8

D4 → Pin 9

D5 → Pin 10

D6 → Pin 11

D7 → Pin 12

VSS, RW, K (LED-) → GND

VDD, A (LED+) → +5V

V0 (contrast) → to GND (or via a 10 kΩ potentiometer/trimmer if you need to fine-tune the display contrast).

Pushbuttons (Pull-Down Configuration)
One terminal of each button connects to +5V, while the other connects to the designated Arduino pin and through a 10 kΩ resistor to GND:

Button 0 → Pin 2

Button 1 → Pin 3

LEDs
Red LED → 220 Ω resistor → Pin 5 (cathode to GND)

Green LED → 220 Ω resistor → Pin 6 (cathode to GND)

How to Flash
Open decimal_to_binary1.ino in the Arduino IDE.

Requires only the built-in `` library (no extra installs needed).

Select your board model, choose the correct COM port, and click Upload.

Repository Files
decimal_to_binary1.ino — Arduino firmware sketch.

decimal-to-binary.pdf — wiring diagram / schematic.

bom.csv — bill of materials.
