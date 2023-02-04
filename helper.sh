make clean

make fcfs
make sjf
make rr
make priority
make priority_rr

echo -e "\nRunning FCFS" 
./fcfs book.txt
echo -e "\n"
./fcfs schedule.txt
echo -e "\n"

echo -e "Running SJF" 
./sjf book.txt
echo -e "\n"
./sjf schedule.txt
echo -e "\n"

echo -e "Running RR" 
./rr schedule.txt
echo -e "\n"

echo -e "Running PRI" 
./priority schedule.txt
echo -e "\n"

echo -e "Running RR-PRI" 
./priority_rr schedule.txt
echo -e "\n"