#!/bin/bash
prog="./a.out"
exec 2>&1

for ((n = 0; n < 10; n++))
do
    echo "#######$n######"
    $prog 10 $n
done
