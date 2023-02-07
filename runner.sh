#!/bin/bash

make clean

rm results.txt

for sched in fcfs sjf priority rr priority_rr; do
  echo "-------------------------------------------" >> output.txt
  echo "Results for scheduler: $sched" >> output.txt
  echo "-------------------------------------------" >> output.txt
  make $sched && valgrind -s --leak-check=full ./$sched schedule.txt >> output.txt
  echo -e "\n" >> output.txt
done

make clean