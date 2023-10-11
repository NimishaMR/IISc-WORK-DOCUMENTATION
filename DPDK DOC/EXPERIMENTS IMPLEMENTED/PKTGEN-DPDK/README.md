**This folder has details regarding what pktgen-dpdk is and how it generates packets and how I have used it in my experiments:**

This is the link to the offcial documentation:
https://pktgen-dpdk.readthedocs.io/en/latest/


Pktgen, (Packet Gen-erator) is a software based traffic generator powered by the DPDK fast packet processing framework.

Some of the features of Pktgen are:

    1)It is capable of generating 10Gbit wire rate traffic with 64 byte frames.
    2)It can act as a transmitter or receiver at line rate.
    3)It has a runtime environment to configure, and start and stop traffic flows.
    4)It can display real time metrics for a number of ports.
    5)It can generate packets in sequence by iterating source or destination MAC, IP addresses or ports.
    6)It can handle packets with UDP, TCP, ARP, ICMP, GRE, MPLS and Queue-in-Queue.
    7)It can be controlled remotely over a TCP connection.
    8)It is configurable via Lua and can run command scripts to set up repeatable test cases.
    9)The software is fully available under a BSD licence.



It operates within the Data Plane Development Kit (DPDK) framework, designed for high-performance network packet processing. It's commonly used for testing and benchmarking network devices and applications. Here's an explanation of how Pktgen-DPDK generates packets:

1)Initialization and Configuration:
Pktgen-DPDK is typically run on a computer or server with DPDK-compatible network interface cards (NICs).
Users configure Pktgen-DPDK through a command-line interface or configuration files to define packet generation parameters, including traffic rate, packet size, destination IP addresses, and other network characteristics.

2)Load on Start (Lua Scripting):
Pktgen-DPDK often uses Lua scripting to specify test scenarios and traffic profiles.
Users can create Lua scripts to automate packet generation, allowing for complex traffic patterns and varying traffic loads.

3)Packet Template:
Users define a packet template or pattern that specifies the structure of each packet to be generated. This includes information such as source and destination MAC addresses, IP addresses, port numbers, and payload content.

4)Packet Generation:
Pktgen-DPDK generates packets based on the defined packet template and Lua scripts.
It uses DPDK's fast packet I/O capabilities to transmit packets at high speeds directly from the NICs, bypassing the operating system's kernel for maximum performance.

 5)Statistics and Monitoring:
Pktgen-DPDK provides real-time statistics on packet transmission, reception, and any packet loss.
Users can monitor various performance metrics, including throughput, latency, and packet loss, to assess the behavior of the network under test.


6) Analysis and Reporting:
After running tests, users can analyze the collected statistics and generate reports to evaluate the performance of network devices or applications.

**WHY DOES PKTGEN-DPDK NEED DPDK SUPPORT TO GENERATE PACKETS?**
Performance: DPDK is optimized for high-performance packet processing. It provides a user-space framework that allows direct access to network interfaces, bypassing the operating system's kernel. This results in significantly lower latency and higher packet processing rates compared to traditional kernel-based networking stacks. Pktgen relies on DPDK's performance enhancements to achieve high-speed packet generation and capture.

Kernel Bypass: Traditional networking stacks in operating systems introduce overhead, including context switches, memory copying, and interrupt handling. Pktgen's goal is to generate and analyze packets as efficiently as possible, which is best achieved by bypassing the kernel entirely. DPDK facilitates this kernel bypass by providing APIs for direct hardware access.

Access to NIC Features: DPDK provides access to advanced features and capabilities of network interface cards (NICs) that may not be accessible through the kernel. These features, such as hardware offloading and advanced packet filtering, are valuable for certain packet generation and network testing scenarios.

Control and Configuration: DPDK offers fine-grained control over NIC configuration parameters, enabling users to adjust settings such as packet transmission rates, packet sizes, and traffic patterns. This level of control is essential for precise network testing and benchmarking, which is a primary use case for Pktgen.

Consistency: DPDK ensures consistent and deterministic packet processing behavior, which is crucial for accurate and reproducible network tests. By working in user space and with DPDK, Pktgen can provide consistent behavior across different hardware and operating systems.OES PKTGEN DPDK NEED DPDK TO GENERATE PACKETS?**

**HOW DOES PKTGEN GENERATE PACKETS?**

<img src="Media/PKTGENARCH.jpg" width="auto">  

Does pktgen-dpdk code create sockets to generate packets?THe answer is no!
The Pktgen-DPDK code does not create sockets to generate packets. Pktgen-DPDK uses the Data Plane Development Kit (DPDK) to generate packets directly through memory-mapped access to network interface cards (NICs). It bypasses the traditional socket-based networking stack provided by the operating system.

Here's how Pktgen-DPDK generates packets without using sockets:

1)Memory-Mapped Access: Pktgen-DPDK leverages DPDK's memory-mapped access to NIC hardware. DPDK provides a framework for user-space applications to interact directly with NICs without going through the kernel's socket-based networking stack.

2)Packet Construction: Pktgen-DPDK constructs packets directly in memory by defining packet headers, payload, and other attributes as data structures. It doesn't use the socket's send() or recv() functions to transmit or receive packets.

3)Packet Transmission: Pktgen-DPDK transmits packets by copying them directly into the memory buffers associated with the NIC's transmit (TX) queues. This process doesn't involve creating sockets, establishing connections, or using socket-related system calls.

4)High Performance: By bypassing sockets and the kernel's networking stack, Pktgen-DPDK achieves higher performance and lower latency, making it suitable for high-speed packet generation and network testing.

