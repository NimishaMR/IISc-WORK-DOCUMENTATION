Hwlatdetector is used to detect large system latencies induced by the behavior of certain underlying hardware or firmware, independent of Linux itself.

The ftrace hardware latency detector works by hogging all of the cpus 

polling the CPU Time Stamp Counter for some period, 

then looking for gaps in the TSC data. 

Any gap indicates a time when the polling was interrupted and since the machine is stopped and interrupts turned off the only thing that could do that would be an SMI.


syntax:
```bash
hwlatdetect [ --duration=<time> ] [--threshold=<usecs> ] [--window=<time interval> ] [--width=<time interval> ] [--hardlimit=<microsecond value> ] [--report=<path> ] [--debug ] [--quiet ] [--watch]
```




The Time Stamp Counter (TSC) counts the number of CPU cycles since its reset.(or the number of clock signals arriving on the CLK pin of the processor since rest)


System management interrupts are high priority unmaskable hardware interrupts which cause the CPU to immediately suspend all other activities, including the operating system, and go into a special execution mode called system management mode (SMM).

SMI can make it impossible for the CPU to handle a critical system interrupt before its deadline

there is very little documentation containing details about which SMIs happen in which situations, so it is hard to identify the specific SMI that is causing the problem. Secondly, even if the problematic SMI can be identified, there is also very little documentation about which firmware options can be used to disable a particular SMI

disabling all SMIs to prevent the system from ever going into SMM is not a recommended solution as it can result in a melted processor. 


 Latencies caused by the kernel are much more common than latencies caused by firmware or hardware, so the possibility of a kernel related latency should be eliminated before any further investigation. On the other hand, if a latency is indeed SMI related then knowing the function calls that are made around the SMI can help determine why it is triggered. 



1)keep cpu busy/non idle
2)First, the latencies in the trace must be identified. Because the trace contains every function call made by the kernel, the timestamps for consecutive tracepoints should only be a few microseconds apart. In theory, any gap between function calls that is longer than a few microseconds could potentially indicate a problem, but it is generally best to start by finding and analyzing the longest latencies in the trace. 
3)https://wiki.linuxfoundation.org/realtime/documentation/howto/debugging/smi-latency/start

