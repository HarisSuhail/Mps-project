# Wireless Motor Control


Transmitter Hardware:
8051 microcontoller + power supply + crystal oscillator
Water level sensor made with 5 bipolar junction transistors
RF transmitter, 433MHz
Detects water level using a BJT circuit interfaced with 8051 port. Transmits level sensor data via 433MHz transmitter attached interfaced to 8051.


Receiver Hardware:
8051 microcontoller + power supply + crystal oscillator
LCD display
Motor interface
GPS module
Keypad (4 columns, 4 rows)
Receives data from transmitter using 433Mhz receiver. Displays water level on LCD display, sends control signals to motor, and sends SMS message command to GPS module. Has an attached Keypad to control device and change its settings.

Software: Transmitter and receiver software is in seperate folders and is labelled accordingly. Open with Keil uVision.
