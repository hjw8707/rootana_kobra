#!/bin/bash

echo "Shell script for the shift"
if [ $# -ne 1 ]
then
    echo "Usage"
    echo ""
    echo "./shift.sh (run) (brho)"
    exit
fi

echo "Run the analysis"
./batch.sh $1
formatted_arg=$(printf "%05d" $1)
echo "Draw the monitors"
root -l -b -q  macros/shift.C\($1\) 
