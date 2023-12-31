**The set up is between 4s0 and 3s0 in the IISc Zenlab.You can use nay two systems of your choice.**

(3so is acting as the bridge and  4s0 acting as the network namespace which sends and receives packets)

<img src="Media/LINUXNAMESPACE.png" width=700>  

1)In the 4s0f0 system execute a script which creates network namespaces to assign IP addresses to each port of a single NIC.THis script can be found in the 
"multinic_namespace_cfg.sh" file under Experiments implememted folder.

2)run it as: sudo ./multinic_namespace_cfg.sh (should be run as root only- else it wont create the namespaces)

3)Make sure that the port numbers in this script is enp4s0f0 and enp4s0f1(or the ip adresses of the eth0 and eth1 in your NIC)

4)This will create the network namespaces and 

<img src="Media/multinicnamesapce.png" width=700>  

to verify that execute :
```bash
ip netns list
```

The output should be nsRX (id: 1)
                     nsTX (id: 0)

                     
5)Becauses network namespaces have been created in this system, if you now run ip a , the ports 4s0f0 and 4s0f1 wont show up

6)Even in wireshark the two ports wont be visible 

7)So if you want to still open wireshrak for those two ports then run wirehsark through terminal using:
```bash
  sudo ip netns exec nsTX wireshark 
  sudo ip netns exec nsRX wireshark
```

8)To undo/remove network namespaces that have been created run: the same script wiht a delete command like this:
```bash
sudo ./multinic_namespace_cfg.sh delete
```
Now if you run "ip a" the ports will reapppear


Next to set up the 3s0 system's two ports as the forwarding bridges, run this command in that system:
```bash
1)sudo ./ multinic-as-bridge.sh
```

The Bridge setting script titled "multinic-as-bridge.sh" can also be found in "experiments implemented folder"

2)IN this script ensure that the ports mentione are 3s0f0 and 3s0f1(or the ip adresses of the eth0 and eth1 in your NIC)

3)Do not create any network namespace in this system , you should just create a bridge here.

<img src="Media/multinibridge.png" width=600> 

Now to send UDP packets from 4s0 and 3s0 fowards it back to 4so and to receive this we need to run the following commands in 4so system
as iperf server and client programs

```bash
1)apt install iperf
2)in one tab first run this command to set up the server to listen:
sudo ip netns exec nsTX iperf -s -u -p 8000

```

3)Now that the server is listening, run this command in another tab to send packets:

```bash
sudo ip netns exec nsRX iperf -c 11.11.11.10 -u -t 15 -b 1k -p 8000
(This command sends UDP packets for 15 seconds at a bandwidth of 1K on port 8000)
```

Then you can run wireshark on 3s0 and choose Epoch time as the timestamp and the Identification field:

Compare the Timestamp values at the bridging NIC Or the TX/RX NIC ports to measure two types of latencies:

**RESULT**

**1)Forwarding latency(at 3s0 system wireshark files)=Average 17.123 Microseconds  (for 20 UDP packets sent for 15 seconds at bw of 1K)**

**2)Round trip latency(at 4s0 system wirehshark files)= Average 176 Microseconds  (for 20 UDP packets sent for 15 seconds at bw of 1K)**





-----------------------------------------------------------------------------------------------------------------------------------------
