#!/bin/bash
echo '======================================================='
echo ' Batch run of the analysis code for offline analysis'
echo '======================================================='

# 인수가 없으면 오류 메시지 출력
if [ "$#" -eq 0 ]; then
    echo "Usage: $0 [comm|phys] (start_run) (end_run)"
    exit 1
fi

# 인수들을 차례로 for문으로 처리
for arg in $(seq $2 $3); do
    formatted_arg=$(printf "%05d" $arg)
    if [ "$1" = "comm" ]; then
        echo "Analyzing ./rawdata/KO_comm/run${formatted_arg}.mid.lz4"
        ./rakobra ./rawdata/KO_comm/run${formatted_arg}.mid.lz4
        mv output000${formatted_arg}.root ./root/comm
    elif [ "$1" = "phys" ]; then
        echo "Analyzing ./rawdata/KO_24_30/run${formatted_arg}.mid.lz4"
        ./rakobra ./rawdata/KO_24_30/run${formatted_arg}.mid.lz4
        mv output000${formatted_arg}.root ./root/phys
    fi
done