In summary, Pktgen-DPDK operates at a lower level than socket-based communication. It interacts directly with NIC hardware using DPDK, allowing for efficient and high-performance packet generation without the overhead associated with socket creation and operation.

**How does pktgen=dpdk page latency calculate the latency and througput?**
Latency Calculation:

1)Round-Trip Latency: Pktgen-DPDK measures round-trip latency, which is the time it takes for a packet to travel from the source port to the destination port and back.

2)Timestamps: Pktgen-DPDK uses timestamps to record the departure time (when the packet is sent) and the arrival time (when the packet is received).

3)Latency Calculation: To calculate latency, Pktgen-DPDK subtracts the departure timestamp from the arrival timestamp for each packet. This calculation gives the time taken by the packet to complete the round trip.

4)Statistics: Pktgen-DPDK collects latency statistics, including minimum, maximum, and average latency. These statistics provide insights into the network's performance.

Throughput Calculation:

Packet Count and Size: Pktgen-DPDK tracks the number of packets sent and received during the test and measures the size of each packet.

Throughput Calculation: Throughput is calculated by dividing the total number of bits transmitted by the total time taken. It considers both the number of packets and their sizes.

Statistics: Pktgen-DPDK provides throughput statistics, such as packets per second (pps) and bits per second (bps), to quantify the network's data transfer rate.

**Does pktgen-dpdk add timestamps into generated packets payload?**

Pktgen-DPDK does not add timestamps into the packets' payload by default. Instead, it typically uses the following approach to measure packet latency

Timestamps on Packet Departure and Arrival:
Pktgen-DPDK uses timestamping functionality provided by DPDK (Data Plane Development Kit) to record the departure time (when a packet is sent) and the arrival time (when the packet is received) for each packet.

Latency Calculation:
To calculate packet latency, Pktgen-DPDK subtracts the departure timestamp from the arrival timestamp for each packet. This calculation provides the time taken by the packet to complete its round-trip journey from the source to the destination and back.

**Functions to generate timestamps**
In Pktgen-DPDK, the timestamps for packet departure (transmission) and arrival (reception) are typically recorded in the packet headers using the timestamp feature provided by DPDK. These timestamps are not stored in the packet payload but rather in the packet header itself. Specifically, DPDK provides two primary fields within the packet header to record these timestamps:

TX (Transmission) Timestamp:
 This timestamp records the time when a packet is transmitted or sent from the source. It is typically recorded in the tx_timestamp field of the packet   header.

RX (Reception) Timestamp:
  This timestamp records the time when a packet is received at the destination. It is typically recorded in the rx_timestamp field of 
  the packet header.

These timestamp fields are managed by DPDK and are not part of the standard Ethernet or IP packet headers. Pktgen-DPDK utilizes DPDK's timestamping capabilities to populate these fields for each packet as it is transmitted and received.

Keep in mind that the exact location and format of these timestamp fields within the packet header may vary depending on the specific version of DPDK and Pktgen-DPDK you are using. 

The specific byte offset for the RX (Reception) and TX (Transmission) timestamps in the UDP packets generated by Pktgen-DPDK depends on the DPDK version and the configuration used. However, in DPDK, these timestamps are typically stored in the Precision Time Protocol (PTP) or IEEE 1588 format in the Ethernet frame's Layer 2 header.


**Where are the timestamps finally located?**
Here are some general pointers on where to look for the timestamps:

IEEE 1588/PTP Timestamps: DPDK often uses the IEEE 1588 or PTP timestamp format, which is a 64-bit timestamp consisting of two 32-bit fields: seconds and nanoseconds. These fields are typically located in the Ethernet frame's Layer 2 header, specifically in the Precision Time Protocol (PTP) fields.

mbuf Structure: DPDK uses an mbuf (memory buffer) structure to manage packet data. The timestamps might be stored within the mbuf structure's metadata fields.

API Functions: DPDK provides API functions for timestamping, such as rte_eth_timesync_read_tx_timestamp and rte_eth_timesync_read_rx_timestamp. These functions are used to read timestamps from packets.

 these timestamps are maintained as part of the Precision Time Protocol (PTP) or IEEE 1588 timestamping functionality provided by DPDK.


Timestamps within DPDK: DPDK, the underlying framework for Pktgen-DPDK, provides APIs for timestamping packets directly at the hardware level. These timestamps are typically not stored in the UDP header but rather in the Ethernet frame's Layer 2 header.

IEEE 1588 Timestamp Format: DPDK often uses the IEEE 1588 or PTP timestamp format, which consists of two 32-bit fields: seconds and nanoseconds. These fields are part of the Precision Time Protocol (PTP) header within the Ethernet frame.

Timestamp API Functions: DPDK provides API functions for reading timestamps from packets, such as rte_eth_timesync_read_tx_timestamp for TX timestamps and rte_eth_timesync_read_rx_timestamp for RX timestamps. These functions access the timestamp fields within the Ethernet frame.

Accessing Timestamps: In Pktgen-DPDK or similar applications, you would typically use DPDK's timestamping APIs to access and record packet timestamps. The actual timestamps are not typically visible in the UDP header or payload, as they are maintained separately by DPDK.

The timestamps recorded by DPDK are precise hardware-level timestamps that capture the time when a packet is transmitted (TX timestamp) and received (RX timestamp). 


**useful links**

1)https://mails.dpdk.org/archives/users/2016-December/001263.html : Tells this:Note that the timestamp is kept in a register on the NIC. It stores only one TX timestamp at a time, irregardless of the number of queues etc.You have to read this register via queue:getTimestamp() before another packet can be timestamped.

2)https://doc.dpdk.org/guides/prog_guide/metrics_lib.html#timestamp-and-latency-calculation : scroll down to  Latency statistics library

3)


**
