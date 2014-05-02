## Using interrupts in Xilinx EDK
* **This is an example to show how to use interrupts from SDK**
* Tested on Xilinx ISE Design Suite 14.6 using Digilent Nexys3 board
* Create a project from the wizard with at least 16KB memory and have `push_buttons` with interrupts, `leds`, and `rs232 serial port`
* Generate the bitstream and export it to SDK
* Create a new application, and add the `interruptdemo.c`
* Check out `system.mhs` for an example design
