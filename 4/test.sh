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
    local orig="              Original = 0.000000 1.000000 2.000000 3.000000 4.000000 0.000000"
    local result="Result   = 0.000000 1.000000 2.000000 0.500000 4.000000 0.000000"
    for ((i = 1; i <= 7; i++))
    do
        echo -n "./a.out 6 $i "
        assert "$($prog 6 $i | grep Result)" "$result"
        echo "$orig"
    done    
}

function testAnswer {
    local answer="Answer: 2"
    for ((i = 1; i <= 7; i++))
    do
        echo -n "./a.out 6 $i "
        assert "$($prog 6 $i | grep Answer)" "$answer"
    done
}

function testSwapCurrentToPrevPrevDividedBy2 {
    local orig="              Original = 0.000000 1.000000 2.000000 3.000000 4.000000 5.000000 6.000000 7.000000 8.000000 9.000000"
    local result="Result   = 0.000000 1.000000 0.000000 0.500000 1.000000 1.500000 2.000000 2.500000 3.000000 3.500000"
    local answer="Answer: 8"
    for ((i = 1; i <= 10; i++))
    do
        echo "./a.out 10 $i"
        assert "$($prog 10 $i | grep Answer)" "$answer"
        assert "$($prog 10 $i | grep Result)" "$result"
        echo "$orig"
    done
}

for mytest in testSwapCurrentToPrevPrevDividedBy2
do 
    echo "=========================== $mytest ==========================="
    $mytest
    echo "Done!"
done

# for mytest in testAnswer testResult
# do 
#     echo "=========================== $mytest ==========================="
#     $mytest
#     echo "Done!"
# done