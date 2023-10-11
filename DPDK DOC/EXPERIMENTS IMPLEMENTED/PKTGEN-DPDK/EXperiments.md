**Here is a list of all the things I've tried with PKTGEN:**


1)**Running a simple pktgen command line interface to send packets:**

The pktgen Builddir/app folder has a pktgen executable file which sends packets 
This is the command:

```bash
cd pktgen-dpdk
cd Builddir/app
./pktgen  -l 0-1 -n 3 -- -P -m "[1].0"
```
Here the -l stands  for core list, so we will send pkts from cores 0 or 1 , this can be chnaged according to your system.

the -n stands for the NUMA nodes to be used(memory related)

-P is for promiscuous mode: pktgen is sending and receiving packets only if this is enalbled / given in the EAL parameters.

-m stands for port list: This says that port 0 will be sending packets using core 1 . Make sure that the core number you are giving in the [], is 
included in the core list 'l'.

THis will open a CLI  like this :

<img src="Media/pktgencli.png" width="auto">  


Here is where you have to give the specifications for the packet such as:

set 0 count 500 : sets the number of packets to be 500(If you dont set a count , too many packets will be sent very quickly and the system will hang)
                  the 0 stands for the port number, since we're sending this on port 0 this should be 0 only.If the pktgen initial command 
                  something like this : -m "[1]:1", then t means that we are sending it on port 1 so ,  make sure to give set 1 count 500 in the CLI


set 0 rate 0.1: This command sets the rate at which packets are sent. 0.1 stands for the percentage of the link speed.Make sure to have a lower value 
                as rate like 0.1 or 0.01 so that you can see the packets flow slowly and dropping of packets can be avoided.


set 0 proto udp:You can select from a range of protoocls supported by pktgen . I have shhown for udp

start 0: to start sending packets
stop 0: to stop sending packets


Check out these links for more information about the EAL parameters
https://pktgen-dpdk.readthedocs.io/en/latest/usage_pktgen.html
https://pktgen-dpdk.readthedocs.io/en/latest/commands.html

2)**page stats and page latency:**

page stats: gives information about the  nummber of packets sent or received and dropped etc:

<img src="Media/pagestats.png" width="auto">  


page latency: this command displays the latencyand threshold and jitter related information on the RX system .So if you have transmitted packets and received 
them on another system, you can run this command on the RX system to find out the latency values

<img src="Media/pagelatency.png" width="auto">  


3)**Running 2 pktgen instances at once :**

Any 2 dpdk related programs or instances can not be run together without setting few paramerets.So for example if you try running two instances of pktgen(maybe on 2 
different ports of the same NIC,on 2 different terminals but the same system, then it will raise an error saying" already one instance of dpdk is running" like this:

<img src="Media/error.png" width="auto">  



So  2 instances of any dpdk program can be run by giving file prefixes to them to differentiate them:For example for dpdk:
On terminal 1 you give: ./pktgen  -l 0-1 -n 3 --file-prefix pg1 -- -P -m "[1].0"  
On terminal 2 you give: 

This will execute and you can carry out an experiment as follows:

<img src="Media/ROUNDTRIP.png" width="auto">  


As you can see in the picture , there is a packet generator(in this case oktgen) generates traffic from PORT 0 of one NIC at specified parameters and sends it through a otical fibre cable
to PORT 0 of the system under test(SUT).There is a forwarding program running at the SUT which does L2 forwarding of these packets to PORT 1 of the SUT
This forwarding program can be of two types:

```bash
A)DPDK based: Run the L2 forwarding example :
cd dpdk-23.03/build/examples
./dpdk-l2fwd -l 0-3 -n 4 -- -q 8 -p 3 --portmap="(0)(1)"

(or) Run the RX_TX callback application : 
cd dpdk-23.03/examples/rxtx_callbacks
make
sudo ./build/rxtx_callbacks -l1 -n4

(or) Run the Testpmd application: cd dpdk-23.03
./build/app/dpdk-testpmd  -n 4 -- -i --port-topology=chained
```

B)Kernel forwarding application: Set up a bridge to do the forwardinh using the script : ./multinic-as-bridge.sh (this script is in the Experiments IMplemented folder 
under DPDK doc)


Run two instances of pktgen on two terminals on the packet generator system:
One port which sends the packets and the other port that receives the packets :
On sending port(PORT 0) run this: ./pktgen  -l 0-1 -n 3 --file-prefix pg1 -- -P -m "[1].0"  
Set whatever parameters for the packet you want
I have set these: set 0 count 50000
                  set 0 rate 0.01
                  set 0 proto udp
                  page stats
ON receiving port(PORT1 ):
```bash
./pktgen  -l 0-1 -n 3 --file-prefix pg2 -- -P -m "[1].1"  
Set these parameters: enable 1 latency
                      page latency
```

Then put start 0 in the PORT 0 terminal

Then in PORT 1 terminal you can see the latency values getting filled up:


The observations made here are that: 
    1)When there's a kernel based forwarder in  SUT :the latency is :
    

    2)When theres a DPDK based forwarder in SUT: The latency is:




4)Sending pre saved pcap files through pktgen:
SAved pcap files can also be sent from pktgen if you dont want pktgen to generate the packets:
the command for that is:
```bash
cd pktgen-ddpk/Builddir/app
./pktgen  -l 0-1 -n 3 -- -P -m "[1].0" -f  -s 0:/home/zenlab/Documents/nimisha/my.pcap
```
where my.pcap is a file stored in the path specified. The 0 next to -s specifies the port from where the packets
are sent.




**5)Capturing received packets on pktgen into pcap files:**

To capture received packets on the RX port: Type these commands in the CLI:
```bash
enable 1 capture
*after all the packeta are captured*
disable 1  capture
```

This will save the captured packets in the form of pcap file in the Builddir/app folder



