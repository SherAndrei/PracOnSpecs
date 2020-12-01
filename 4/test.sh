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

function testFile {
    if [ $# -ne 4 ]
    then
        echo "Usage: <length> <expecting answer> <expecting result> <filename>"
        echo "Your usage $#: "$@""
    else 
        for ((i = 1; i <= $1 ; i++))
        do
            echo "./a.out $1 $i $2"
            assert "$($prog $1 $i $2 | grep Answer)" "$3"
            assert "$($prog $1 $i $2 | grep Result)" "$4"
        done
    fi
}

function testSmall {
    echo "1 2 3 2 1 2 3 2 1 2" > sin
    local result="Result   = 1.000000 2.000000 1.000000 2.000000 3.000000 2.000000 1.000000 2.000000 1.000000 2.000000"
    local answer="Answer: 6"
    testFile 10 sin "$answer" "$result"
    rm sin

    echo "0 1 10 1 0" > mount
    local result="Result   = 0.000000 1.000000 0.000000 1.000000 0.000000"
    local answer="Answer: 1"    
    testFile 5 mount "$answer" "$result"
    rm mount

    echo "-1 1 -10 1 -1" > pit
    local result="Result   = -1.000000 1.000000 -1.000000 1.000000 -1.000000"
    local answer="Answer: 1"    
    testFile 5 pit "$answer" "$result"
    rm pit

    echo "1" > one
    local result="Result   = 1.000000"
    local answer="Answer: 0"
    testFile 1 one "$answer" "$result"
    rm one

    echo "2 2" > two
    local result="Result   = 2.000000 2.000000"
    local answer="Answer: 0"
    testFile 2 two "$answer" "$result"
    rm two

    echo "3 3 3" > three
    local result="Result   = 3.000000 3.000000 3.000000"
    local answer="Answer: 0"
    testFile 3 three "$answer" "$result"
    rm three
    
    echo "4 4 4 4" > four
    local result="Result   = 4.000000 4.000000 4.000000 4.000000"
    local answer="Answer: 0"
    testFile 4 four "$answer" "$result"
    rm four

    echo "1 1 -2 -2 1 1" > six
    local result="Result   = 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000"
    local answer="Answer: 2"
    testFile 6 six "$answer" "$result"
    rm six

    echo "1 1 -2 -2 1 1" > six
    local result="Result   = 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000"
    local answer="Answer: 2"
    testFile 6 six "$answer" "$result"
    rm six

    touch empty
    local result="Result   = 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000"
    local answer="Answer: 2"
    testFile 6 six "$answer" "$result"
    rm six
}

function testMedium {
    local answer="Answer: 96"
    for ((i = 1; i <= 100; i++))
    do
        echo "./a.out 100 $i"
        assert "$($prog 100 $i | grep Answer)" "$answer"
    done
}

function testBig {
    local answer="Answer: 99999996"
    for ((i = 1; i <= 20; i++))
    do
        echo "./a.out 100000000 $i"
        assert "$($prog 100000000 $i | grep Answer)" "$answer"
    done
}

for mytest in testSmall testMedium testBig 
do 
    echo "=========================== $mytest ==========================="
    $mytest
    echo "Done!"
done
