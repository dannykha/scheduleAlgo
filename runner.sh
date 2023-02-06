#!/bin/bash

make clean

rm results.txt

for sched in fcfs sjf priority rr priority_rr; do
  echo "-------------------------------------------" >> results.txt
  echo "Results for scheduler: $sched" >> results.txt
  echo "-------------------------------------------" >> results.txt
  make $sched && valgrind -s --leak-check=full ./$sched schedule.txt >> results.txt
  echo -e "\n" >> results.txt
done

make clean