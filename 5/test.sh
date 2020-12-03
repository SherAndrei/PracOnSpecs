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
    if [ $# -ne 5 ]
    then
        echo "Usage: <length> <filename> <expecting answer> <expecting result> <expecting mean>"
        echo "Your usage $#: "$@""
    else 
        for ((i = 1; i <= $1 ; i++))
        do
            echo "./a.out $1 $i $2: "
            echo -n "Answer: "
            assert "$($prog $1 $i $2 | grep Answer)" "$3"
            echo -n "Result: "
            assert "$($prog $1 $i $2 | grep Result)" "$4"
            echo -n "Mean:   "
            assert "$($prog $1 $i $2 | grep Mean)" "$5"
        done
    fi
}

function testMin {
    echo "1 1 2 3" > 1123
    local result="Result   = 1.000000 1.000000 2.000000 3.000000"
    local answer="Answer: 1"
    local mean="Mean: 1.000000"
    testFile 4 1123 "$answer" "$result" "$mean"
    rm 1123

    echo "1 5 2 3" > 1523
    local result="Result   = 1.000000 5.000000 2.000000 3.000000"
    local answer="Answer: 0"
    local mean="Mean: 0.000000"
    testFile 4 1523 "$answer" "$result" "$mean"
    rm 1523

    echo "1 3 4 6 9" > 13469
    local result="Result   = 1.000000 3.500000 3.500000 6.000000 9.000000"
    local answer="Answer: 2"
    local mean="Mean: 3.500000"
    testFile 5 13469 "$answer" "$result" "$mean"
    rm 13469

    echo "1 3 4 6 1 9" > 134619
    local result="Result   = 1.000000 3.000000 4.000000 6.000000 1.000000 9.000000"
    local answer="Answer: 1"
    local mean="Mean: 3.000000"
    testFile 6 134619 "$answer" "$result" "$mean"
    rm 134619

    echo "1 3 4 6 1 20" > 1346120
    local result="Result   = 1.000000 4.500000 4.000000 4.500000 1.000000 20.000000"
    local answer="Answer: 2"
    local mean="Mean: 4.500000"
    testFile 6 1346120 "$answer" "$result" "$mean"
    rm 1346120

    echo "6 5 4 3 2 1" > 654321
    local result="Result   = 6.000000 5.000000 4.000000 3.000000 2.000000 1.000000"
    local answer="Answer: 0"
    local mean="Mean: 0.000000"
    testFile 6 654321 "$answer" "$result" "$mean"
    rm 654321

    echo "1 2 3 5 8 13" > 1235813
    local result="Result   = 1.000000 3.333333 3.333333 3.333333 8.000000 13.000000"
    local answer="Answer: 3"
    local mean="Mean: 3.333333"
    testFile 6 1235813 "$answer" "$result" "$mean"
    rm 1235813
}

function testMedium {
    local answer="Answer: 97"
    for ((i = 1; i <= 100; i++))
    do
        echo -n "./a.out 100 $i: "
        assert "$($prog 100 $i | grep Answer)" "$answer"
    done
}

function testBig {
    local answer="Answer: 99999997"
    for ((i = 1; i <= 20; i++))
    do
        echo -n "./a.out 100000000 $i: "
        assert "$($prog 100000000 $i | grep Answer)" "$answer"
    done
}

for mytest in testMin testMedium testBig
do 
    echo "=========================== $mytest ==========================="
    $mytest
    echo "Done!"
done
