#!/bin/bash

# Copyright 2016 by the Trustees of Dartmouth College 
# and Clemson University, and distributed under the terms
# of the "Dartmouth College Non-Exclusive Research Use Source 
# Code License Agreement" (for NON-COMMERCIAL research purposes only),
# as detailed in a file named LICENSE.pdf within this repository.


################################################################################
# Tested by Travis Peters on Mac OSX 10.10.5
#
# The following script uses the cinclude2dot.pl tool to generate a graph (dot 
# file) for the code dependencies. At a high level, the tool is pointed at the 
# root of a project, parses every file in that directory and below, and then 
# generates a dot file based on the "include" statements. The dot tool then 
# turns that dot file into a ps file which can be easily viewed (and even 
# convereted into a PDF or PNG if you so desire). 
#
# NOTE: This script is for convenience as it runs the commands you need to in 
# order to build the dot file, convert it to ps, and open it. 
################################################################################

# Clean up.
rm source.dot
rm source.ps

DEPENDDIR=$PWD              # where the dependency graphing tool lives.
SRCDIR="$PWD/../gen/core"   # where the root of the code lives.

# Change to root src directory
cd $SRCDIR
perl $DEPENDDIR/cinclude2dot.pl > $DEPENDDIR/source.dot

# Generate a directed dependency graph.
cd $DEPENDDIR
dot -Tps source.dot > source.ps
sleep 3

# Show me!
open source.ps