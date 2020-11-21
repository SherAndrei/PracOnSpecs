#!/bin/bash
prog="./a.out"
tests="./tests"
exec 1> result.txt

function assert {
    if [ $# -ne 3 ] 
    then
        echo "Usage: [SEQUENCE] [RESULT] [EXPECTING]"
    else 
        if [ $2 -ne $3 ] 
        then
            echo -n  "Wrong! "
            echo "Result $2 | Expecting $3 in $1"
        fi
fi
}

function test1 {
    assert empty  $($prog $tests/empty) 0
    assert 0      $($prog $tests/0) 1
    assert 1      $($prog $tests/1) 1
    assert 00     $($prog $tests/00) 2
    assert 01     $($prog $tests/01) 1
    assert 10     $($prog $tests/10) 1
    assert 11     $($prog $tests/11) 2
    assert 000    $($prog $tests/000) 3
    assert 001    $($prog $tests/001) 2
    assert 010    $($prog $tests/010) 2
    assert 011    $($prog $tests/011) 2
    assert 100    $($prog $tests/100) 2
    assert 101    $($prog $tests/101) 1
    assert 110    $($prog $tests/110) 2
    assert 111    $($prog $tests/111) 3
    assert 210    $($prog $tests/210) 1
    assert 012    $($prog $tests/012) 1
    assert 0000   $($prog $tests/0000) 4
    assert 0001   $($prog $tests/0001) 3
    assert 0010   $($prog $tests/0010) 3
    assert 0100   $($prog $tests/0100) 3
    assert 0101   $($prog $tests/0101) 2
    assert 0110   $($prog $tests/0110) 2
    assert 0011   $($prog $tests/0011) 3
    assert 0111   $($prog $tests/0111) 3
    assert 0110   $($prog $tests/0110) 2
    assert 1001   $($prog $tests/1001) 2
    assert 1111   $($prog $tests/1111) 4
    assert 10101  $($prog $tests/10101) 2
    assert 01010  $($prog $tests/01010) 3
    assert 001122 $($prog $tests/001122) 4
}

function test2 {
    assert "empty|empty" $($prog $tests/empty $tests/empty) 0
    assert "0|empty"     $($prog $tests/0 $tests/empty) 1
    assert "empty|1"     $($prog $tests/empty $tests/1) 1
    assert "0|0"     $($prog $tests/0 $tests/0) 2
    assert "0|1"     $($prog $tests/0 $tests/1) 1
    assert "1|0"     $($prog $tests/1 $tests/0) 1
    assert "1|1"     $($prog $tests/1 $tests/1) 2
    assert "0|00"    $($prog $tests/0 $tests/00) 3
    assert "00|1"    $($prog $tests/00 $tests/1) 2
    assert "0|10"    $($prog $tests/0 $tests/10) 2
    assert "01|1"    $($prog $tests/01 $tests/1) 2
    assert "1|00"    $($prog $tests/1 $tests/00) 2
    assert "10|1"    $($prog $tests/10 $tests/1) 1
    assert "1|10"    $($prog $tests/1 $tests/10) 2
    assert "11|1"    $($prog $tests/11 $tests/1) 3
    assert "2|10"    $($prog $tests/2 $tests/10) 1
    assert "01|2"    $($prog $tests/01 $tests/2) 1
    assert "00|00"   $($prog $tests/00 $tests/00) 4
    assert "00|01"   $($prog $tests/00 $tests/01) 3
    assert "00|10"   $($prog $tests/00 $tests/10) 3
    assert "01|00"   $($prog $tests/01 $tests/00) 3
    assert "01|01"   $($prog $tests/01 $tests/01) 2
    assert "01|10"   $($prog $tests/01 $tests/10) 2
    assert "00|11"   $($prog $tests/00 $tests/11) 3
    assert "01|11"   $($prog $tests/01 $tests/11) 3
    assert "01|10"   $($prog $tests/01 $tests/10) 2
    assert "10|01"   $($prog $tests/10 $tests/01) 2
    assert "11|11"   $($prog $tests/11 $tests/11) 4
    assert "10|101"  $($prog $tests/10 $tests/101) 2
    assert "010|10"  $($prog $tests/010 $tests/10) 3
    assert "001|122" $($prog $tests/001 $tests/122) 4
}

function test3 {
    assert "empty|empty|empty" $($prog $tests/empty $tests/empty $tests/empty) 0
    assert "empty|0|empty"     $($prog $tests/empty $tests/0 $tests/empty) 1
    assert "empty|empty|1"     $($prog $tests/empty $tests/empty $tests/1) 1
    assert "0|0|empty"     $($prog $tests/0 $tests/0 $tests/empty) 2
    assert "0|empty|1"     $($prog $tests/0 $tests/empty $tests/1) 1
    assert "empty|1|0"     $($prog $tests/empty $tests/1 $tests/0) 1
    assert "1|empty|1"     $($prog $tests/1 $tests/empty $tests/1) 2
    assert "0|0|0"    $($prog $tests/0 $tests/0 $tests/0) 3
    assert "0|0|1"    $($prog $tests/0 $tests/0 $tests/1) 2
    assert "0|1|0"    $($prog $tests/0 $tests/1 $tests/0) 2
    assert "0|1|1"    $($prog $tests/0 $tests/1 $tests/1) 2
    assert "1|0|0"    $($prog $tests/1 $tests/0 $tests/0) 2
    assert "1|0|1"    $($prog $tests/1 $tests/0 $tests/1) 1
    assert "1|1|0"    $($prog $tests/1 $tests/1 $tests/0) 2
    assert "1|1|1"    $($prog $tests/1 $tests/1 $tests/1) 3
    assert "2|1|0"    $($prog $tests/2 $tests/1 $tests/0) 1
    assert "0|1|2"    $($prog $tests/0 $tests/1 $tests/2) 1
    assert "0|0|00"   $($prog $tests/0 $tests/0 $tests/00) 4
    assert "00|0|1"   $($prog $tests/00 $tests/0 $tests/1) 3
    assert "0|0|10"   $($prog $tests/0 $tests/0 $tests/10) 3
    assert "01|0|0"   $($prog $tests/01 $tests/0 $tests/0) 3
    assert "0|1|01"   $($prog $tests/0 $tests/1 $tests/01) 2
    assert "01|1|0"   $($prog $tests/01 $tests/1 $tests/0) 2
    assert "0|0|11"   $($prog $tests/0 $tests/0 $tests/11) 3
    assert "01|1|1"   $($prog $tests/01 $tests/1 $tests/1) 3
    assert "0|1|10"   $($prog $tests/0 $tests/1 $tests/10) 2
    assert "10|0|1"   $($prog $tests/10 $tests/0 $tests/1) 2
    assert "1|1|11"   $($prog $tests/1 $tests/1 $tests/11) 4
    assert "10|10|1"  $($prog $tests/10 $tests/10 $tests/1) 2
    assert "0|10|10"  $($prog $tests/0 $tests/10 $tests/10) 3
    assert "00|11|22" $($prog $tests/00 $tests/11 $tests/22) 4
}

for mytest in test1 test2 test3
do 
    echo "=========================== $mytest ==========================="
    $mytest
    echo "Done!"
done