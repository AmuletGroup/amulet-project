# Copyright 2016 by the Trustees of Dartmouth College 
# and Clemson University, and distributed under the terms
# of the "Dartmouth College Non-Exclusive Research Use Source 
# Code License Agreement" (for NON-COMMERCIAL research purposes only),
# as detailed in a file named LICENSE.pdf within this repository.

import subprocess
import os

LOG_SIZE = 4 # log size in bytes
LOG_OFFSET = 6 # offset in bytes from start of FRAM analytics section
ADC_REF = 2.8 # This is the voltage of the regulator
ADC_BITS = 12
ADC_MULTIPLIER = 2 # Set by voltage divider in hardware

# Get hexout info from attached device
cmd = ['mspdebug', 'tilib', '"hexout 0xD000 258 analytics_dump.hex"']
process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
process.wait()

# Load dumped HEX file
lines = [line.rstrip('\n') for line in open('analytics_dump.hex ')]

print "================ BEGIN HEXDUMP ===================="
# Get data info from file in Intel HEX format
all_data = ''
for line in lines:
	# We dont use any of the meta information currently, could use it to compute checksum
	# Or do more sophisticated logging
	start_code = 		line[0:1]
	byte_count = 		int(line[1:3],16)
	address = 			int(line[3:7],16)
	record_type = 		int(line[7:9],16)
	data = 				line[9:-2]
	checksum = 			int(line[-2:], 16)
	print "Byte count",byte_count,"Address",address,"Record type",record_type,"Data",data,"Checksum",checksum

	all_data = all_data + data

print "================ END HEXDUMP ===================="

# Convert to hex bytes (in little endian)
all_data = [all_data[i:i+2] for i in range(0, len(all_data), 2)]

# Get stats
print "\n\n=============== ANALYTICS ==================="
reset_count = int(all_data[0],16) + (int(all_data[1],16) << 8)
num_batt_logs = int(all_data[2],16)
print "Number of Resets:",reset_count
print "Number of battery logs:",num_batt_logs

# Get batt log
for i in xrange(0,num_batt_logs):
	start = i*LOG_SIZE+LOG_OFFSET
	end = start + LOG_SIZE
	log_bytes = [int(x,16) for x in all_data[start:end]]
	battery_voltage_adc = (log_bytes[0]+(log_bytes[1]<<8))
	battery_voltage = float(battery_voltage_adc) * (ADC_REF / float(pow(2,ADC_BITS))) * ADC_MULTIPLIER
	minutes = log_bytes[2]
	hours = log_bytes[3]
	print "#%d Battery Voltage=%.4fV Time=%d:%d" % (i,battery_voltage,hours,minutes)
	
# Now delete the hex dump file
os.remove('analytics_dump.hex ')