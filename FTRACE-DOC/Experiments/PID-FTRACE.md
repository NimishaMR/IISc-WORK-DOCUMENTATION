**This file has the bash script that I've written which can automatically fetch the Process ID of any linux command on bash and give the frace output
pertaining to the execution of only that ccommand.

The function_graph that has been mentioned in the echo statement in this script can be replaced with other tracers as well.

```bash
#!/bin/bash     
DPATH="/sys/kernel/debug/tracing"
PID=$$
TEMP="/tmp/mytrace.txt"
## Quick basic checks
[ `id -u` -ne 0  ]  &&  { echo "needs to be root" ; exit 1; }  # check for root permissions
[ -z $1 ] && { echo "needs process name as argument" ; exit 1; } # check for args to this function
mount | grep -i debugfs &> /dev/null
[ $? -ne 0 ] && { echo "debugfs not mounted, mount it first"; exit 1; } #checks for debugfs mount

# flush existing trace data
echo nop > $DPATH/current_tracer

# set function tracer
echo function_graph > $DPATH/current_tracer

# enable the current tracer
#echo 1 > $DPATH/tracing_on

# write current process id to set_ftrace_pid file
echo $PID > $DPATH/set_ftrace_pid

# start the tracing
echo 1 > $DPATH/tracing_on
# execute the process
exec $* > /dev/null 2>&1 &
#echo "$*"

`cat $DPATH/trace > $TEMP`

echo 0 > $DPATH/tracing_on

echo nop > $DPATH/current_tracer

```


Run this script with a command for which you want to extract the PID

