#!/bin/bash
echo '================================='
echo ' Batch run of the analysis code'
echo '================================='

if [ "$#" -eq 0 ]; then
    echo "Usage: $0 arg1 arg2 arg3 ..."
    exit 1
fi

# 인수들을 차례로 for문으로 처리
for arg in "$@"
do
    formatted_arg=$(printf "%05d" $arg)
    echo "Analyzing ~/online/data_2024/run${formatted_arg}.mid.lz4"
    ./ana ~/online/data_2024/run${formatted_arg}.mid.lz4
done

