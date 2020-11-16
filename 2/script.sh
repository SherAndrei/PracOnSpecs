prog="./a.out"
tests="./../tests"

echo "=========================== test1 ==========================="
echo a.txt
${prog} ${tests}/a.txt
echo b.txt
${prog} ${tests}/b.txt
echo c.txt
${prog} ${tests}/c.txt
echo d.txt
${prog} ${tests}/d.txt
echo e.txt
${prog} ${tests}/e.txt
echo f.txt
${prog} ${tests}/f.txt
echo g.txt
${prog} ${tests}/g.txt
echo h.txt
${prog} ${tests}/h.txt
echo i.txt
${prog} ${tests}/i.txt
echo j.txt
${prog} ${tests}/j.txt
echo k.txt
${prog} ${tests}/k.txt
echo empty.txt
${prog} ${tests}/empty.txt
echo bad.txt
${prog} ${tests}/bad.txt
echo "=========================== test2 ==========================="
echo a.txt b.txt
${prog} ${tests}/a.txt ${tests}/b.txt
echo "=========================== test3 ==========================="        
echo b.txt b.txt b.txt
${prog} ${tests}/b.txt ${tests}/b.txt ${tests}/b.txt
echo "=========================== test4 ==========================="        
echo ab.txt b.txt bc.txt
${prog} ${tests}/ab.txt ${tests}/b.txt ${tests}/bc.txt
echo "=========================== test5 ==========================="        
echo cs.txt bad.txt b.txt
${prog} ${tests}/cs.txt ${tests}/bad.txt ${tests}/b.txt
echo "=========================== test6 ==========================="        
echo empty.txt
${prog} ${tests}/empty.txt
echo "=========================== test7 ==========================="        
${prog} ${tests}/a.txt ${tests}/b.txt ${tests}/c.txt \
        ${tests}/d.txt ${tests}/e.txt ${tests}/f.txt \
        ${tests}/g.txt ${tests}/h.txt ${tests}/i.txt \
        ${tests}/j.txt 
echo "=========================== test8 ==========================="        
echo bad.txt k.txt
${prog} ${tests}/bad.txt ${tests}/k.txt