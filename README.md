# videostreaming
A software using opencv to grab frames from a webcamera, compressing to jpeg and streaming over UDP.

It is optimized for lower performance embedded systems like raspberry Pi, Odroid etc.
Tested on the Odroid C1 and achieves 20fps over the network with a Mobius webcamera and 640x480 frame size.

Required libraries: Opencv

You will have to edit the INCLUDEPATH and LIBS in Makefile to point to your opencv libraries.
