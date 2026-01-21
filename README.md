This is a bare metal project for the STM32F767ZI on a Nucleo-F767ZI.
It toggles the green LED (LD1) with 1 Hz and the output pin G0 with 50 Hz.

1. Clone the repo with git

git clone https://github.com/nikolai-baschinski/STM32F767ZI_LD1_PG0_Toggle_bare_metal.git

3. Install the STM32CubeIDE and import the project.
4. Connect the Nucleo-STM32F67ZI dev board to the USB interface of your PC.
5. Build and Run the project on the target.

The clock is set to 96 MHz.
The setting for the prescalers can be found in the excel file doc/Calculation of the CPU clock for STM32F7.xlsx

![Nucleo-F767ZI](/doc/Nucleo-F767ZI.jpg)

![Oszilloscope on PG0](/doc/oscilloscope%20PG0%2050%20Hz.bmp)

