**Comparison of DPDK RXTX CALLBACKS example performance with kernel  L2 forwarding experiment:**


<img src="Media/ROUNDTRIP.png" width="auto"> 



1)BOth are using the same packet generator with same parameters:

Use pktgen as mentioned in the packetgen instruction file:
```bash
set the following parametrs:
set 0 count 50000
set 0 proto udp
set 0 rate 0.01
```

NOw the average cycles in DPDK Was 39 cycles:


<img src="Media/RXTXcallbackcycles.png" width="auto"> 

Hence the average forwarding latency for 50000 packets is:
**15.6 ns**

IN kernel:

If the bridging script is run in the system and wireshark captures are taken and compared each packet wise between the two ports of nic :
then if the epoch time's are subtracted :

<img src="Media/wireshark.png" width="auto"> 

The latencies progress as follows:

1)Latency 1- 136938 ns
2)Latency 2: 213806 ns
3)Latency 3: 493365951.4ns
