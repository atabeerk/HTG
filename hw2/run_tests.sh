#!/bin/bash
#for ((i=3; i<=15; i=i+1))
#do
#      ./buildsa --prefix $i "example/ecoli.fa" "binaries/ecoli_pt$i"
#      filesize1=$(ls -lh "binaries/ecoli_pt$i.bin" | awk '{print  $5}')
#      filesize2=$(ls -lh "binaries/ecoli_pt$i.sdsl" | awk '{print  $5}')
#      echo "size: $filesize1+$filesize2"
#      echo "-----------"
#done

ALG="naive"

for ((i=3; i<=10; i=i+1))
do
      ./querysa --index binaries/ecoli_pt$i  --queries "example/query.fa" --query_mode $ALG --output "output/ecoli_pt${i}_${ALG}_output.txt"
      echo "$ALG $i"
      echo "-----------"
      echo "diff:"
      echo `diff "output/ecoli_pt${i}_${ALG}_output.txt" example/query_res.txt`
done

ALG="simple_accel"
for ((i=3; i<=10; i=i+1))
do
      ./querysa --index "binaries/ecoli_pt${i}"  --queries "example/query.fa" --query_mode $ALG --output "output/ecoli_pt${i}_${ALG}_output.txt"
      echo "$ALG $i"
      echo "-----------"
      echo "diff:"
      echo `diff "output/ecoli_pt${i}_${ALG}_output.txt" example/query_res.txt`
done
