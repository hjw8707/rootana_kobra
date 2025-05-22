#!/bin/bash
echo '======================================================='
echo ' Batch run of the analysis code for offline analysis'
echo '======================================================='

# 인수가 없으면 오류 메시지 출력
if [ "$#" -eq 0 ]; then
    echo "Usage: $0 [comm|phys] (start_run) (end_run)"
    exit 1
fi

echo "Analyzing $1 runs from $2 to $3"

if [ "$2" = "all_cs" ]; then
    if [ "$1" = "comm" ]; then
        echo "No comm runs"
        exit 0
    elif [ "$1" = "phys" ]; then
        $0 phys 223 226
        $0 phys 251 254
        $0 phys 259 267
        $0 phys 271 299
        $0 phys 304 310
        $0 phys 316 316
        $0 phys 318 345
        $0 phys 347 352
        $0 phys 361 362
        $0 phys 364 376
        $0 phys 378 400
        $0 phys 402 404
        $0 phys 410 413
        $0 phys 415 415
        $0 phys 420 422
        $0 phys 425 431
        $0 phys 433 440
        exit 0
    fi
fi

if [ "$2" = "all_bls" ]; then
    if [ "$1" = "comm" ]; then
        echo "No comm runs"
        exit 0
    elif [ "$1" = "phys" ]; then
        $0 phys 527 533
        $0 phys 537 540
        $0 phys 542 542
        $0 phys 546 551
        $0 phys 553 556
        exit 0
    fi
fi

if [ "$2" = "all_mdis" ]; then
    if [ "$1" = "comm" ]; then
        echo "No comm runs"
        exit 0
    elif [ "$1" = "phys" ]; then
        $0 phys 590 590
        $0 phys 592 596
        $0 phys 598 598
        $0 phys 601 607
        $0 phys 610 612
        $0 phys 615 617
        $0 phys 620 623
        $0 phys 625 625
        $0 phys 627 630
        $0 phys 632 632
        $0 phys 636 648
        $0 phys 651 653
        $0 phys 655 657
        $0 phys 660 661
        $0 phys 706 708
        $0 phys 713 740
        $0 phys 746 746
        $0 phys 748 748
        $0 phys 750 752
        $0 phys 754 759
        $0 phys 763 765
        $0 phys 767 767
        $0 phys 769 771
        $0 phys 773 789
        exit 0
    fi
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

