# LED Control and Timer Interrupt 

## Overview

This project demonstrates the use of interrupts and timers in embedded systems. The main goal is to manage LED states based on button presses and timer interrupts, implementing a simple user interface for mode selection and LED control.

## Project Structure

The project is structured to achieve the following functionalities:

1. **Button and LED Configuration**
   - Configures two buttons for input and two pairs of LEDs for output.
   - Buttons and LEDs are connected to specific ports with appropriate settings for input, output, and internal resistors.

2. **Timer Configuration**
   - Uses a 16-bit Timer to generate periodic interrupts.
   - The timer interrupts are used to manage the state of the LEDs based on the selected mode.

3. **Interrupt Service Routines (ISRs)**
   - Implemented ISRs for handling button presses and timer events.
   - Button presses toggle the selection of the active LED and change the operating mode.
   - Timer interrupts control the timing of LED state changes.

## System Behavior

### Initialization

- **Watchdog Timer:** Disabled to prevent unintended resets during development.
- **Switch Configuration:** 
  - **P1.1 and P1.4** are configured as input with internal pull-up resistors.
  - Switch interrupts are configured at the port level, NVIC, and CPU level.
- **LED Configuration:**
  - **P1.0, P2.0, P2.1, and P2.2** are configured as outputs.
  - All LEDs are initialized to be turned off.
- **Timer Configuration:** A 16-bit Timer is set up for periodic interrupts.

### Operation Modes

1. **Regular Mode:**
   - The system defaults to this mode.
   - The "other" LED toggles every `D` seconds (default `D` is 1 second).

2. **Slow Mode:**
   - Activated by pressing the mode button for the second time.
   - The "other" LED toggles every `D` seconds (where `D` is changed to 2 seconds).

3. **Paused Mode:**
   - Activated by pressing the mode button for the third time.
   - All LEDs turn off and the system halts LED state changes.

4. **Return to Regular Mode:**
   - Activated by pressing the mode button for the fourth time.
   - The system returns to the regular mode and continues the LED behavior cycle.

### LED Behavior

- In **Regular** and **Slow** modes, the currently inactive LED switches states or colors every `D` seconds.
  - If the Red LED is inactive, it toggles on and off every `D` seconds.
  - If the RGB LED is inactive, it cycles through color combinations every `D` seconds.

## Development Setup

1. **Development Environment:**
   - Keil IDE is used for project creation and management.
   - Ensure all necessary libraries and configurations are included.

2. **Configuration Steps:**
   - Disable the Watchdog timer.
   - Set up port configurations for switches and LEDs.
   - Configure Timer A for periodic interrupts.
   - Implement ISRs for switch and timer events.

## Notes

- Ensure to refer to the appropriate reference manuals and lecture slides for detailed configuration of interrupts and timers.
- Test the system thoroughly to validate correct behavior in all modes.
