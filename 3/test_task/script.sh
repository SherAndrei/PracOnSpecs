#!/bin/bash
prog="./a.out"
tests="./test_task"
exec 1> result.txt

function assert {
    if [ $# -ne 3 ] 
    then
        echo "Usage: [SEQUENCE] [RESULT] [EXPECTING]"
        echo "Hint: if usage is correct try run it yourself, possible segfault"
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


for mytest in test1 test2
do 
    echo "=========================== $mytest ==========================="
    $mytest
    echo "Done!"
done