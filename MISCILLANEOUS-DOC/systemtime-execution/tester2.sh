#!/bin/bash
# This should be at the top of the script to get the start time of the script
start=$(date +%s.%N)


date +%T.%N

duration=$(echo "$(date +%s.%N) - $start" | bc)
execution_time=`printf "%.9f seconds" $duration`

echo "Script Execution Time: $execution_time"