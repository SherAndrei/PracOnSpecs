#!/bin/bash
prog="./a.out"
tests="./tests"
exec 1> result.txt

function mytest {
if [ $# -ne 3 ] 
then
    echo "Usage: <filename> [RESULT] [EXPECTING]"
else 
    if [ $2 -eq $3 ] 
    then
        echo -n  "Correct! "
    else 
        echo -n  "Wrong!   "
    fi
    echo "Result $2 | Expecting $3 in $1"
fi

}

# echo "=========================== test1 ==========================="
mytest 0    $($prog $tests/0) 1
mytest 1    $($prog $tests/1) 1
mytest 00   $($prog $tests/00) 2
mytest 01   $($prog $tests/01) 1
mytest 10   $($prog $tests/10) 1
mytest 11   $($prog $tests/11) 2
mytest 000  $($prog $tests/000) 3
mytest 001  $($prog $tests/001) 2
mytest 010  $($prog $tests/010) 2
mytest 011  $($prog $tests/011) 2
mytest 100  $($prog $tests/100) 2
mytest 101  $($prog $tests/101) 1
mytest 110  $($prog $tests/110) 2
mytest 111  $($prog $tests/111) 3
mytest 210  $($prog $tests/210) 1
mytest 012  $($prog $tests/012) 1
mytest 0000 $($prog $tests/0000) 4
mytest 0001 $($prog $tests/0001) 3
mytest 0010 $($prog $tests/0010) 3
mytest 0100 $($prog $tests/0100) 3
mytest 0101 $($prog $tests/0101) 2
mytest 0110 $($prog $tests/0110) 2
mytest 0011 $($prog $tests/0011) 3
mytest 0111 $($prog $tests/0111) 3
mytest 0110 $($prog $tests/0110) 2
mytest 1001 $($prog $tests/1001) 2
mytest 1111 $($prog $tests/1111) 4
# mytest =========================== test2 ==========================="
# echo 'a.txt b.txt: Correct result 0 | Our:'
# ${prog} ${tests}/a.txt ${tests}/b.txt
# echo "=========================== test3 ==========================="        
# echo 'b.txt b.txt b.txt: Correct result 0 | Our:'
# ${prog} ${tests}/b.txt ${tests}/b.txt ${tests}/b.txt
# echo "=========================== test4 ==========================="        
# echo 'ab.txt b.txt bc.txt:'
# ${prog} ${tests}/ab.txt ${tests}/b.txt ${tests}/bc.txt
# echo "=========================== test5 ==========================="        
# echo 'cs.txt bad.txt b.txt:'
# ${prog} ${tests}/cs.txt ${tests}/bad.txt ${tests}/b.txt
# echo "=========================== test6 ==========================="        
# echo 'empty.txt:'
# ${prog} ${tests}/empty.txt
# echo "=========================== test7 ==========================="        
# echo 'Correct result 0 | Our:'
# ${prog} ${tests}/a.txt ${tests}/b.txt ${tests}/c.txt \
#         ${tests}/d.txt ${tests}/e.txt ${tests}/f.txt \
#         ${tests}/g.txt ${tests}/h.txt ${tests}/i.txt \
#         ${tests}/j.txt ${tests}/k.txt 
# echo "=========================== test8 ==========================="        
# echo 'Correct result 35 | Our:'
# ${prog} ${tests}/a.txt ${tests}/b.txt ${tests}/c.txt \
#         ${tests}/d.txt ${tests}/e.txt ${tests}/f.txt \
#         ${tests}/g.txt ${tests}/h.txt ${tests}/i.txt \
#         ${tests}/j-1.txt ${tests}/k.txt 
# echo "=========================== test9 ==========================="        
# echo 'bad.txt k.txt'
# ${prog} ${tests}/bad.txt ${tests}/k.txt