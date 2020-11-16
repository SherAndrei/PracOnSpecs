prog="./a.out"
tests="./../tests"

echo "=========================== test1 ==========================="
echo 'a.txt: Correct result 0 | Our:'
${prog} ${tests}/a.txt
echo 'b.txt: Correct result 0 | Our:'
${prog} ${tests}/b.txt
echo 'c.txt: Correct result 0 | Our:'
${prog} ${tests}/c.txt
echo 'd.txt: Correct result 1 | Our:'
${prog} ${tests}/d.txt
echo 'e.txt: Correct result 2 | Our:'
${prog} ${tests}/e.txt
echo 'f.txt: Correct result 0 | Our:'
${prog} ${tests}/f.txt
echo 'g.txt: Correct result 0 | Our:'
${prog} ${tests}/g.txt
echo 'h.txt: Correct result 0 | Our:'
${prog} ${tests}/h.txt
echo 'i.txt: Correct result 0 | Our:'
${prog} ${tests}/i.txt
echo 'j.txt: Correct result 0 | Our:'
${prog} ${tests}/j.txt
echo 'k.txt: Correct result 1 | Our:'
${prog} ${tests}/k.txt
echo 'empty.txt: Correct result 0 | Our:'
${prog} ${tests}/empty.txt
echo bad.txt:
${prog} ${tests}/bad.txt
echo "=========================== test2 ==========================="
echo 'a.txt b.txt: Correct result 0 | Our:'
${prog} ${tests}/a.txt ${tests}/b.txt
echo "=========================== test3 ==========================="        
echo 'b.txt b.txt b.txt: Correct result 0 | Our:'
${prog} ${tests}/b.txt ${tests}/b.txt ${tests}/b.txt
echo "=========================== test4 ==========================="        
echo 'ab.txt b.txt bc.txt:'
${prog} ${tests}/ab.txt ${tests}/b.txt ${tests}/bc.txt
echo "=========================== test5 ==========================="        
echo 'cs.txt bad.txt b.txt:'
${prog} ${tests}/cs.txt ${tests}/bad.txt ${tests}/b.txt
echo "=========================== test6 ==========================="        
echo 'empty.txt:'
${prog} ${tests}/empty.txt
echo "=========================== test7 ==========================="        
echo 'Correct result 0 | Our:'
${prog} ${tests}/a.txt ${tests}/b.txt ${tests}/c.txt \
        ${tests}/d.txt ${tests}/e.txt ${tests}/f.txt \
        ${tests}/g.txt ${tests}/h.txt ${tests}/i.txt \
        ${tests}/j.txt ${tests}/k.txt 
echo "=========================== test8 ==========================="        
echo 'Correct result 35 | Our:'
${prog} ${tests}/a.txt ${tests}/b.txt ${tests}/c.txt \
        ${tests}/d.txt ${tests}/e.txt ${tests}/f.txt \
        ${tests}/g.txt ${tests}/h.txt ${tests}/i.txt \
        ${tests}/j-1.txt ${tests}/k.txt 
echo "=========================== test9 ==========================="        
echo 'bad.txt k.txt'
${prog} ${tests}/bad.txt ${tests}/k.txt