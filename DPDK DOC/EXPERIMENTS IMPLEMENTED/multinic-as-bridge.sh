#!/bin/bash

# Description: Configures Netronome as a bridge/switch
# using Linux 'ip link' utility. Run this on system hosting Netronome card.

echo "Run as sudo"

echo "To remove configuration, add argument 'delete'"

echo "Current Time = "`date`
echo "HOSTNAME = "`hostname`
echo "USER ID = "`whoami`
echo "IP ADDRESS = "`ip a s enp6s0 | grep "inet " | cut -f6 -d " "`

echo "Setting up 2-port NIC as a switch"
ip link add Switch1 type bridge
ip link set enp3s0f0 master Switch1
ip link set enp3s0f1 master Switch1
ip link set Switch1 up


sleep 2
echo "Verify"
ip a

if [ "$1" = delete ]; then
	echo "Removing bridge"
	ip link del Switch1
	sleep 1
	ip a
fi
