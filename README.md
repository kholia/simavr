simavr - a lean and mean Atmel AVR simulator for linux
======

_simavr_ is a new AVR simulator for linux, or any platform that uses avr-gcc. It uses
avr-gcc's own register definition to simplify creating new targets for supported AVR
devices. The core was made to be small and compact, and hackable so allow quick
prototyping of an AVR project. The AVR core is now stable for use with parts
with <= 128KB flash, and with preliminary support for the bigger parts. The
simulator loads ELF files directly, and there is even a way to specify simulation
parameters directly in the emulated code using an .elf section. You can also
load multipart HEX files.

Installation
------------
On OSX, we recommend using [homebrew](https://brew.sh):

    brew tap osx-cross/avr
    brew install --HEAD simavr

On Ubuntu, SimAVR is available in the Bionic package source:

    apt-get install simavr

(Note that the command is made available under the name `simavr` not `run_avr`.)

Otherwise, `make` is enough to just start using __bin/simavr__. To install the __simavr__ command system-wide, `make install RELEASE=1`.

Supported IOs
--------------
* _eeprom_
* _watchdog_
* _IO ports_ (including pin interrupts)
* _Timers_, 8 &16 (Normal, CTC and Fast PWM, the overflow interrupt too)
* The _UART_, including tx & rx interrupts (there is a loopback/local echo test mode too)
* _SPI_, master/slave including the interrupt
* _i2c_ Master & Slave
* External _Interrupts_, INT0 and so on.
* _ADC_
* Self-programming (ie bootloaders!)

Emulated Cores (very easy to add new ones!)
--------------
+ ATMega2560
+ AT90USB162 (with USB!)
+ ATMega1281
+ ATMega1280
+ ATMega128
+ ATMega128rf1
+ ATMega16M1
+ ATMega169
+ ATMega162
+ ATMega164/324/644
+ ATMega48/88/168/328
+ ATMega8/16/32
+ ATTiny25/45/85
+ ATTIny44/84
+ ATTiny2313/2313v
+ ATTiny13/13a

Extras:
-------
* fully working _gdb_ support including some pretty cool “passive modes”.
* There is also very easy support for “VCD” (Value Change Dump) that can be visualized
graphically as “waveforms” with tools like _gtkwave_ (see below).
* There are a few examples of real life firmwares running on simavr, including OpenGL rendering of the display…
* There is support for _Arduino_, but no IDE integration

Documentation And Further Information
-------------------------------------

* Manual / Developer Guide: https://github.com/buserror-uk/simavr/blob/master/doc/manual/manual.pdf?raw=true
* Examples: https://github.com/buserror-uk/simavr/tree/master/examples
* Mailing List: http://groups.google.com/group/simavr
* IRC: _#simavr_ on Freenode

Contributing
------------

Patches are always welcome! Please submit your changes via Github pull requests.

VCD Support -- built in logic analyzer
-----------
_simavr_ can output most of its pins, firmware variables, interrupts and a few other
things as signals to be dumped into a file that can be plotted using gtkwave for
further, precise analysis.
A firmware can contain instructions for _simavr_ to know what to trace, and the file is
automatically generated.
Example:

	const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
		{ AVR_MCU_VCD_SYMBOL("UDR0"), .what = (void*)&UDR0, },
		{ AVR_MCU_VCD_SYMBOL("UDRE0"), .mask = (1 << UDRE0), .what = (void*)&UCSR0A, },
	};

Will tell _simavr_ to generate a trace everytime the UDR0 register changes and everytime
the interrupt is raised (in UCSR0A). The *_MMCU_* tag tells gcc that it needs compiling,
but it won't be linked in your program, so it takes literally zero bytes, this is a code
section that is private to _simavr_, it's free!
A program running with these instructions and writing to the serial port will generate
a file that will display:

	$ ./simavr/run_avr tests/atmega88_example.axf
	AVR_MMCU_TAG_VCD_TRACE 00c6:00 - UDR0
	AVR_MMCU_TAG_VCD_TRACE 00c0:20 - UDRE0
	Loaded 1780 .text
	Loaded 114 .data
	Loaded 4 .eeprom
	Starting atmega88 - flashend 1fff ramend 04ff e2end 01ff
	atmega88 init
	avr_eeprom_ioctl: AVR_IOCTL_EEPROM_SET Loaded 4 at offset 0
	Creating VCD trace file 'gtkwave_trace.vcd'
	Read from eeprom 0xdeadbeef -- should be 0xdeadbeef..
	Read from eeprom 0xcafef00d -- should be 0xcafef00d..
	simavr: sleeping with interrupts off, quitting gracefully

