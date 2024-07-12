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
root -l -b -q  macros/shift.C\($1\) | tee temproot.txt
./midcheck ~/online/KO_24_30/run${formatted_arg}.mid.lz4 | tee tempmidcheck.txt
o17=`sed -n 's/17O: \([0-9]*\)/\1/p' temproot.txt`
o18=`sed -n 's/18O: \([0-9]*\)/\1/p' temproot.txt`
o19=`sed -n 's/19O: \([0-9]*\)/\1/p' temproot.txt`
o20=`sed -n 's/20O: \([0-9]*\)/\1/p' temproot.txt`
o21=`sed -n 's/21O: \([0-9]*\)/\1/p' temproot.txt`
etime=`sed -n 's/Elapsed second: \([0-9]*\) sec/\1/p' tempmidcheck.txt`
o17rate=`echo "scale=3; ${o17} / ${etime}" | bc`
o18rate=`echo "scale=3; ${o18} / ${etime}" | bc`
o19rate=`echo "scale=3; ${o19} / ${etime}" | bc`
o20rate=`echo "scale=3; ${o20} / ${etime}" | bc`
o21rate=`echo "scale=3; ${o21} / ${etime}" | bc`
echo "17O rate: ${o17rate} pps"
echo "18O rate: ${o18rate} pps"
echo "19O rate: ${o19rate} pps"
echo "20O rate: ${o20rate} pps"
echo "21O rate: ${o21rate} pps"
rm temproot.txt
rm tempmidcheck.txt
