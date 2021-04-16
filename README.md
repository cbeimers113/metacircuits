Metacircuits Mod V 1.0
===============================

By firefreak11

Inspired by the [Wiremod](https://github.com/wiremod/wire) addon for Garry's Mod, Metacircuits was designed to allow the player to create
more complex electronic circuits without using the pixel grid to create physical connections. The goal of this is to create a sandbox where
electronics can be created with execution speeds and compact designs similar to subframe technology, but without having to deal with the complex
simulation features that make subframe possible. The mod introduces components that can have inputs and/or outputs which can be "wired" together 
to send data between components. The DIGI (digitizer) element can take an electrical signal (SPRK) and convert it to a circuit signal of "1", 
which can be accessed by other components via DIGI's output. The DIOD (diode) element does the opposite, converting any non-zero circuit signal 
to an electrical signal. Other components include logic gates and LEDs.


Instructions
============

To connect two components, use the wire tool in the Circuits menu. You must use the line drawing feature (shift click) to specify which 
components to connect together. Shift click on the component whose **output** you want, and release the wire tool on the component whose
**input** you want. A window will open up which will allow you to specify which input should recieve which output's signal. Once this is
set, click the "OK" button to complete the connection. The UI will draw a light green wire to show the connection visually if this feature
is enabled. 


Warnings
========

Creating circuits that loop onto themselves will cause a stack overflow since the entire circuit's state is updated at once each frame.
If you need the output of a circuit to feed back into itself as input, use DIOD to "decode" the signal into SPRK, then send the SPRK back
into the circuit via DIGI.

Copying/cutting and pasting circuits is buggy, so if you need to do this to a circuit, use the stamp feature. Rotating and flipping stamps
is currently not supported for circuit stamps.

Save documents currently don't retain circuit data. To share a circuit build, save the build as a stamp, then share the .stm file in stamps/
*as well as* the .cct file with the same name in circuits/.


Features
========

- Draw Circuit Connections: Press 'C' to enable or click the 'W' quick option to draw circuit connections


Tools
=====

- Wire tool: Connects circuit inputs and outputs. Make connections with line draw tool from output to input component.
- Unplug tool: Removes wire connections from input component.


Components
==========

- DIGI: Digitizer. Converts an electrical signal to circuit data.
- DIOD: Diode. Converts circuit output to an electrical signal.
- BUFR: Buffer. Allows a signal to pass through.
- JOIN: Joins many signals into one, acting like an OR gate with unlimited inputs.
- LED: Light emitting diode. Lights up when it recieves a circuit signal.
- TRNS: Transistor. Allows a signal to pass through when enabled. 
- OR gate. Outputs the bitwise OR of the input values.
- AND gate. Outputs the bitwise AND of the input values.
- NOT gate. Inverts the input signal.
- NOR gate. Outputs the inverted bitwise OR of the input values.
- NAND gate. Outputs the inverted bitwise AND of the input values.
- XOR gate. Outputs a signal only if one of the inputs carries a signal.
- XNOR gate. Outputs a signal only if both inputs are the same.

