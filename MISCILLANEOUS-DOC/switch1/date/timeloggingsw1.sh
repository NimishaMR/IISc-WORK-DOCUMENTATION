#!/bin/bash

# Date - 11 May 2023
# Author - Nimisha
# Description - system time logging of switch 2 for some time

systemtimetaken='time.csv'

echo ' ' > $systemtimetaken

# Timing Array of 1, 10, 30 and 60 seconds
TimingArray=(1 10 30 60 120 3600)

SECONDS=0

while true
do
if [ $SECONDS -le ${TimingArray[1]} ];then

date +"%T.%N">> time.csv
#date +"%s%n">> time.csv

else
echo "Elapsed Time (using \$SECONDS): $SECONDS seconds"
# killall -9 watch
sleep 1
break
fi
sleep 1
done
# reset

 sed -i '/^$/d' time.csv
 sed -i '/^user/d' time.csv
 sed -i '/^sys/d' time.csv
