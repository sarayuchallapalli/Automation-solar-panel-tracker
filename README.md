# Automatic Solar Panel Tracker

This project is a microcontroller-based automatic solar panel tracking system that enhances solar energy efficiency by adjusting the panel’s position according to the direction of sunlight.

# Working Principle

The system operates using two LDR (Light Dependent Resistor) sensors to detect sunlight intensity from both directions. Here's how it works:

- The analog signals from the LDRs are routed through a **74HC4051 multiplexer** to the **ADC0804**, which converts the signals into digital values.
- The **AT89C51 microcontroller** reads these digital values and compares them:
  - If the **left LDR** receives more light, the motor rotates the solar panel **left**.
  - If the **right LDR** receives more light, the panel rotates **right**.
- When both readings are nearly equal (e.g., in cloudy conditions), the system switches to **time-based control** using a **DS1307 RTC (Real-Time Clock)**. The panel is then adjusted based on the sun's expected position during the day.

This ensures **continuous and efficient solar tracking** throughout the day, regardless of weather conditions.

# Components Used
- AT89C51 Microcontroller
- LDR sensors (x2)
- ADC0804 (Analog to Digital Converter)
- 74HC4051 Multiplexer
- DS1307 RTC Module
- DC Motor or Stepper Motor
- Motor Driver (e.g., L293D or ULN2003)
- Power Supply Circuit
- Resistors, Capacitors, Crystal Oscillator
- Proteus for Simulation
- Keil uVision for Coding

#Files Included
- Source Code (.c)
- Hex File (.hex)
- Proteus Simulation File (.psdprj)
- Circuit Diagram 
- 

# How to Use
1. Open the .psdprj file in Proteus.
2. Upload the .hex file to the AT89C51 in simulation.
3. Run the simulation and observe the panel rotation based on LDR inputs.
4. Modify the code in Keil if needed and re-generate the .hex file.



# Author
Sarayu Challapalli  
Electronics & Embedded Systems Enthusiast

---