And when the file is loaded in gtkwave, you see:
![gtkwave](https://github.com/buserror-uk/simavr/raw/master/doc/img/gtkwave1.png)

You get a very precise timing breakdown of any change that you add to the trace, down
to the AVR cycle.

Example:
--------
_simavr_ is really made to be the center for emulating your own AVR projects, not just
a debugger, but also the emulating the peripherals you will use in your firmware, so
you can test and develop offline, and now and then try it on the hardware.

You can also use _simavr_ to do test units on your shipping firmware to validate it
before you ship a new version, to prevent regressions or mistakes.

_simavr_ has a few 'complete projects/ that demonstrate this, most of them were made
using real hardware at some point, and the firmware binary is _exactly_ the one that
ran on the hardware. The key here is to emulate the _parts_ or peripherals that
are hooked to the AVR. Of course, you don't have to emulate the full hardware, you just
need to generate the proper stimulus so that the AVR is fooled.

HD44780 LCD Board Demo
----------------------

![lcd](https://github.com/buserror-uk/simavr/raw/master/doc/img/hd44780.png)

This example board hooks up an Atmega48 to an emulated HD44780 LCD and display a running
counter in the 'lcd'. Everything is emulated, the firmware runs exactly like this
on a real hardware.

![lcd-gtkwave](https://github.com/buserror-uk/simavr/raw/master/doc/img/hd44780-wave.png)

And this is a gtkwave trace of what the firmware is doing. You can zoom in, measure, etc
in gtkwave, select trades to see etc.

Quite a few other examples are available!

Usage
-----

Install build dependencies:

```
sudo apt-get install libelf-dev freeglut3 freeglut3-dev gcc-avr avr-libc gcc make
```

Clone and build the latest `simavr` code:

```
git clone https://github.com/kholia/simavr.git

cd simavr

make
```

Run sample programs:

```
./simavr/run_avr -f 16000000 -m atmega328p ArduinoUniqueID.ino.elf

./simavr/run_avr -f 16000000 -m atmega328p ArduinoUniqueID.ino.hex
```

Sample run:

```
$ ./simavr/run_avr -f 16000000 -m atmega328p ArduinoUniqueID.ino.hex
Loaded 1966 .text at address 0x0
Loaded 42 .data
in setup()..
in loop()..
5D000C945D00945D00..
in loop()..
5D000C945D00945D00..
in loop()..
5D000C945D00945D00..
in loop()..
5D000C945D00945D00..
...
```

GDB usage:

```
$ ./simavr/run_avr -g -f 16000000 -m atmega328p ArduinoUniqueID.ino.elf
Loaded 1966 .text at address 0x0
Loaded 42 .data
avr_gdb_init listening on port 1234
...

$ avr-gdb ArduinoUniqueID.ino.elf
GNU gdb (GDB) 10.1.90.20210103-git
...
(gdb) target remote :1234
Remote debugging using :1234
0x00000000 in __vectors ()
(gdb) break main
(gdb) c
Continuing.

Breakpoint 1, main () at /home/.../hardware/avr/1.8.4/cores/arduino/main.cpp:35
35		init();
(gdb)
```


```
$ avr-objdump -S -j .sec1 -d -C -m avr5 ArduinoUniqueID.ino.hex
...
 616:	10 f4       	brcc	.+4      	;  0x61c
 618:	f0 e0       	ldi	r31, 0x00	; 0
 61a:	e0 e0       	ldi	r30, 0x00	; 0
 61c:	be 01       	movw	r22, r28
 61e:	62 5f       	subi	r22, 0xF2	; 242
 620:	7f 4f       	sbci	r23, 0xFF	; 255
 622:	e6 0f       	add	r30, r22
 624:	f7 1f       	adc	r31, r23
 626:	10 93 57 00 	sts	0x0057, r17	;  0x800057 <-- try to find this ;)
 62a:	e4 91       	lpm	r30, Z
 62c:	6e 2f       	mov	r22, r30
 62e:	70 e0       	ldi	r23, 0x00	; 0
 630:	90 e0       	ldi	r25, 0x00	; 0
 632:	80 e0       	ldi	r24, 0x00	; 0
 634:	4a e0       	ldi	r20, 0x0A	; 10
 636:	0e 94 9e 01 	call	0x33c	;  0x33c
```

Tracing patch:

```diff
diff --git a/simavr/sim/sim_core.c b/simavr/sim/sim_core.c
index 4bb2543..118afb5 100644
--- a/simavr/sim/sim_core.c
+++ b/simavr/sim/sim_core.c
@@ -1013,6 +1013,8 @@ run_one_again:
                                                        uint16_t z = avr->data[R_ZL] | (avr->data[R_ZH] << 8);
                                                        int op = opcode & 1;
                                                        STATE("lpm %s, (Z[%04x]%s)\n", avr_regname(d), z, op ? "+" : "");
+                                                       printf("Opcode -> %x\n", opcode);
+                                                       printf("lpm %s, (Z[%04x]%s)\n", avr_regname(d), z, op ? "+" : "");
                                                        _avr_set_r(avr, d, avr->flash[z]);
                                                        if (op) {
                                                                z++;
```

```
$ ./simavr/run_avr -m atmega328pb -f 16000000 ArduinoUniqueID8.ino.elf
Loaded 1878 .text at address 0x0
Loaded 34 .data
...
Opcode -> 91e4
lpm ZL, (Z[000e])
Opcode -> 91e4
lpm ZL, (Z[000f])
Opcode -> 91e4
lpm ZL, (Z[0010])
Opcode -> 91e4
lpm ZL, (Z[0011])
Opcode -> 91e4
lpm ZL, (Z[0012])
Opcode -> 91e4
lpm ZL, (Z[0013])
Opcode -> 91e4
lpm ZL, (Z[0015])
Opcode -> 91e4
lpm ZL, (Z[0016])
Opcode -> 91e4
lpm ZL, (Z[0017])
UniqueID: 00 0C 94 5D 00 94 5D 00 ..
...
```

```
$ avr-objdump -d ArduinoUniqueID8.ino.elf | grep -B 5 "e4 91"
 55a:	30 e0       	ldi	r19, 0x00	; 0
 55c:	20 e0       	ldi	r18, 0x00	; 0
 55e:	e2 0f       	add	r30, r18
 560:	f3 1f       	adc	r31, r19
 562:	40 93 57 00 	sts	0x0057, r20	; 0x800057 <__TEXT_REGION_LENGTH__+0x7e0057>
 566:	e4 91       	lpm	r30, Z
```
