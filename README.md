# Remote repeat
Repeats IR remote signals received at e.g. a TV to a connected IR LED connected to a long wire placed e.g. inside a cabinet. This is also a minimal footprint platform to receive standard IR signals, manipulate them and to send standard IR signals.

## Hardware
The hardware consists of a standard IR receiver (TSOP38238) and an Attiny85 to process the signal and to control sending a new signal. The application is powered by USB micro from e.g. the USB output from a TV.

![alt tag](https://github.com/mik4el/gadget-remote-repeat/blob/master/remote_repeat_20160908_2.JPG)

The pcb, where more details can be found in the hardware folder, is a minimal two layer pcb for the application. The top view of the current pcb can be viewed below:

![alt tag](https://github.com/mik4el/gadget-remote-repeat/blob/master/gadget-remote-repeat-hardware/pcb_top.png)

## Software dependencies
The software is based on Arduino and uses the external library tiny_IRremote adopted by https://github.com/SeeJayDee from the popular https://github.com/z3t0/Arduino-IRremote library. I have made modifications to the library to be even smaller for the current usage. The modified library is in ```gadget-remote-repeat-tiny```. To include the current library please follow e.g. https://www.arduino.cc/en/Guide/Libraries to install an external library.

## Structure of repo
For an Attiny85 version of the software, please see ```gadget-remote-repeat-tiny```. For other Arduino platforms please see ```gadget-remote-repeat```.
