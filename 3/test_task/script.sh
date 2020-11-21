#!/bin/bash
prog="./a.out"
tests="./test_task"
exec 2>&1
exec 1> result.txt

function assert {
    if [ $# -ne 3 ] 
    then
        echo "Usage: [SEQUENCE] [RESULT] [EXPECTING]"
        echo "Your usage: $@"
    else 
        if [ $2 -ne $3 ] 
        then
            echo -n  "Wrong! "
            echo "Result $2 | Expecting $3 in $1"
        fi

fi
}

function test1 {
    assert "5|empty|0|empty|5" $($prog $tests/5 $tests/empty $tests/0 $tests/empty $tests/5) 1
    assert "5|0|5" $($prog $tests/5 $tests/0 $tests/5) 1
    assert "505" $($prog $tests/505) 1
}

function test2 {
    assert "5|empty|0|empty|4|5" $($prog $tests/5 $tests/empty $tests/0 $tests/empty $tests/4 $tests/5) 1
    assert "5|0|4|5" $($prog $tests/5 $tests/0 $tests/4 $tests/5) 1
    assert "5045" $($prog $tests/5045) 1
}


function test3 {
    assert "0|empty|0|empty|0|1" $($prog $tests/0 $tests/empty $tests/0 $tests/empty $tests/0 $tests/1) 3
    assert "0|empty|0|empty|01" $($prog $tests/0 $tests/empty $tests/0 $tests/empty $tests/01) 3
    assert "0|1|0|0" $($prog $tests/0 $tests/1 $tests/0 $tests/0) 3
    assert "01|0|0" $($prog $tests/01 $tests/0 $tests/0) 3
    assert "1000" $($prog $tests/1000) 3
}

function test4 {
    assert "54|01" $($prog $tests/54 $tests/01) 1
    assert "54|0|1" $($prog $tests/54 $tests/0  $tests/1) 1
    assert "54|0|1" $($prog $tests/54 $tests/0  $tests/1) 1
    assert "540|1" $($prog $tests/540 $tests/1) 1
    assert "54|empty|01" $($prog $tests/54 $tests/empty $tests/01) 1
    assert "54|0|empty|1" $($prog $tests/54 $tests/0 $tests/empty $tests/1) 1
    assert "54|0|1|empty" $($prog $tests/54 $tests/0 $tests/1 $tests/empty) 1
    assert "empty|540|1" $($prog $tests/empty $tests/540 $tests/1) 1
}


for mytest in test1 test2 test3 test4
do 
    echo "=========================== $mytest ==========================="
    $mytest
    echo "Done!"
done