# Wireless Motor Control

<br />
Transmitter Hardware:<br />
8051 microcontoller + power supply + crystal oscillator<br />
Water level sensor made with 5 bipolar junction transistors<br />
RF transmitter, 433MHz<br />
Detects water level using a BJT circuit interfaced with 8051 port. Transmits level sensor data via 433MHz transmitter attached interfaced to 8051.<br />


Receiver Hardware:<br />
8051 microcontoller + power supply + crystal oscillator<br />
LCD display<br />
Motor interface<br />
GPS module<br />
Keypad (4 columns, 4 rows)<br />
Receives data from transmitter using 433Mhz receiver. Displays water level on LCD display, sends control signals to motor, and sends SMS message command to GPS module. Has an attached Keypad to control device and change its settings.
<br />
Software: Transmitter and receiver software is in seperate folders and is labelled accordingly. Open with Keil uVision.
<br />
