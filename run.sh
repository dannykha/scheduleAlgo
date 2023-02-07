#!/bin/bash

make clean

for sched in fcfs priority_rr priority rr sjf; do
  make $sched && ./$sched schedule.txt > testResults/$sched.txt
done

make clean