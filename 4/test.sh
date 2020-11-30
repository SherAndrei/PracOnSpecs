#!/bin/bash
prog="./a.out"
exec 2>&1
# exec 1> result.txt

function assert {
    if [ $# -ne 2 ]
    then
        echo "Usage: [RESULT] [EXPECTING]"
        echo "Your usage: "$@""
    else 
        if [ "$1" != "$2" ]
        then
            echo "Wrong! "
            echo "  Result    : $1"
            echo "  Expecting : $2"
        else
            echo "Correct!"
        fi
fi
}

function testResult {
    local result="Result   = 0.000000 1.000000 2.000000 0.500000 1.500000 2.500000 3.500000 2.000000 3.000000 4.000000"
    for ((i = 1; i <= 10; i++))
    do
        assert "$($prog 10 $i | grep Result)" "$result"
    done    
}

function testAnswer {
    local answer="Answer: 6"
    for ((i = 1; i <= 10; i++))
    do
        assert "$($prog 10 $i | grep Answer)" "$answer"
    done
}

for mytest in testAnswer testResult
do 
    echo "=========================== $mytest ==========================="
    $mytest
    echo "Done!"
done