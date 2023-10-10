#!/bin/bash

# Author: Joydeep Pal
# Date: Nov 2022
# Date Modified: Apr 2023
# Description: Configures MultiNIC as isolated network namespaces with
# access to different physical ports. This helps when Tx and Rx ports
# are on the same PC and iperf/ping commands have to be used.
# Otherwise, these commands don't send packets out to the wire
# because kernel routes it internally.

# Configure network namespaces using script, if both tx and rx ports are on same PC'
echo "Run as sudo"

echo "NOTE: To delete created namespaces, run above command with argument 'delete'"

PORT_0=enp3s0f0
PORT_1=enp3s0f1
IPTX=11.11.11.10
IPRX=11.11.11.11
NW_Namespace_0=nsTX
NW_Namespace_1=nsRX
MAC_ADDRESS_0=00:1b:21:c2:54:40
MAC_ADDRESS_1=00:1b:21:c2:54:42

echo ''
echo "Show all network devices/interfaces"
ip a 

# Creates network namespaces 
# (acts like computer nodes, with deactivated loopback devices)
ip netns add $NW_Namespace_0
ip netns add $NW_Namespace_1
ip netns

# Assign the two Ethernet ports to respective network namespaces
ip link set $PORT_0 netns $NW_Namespace_0
ip link set $PORT_1 netns $NW_Namespace_1

# Assign IP addressess to the interafces 
# to make them functional and activate them
ip -n $NW_Namespace_0 addr add $IPTX/24 dev $PORT_0
ip -n $NW_Namespace_1 addr add $IPRX/24 dev $PORT_1
ip -n $NW_Namespace_0 link set $PORT_0 up
ip -n $NW_Namespace_1 link set $PORT_1 up

sleep 1

echo "Root namespace ports"
ip a
echo ''
echo "Namespace $NW_Namespace_0 ports"
ip -n $NW_Namespace_0 a
echo ''
echo "Namespace $NW_Namespace_1 ports"
ip -n $NW_Namespace_1 a

<< comment
# If namespaces are used, you can open wireshark captures using below commands
sudo ip netns exec nsTX tshark -i enp1s0f0 -a duration:2 -w ../TXv1.pcap &
sudo ip netns exec nsRX tshark -i enp1s0f1 -a duration:2 -w ../RXv1.pcap
comment

if [ "$1" = delete ]; then
	echo "Removing user namespaces"
	ip netns delete $NW_Namespace_0
	ip netns delete $NW_Namespace_1
	sleep 1
	ip a
fi
