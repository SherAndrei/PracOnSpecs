#!/bin/bash

for ((n = 1; n < 5; n++))
    do ./script.sh > log$n.txt
done

for ((n = 2; n < 5; n++))
    do diff log1.txt log$n.txt
done
