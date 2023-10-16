Link followed for creating cpuset(core isolation):

Scroll down to usage examples and syntax:

https://docs.kernel.org/admin-guide/cgroup-v1/cpusets.html#:~:text=Cpusets%20provide%20a%20Linux%20kernel,memory%20(mbind%2C%20set_mempolicy).



1)Navigate to OtherLocations>>Computer>>sys/fs/cgroup/cpuset folder in the Switch 2 system
I've created my_cpuset in the sys/fs/cgroup/cpuset folder which will have the files in which cpu isolation has been done to run the bash script 'timeloggingsw2.sh'(located in Documents/nimisha_test)

2)added cpus 0 in cpuset.cpus(so the only allocated cpu is '0')

3)tried to add mems 0-7 as given in the link but it gave an error as follows: write error: Invalid argument

Reason for the error is found in :
https://man7.org/linux/man-pages/man7/cpuset.7.html

Scroll down to errors,
The error was identified as "Attempted to write(2) a list to a cpuset.mems file that included a node that held no memory"

So to check which all numa memory nodes are present in our system(switch2) execute:

a)apt install numacatl
b)numactl --hardware

only memory node 0 was available 
4)so changed this command :/bin/echo 0-7 > cpuset.mems
to this command::/bin/echo 0> cpuset.mems


5)to attach shell to cpuset:
$$ is the process ID of the current shell instance

6)By default the cpu_Exclusive and mem_exclusive are set to 0

This has been changed to 1 using the commands:
/bin/echo 1 > cpuset.cpu_exclusive
/bin/echo 1 > cpuset.mem_exclusive

by seeting the flags to 1 you ensure that the memory node and cpu list thats allocated to your process will be exclusively for only your process

7)Next task is to find out the process id of your task 
so the command to find that is:
run the process then do 'echo $!'
then attach the pid to task by :
/bin/echo PID > tasks
The idea of this is to tell the computer to allocate only the cpu lsited in cpuset.cpus to the process's pid 

But the problem here is the bash script we want to run for example 'timelogger.sh' has a newley generated pid everytime the script is ran after killing it

Hence a fixed pid can not be defined for a bash script for its entire lifetime.So a fixed pid can no be added to the task file.

So:The process(for example:  ./timelogger.sh) should be run and its pid must be noted,
As long as the process is not killed or the terminal is not closed,however many times you run the process its pid will be same
Only if you kill it or open a new terminal and run it again,the pid will change


8)So the terminal wasnt closed and the pid was noted and added to the tasks file in my_cpuset using the command /bin/echo PID > tasks
here PID was 10307

9)According to the documentation,core isolation must have been achieved if these steps are completed

To verify this I executed the following command which shows that the Cpu '0' has been assigned to the pid 10307

command: ps -o pid,psr,comm -p 10307

result:  PID    PSR COMMAND
         10307   0   bash

The above result indicates that the pid 10307 has been assigned the cpu core 0-core isolation done


10)the new csv values are obtained which indicate sw2's system time for a duration of 1 hour after core isolation has been done
and the same has been plotted to see if it differs from the csv plot of sw2 when core isolation was not done

11)the plots can be found on the system(SWITCH 1) in the folder:Documents/Microcprograms/nimisha_tests/polts with core isolation folder











