#!/bin/bash

# Date - 11 May 2023
# Author - Nimisha
# Description - scrip to measure time it takes to measure system time

systemtimetaken='systemtime.csv'

echo ' ' > $systemtimetaken

# Timing Array of 1, 10, 30 and 60 seconds
TimingArray=(1 10 30 60 3600)

SECONDS=0

while true
do
if [ $SECONDS -le ${TimingArray[4]} ];then

    ./tester2.sh >> systemtime.csv

else
echo "Elapsed Time (using \$SECONDS): $SECONDS seconds"
# killall -9 watch
sleep 1
break
fi
sleep 1
done
# reset


 #sed -i '/^16/d' systemtime.csv


