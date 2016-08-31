##### analytics.py
This script communicates with the amulet device using MSPDEBUG, and gets a hex dump of the FRAM section of memory that stores the crash/reset and battery voltage logs.

The battery voltage logs are only 256 entries large, and only gather every ten minutes. The log looks like this, with timestamps in 24 hour time, and battery voltage precalculated from the reference:

	#0 Battery Voltage=4.2010V Time=5:10
	#1 Battery Voltage=4.19020V Time=5:20

The FRAM reset counter is a single 16-bit counter that increments every time the device is restarted or a PUC is recieved. 


Just run as a normal python program, make sure the amulet is connected to the computer:

	python analytics.py


##### Understanding Binary size

Show size of objects and their location in FRAM, sorted, top 100

	msp430-elf-readelf -W -s gen/msp430fr5969.out | awk '$4 == "OBJECT" { print }' | sort -k 3 -n -r | head -n 100 | c++filt

Show function size, sorted

	msp430-elf-nm --print-size --size-sort --radix=d gen/msp430fr5969.out