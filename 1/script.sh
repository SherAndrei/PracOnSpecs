prog="./process"
tests="./../tests"

echo "=========================== test1 ==========================="
${prog} ${tests}/a.txt ${tests}/b.txt ${tests}/c.txt \
        ${tests}/d.txt ${tests}/e.txt ${tests}/f.txt \
        ${tests}/g.txt ${tests}/h.txt ${tests}/i.txt \
        ${tests}/j.txt 
echo "=========================== test2 ==========================="        
${prog} ${tests}/b.txt ${tests}/b.txt ${tests}/b.txt
echo "=========================== test3 ==========================="        
${prog} ${tests}/ab.txt ${tests}/b.txt ${tests}/bc.txt
echo "=========================== test4 ==========================="        
${prog} ${tests}/cs.txt ${tests}/bad.txt ${tests}/b.txt
echo "=========================== test5 ==========================="        
${prog} ${tests}/empty.txt