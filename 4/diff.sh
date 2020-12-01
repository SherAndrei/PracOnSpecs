#!/bin/bash
prog="./a.out"
exec 2>&1
amount=10

for ((l = 1; l <= $amount; l++))
do
    exec > log$l 
    for ((n = 0; n < 50; n++))
    do
        echo "#######$n######"
        $prog 50 $n | grep 'Result\|Answer'
    done
done

exec > diff
for ((l = 1; l <= $amount; l++))
do
    echo "++++++++++++++++++ $l ++++++++++++++++++++++"
    diff --from-file=log$l log1 log2 log3 log4 log5 log6 log7 log8 log9 log10
done

for ((l = 1; l <= $amount; l++))
do
    rm log$l
done