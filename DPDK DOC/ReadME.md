This is the documentation for the work pertaining to the Intel Data Plane Development Kit that I have worked on at Zenlab , IISc.

It conatins information about what DPDK is , it's advantages and drawbacks , it's architecturual composition and it's scope of application.

To set up DPDK on your Ubuntu machine , you need to keep in mind some dependensies and do some installations.All the modules to be installed are provided as 'shell' commands in the Installations and Dependencies File.

The different experiments that have been carried out using DPDK sample applications and it's relevent implementation steps have been provided as "Bash" commands to be executed on the terminal in the folder "Procedure for implementation".

While setting up and using DPDK and DPDK Based Traffic Generators like Pktgen and Moongen , I encountered many errors which have been documented with their respective corrections in the "Errors and Resolutions" folder.

**SO WHAT IS DPDK?**

Link to official newest release documentation: https://doc.dpdk.org/guides/linux_gsg/intro.html
DPDK is a set of libraries and drivers for fast packet processing.
It supports many processor architectures and both FreeBSD and Linux.

The DPDK uses the Open Source BSD-3-Clause license for the core libraries and drivers. The kernel components are GPL-2.0 licensed.
Data Plane Development Kit (DPDK) greatly boosts packet processing performance and throughput, allowing more time for data plane applications. DPDK can improve packet processing performance by up to ten times.As a result, telecom and network equipment manufacturers (TEMs and NEMs) can lower development costs, use fewer tools and support teams, and get to market faster.

The DPDK has five core components that are responsible for sending packets from point A to B in the framework:


1)**The EAL (Environment Abstraction Layer)** is responsible for gaining access to low-level resources such as hardware and memory space.The EAL conceals the kernel environment's specifics then creates a primary programming interface to the libraries and other parts of the operating system. This is important when providing a database as a service across a network linking to offsite data storage facilities.
   
2)**MBUF** is a specific data structure that carries network packets as messages.This happens in the background without the cloud database user’s knowledge. 
    The DPDK MEMPOOL is the library for creating allocated memory packets. They are of fixed size. The DPDK uses a MEMPOOL handler for storing free objects.
    
3)The **RING** library manages log-less queues/messages sent between threads, cores, or other parallel entities. In relation to the database as a service model, the RING library helps with data packet instructions.
    
4)The **TIMER** library is a DPDK service that offers asynchronistic callback functions. There is an interface to add, delete, or restart a timer.

**How does DPDK bypass the kernel?**
1)Instead of relying on a kernel network stack, kernel bypass solutions implement user space networking. It helps to avoid unnecessary memory copies (kernel space to user space) and avoids the scheduler delay when placing the receiver thread for execution. In kernel bypass, the receiver thread typically uses busy-waiting. Rather than waiting on a lock, it continuously checks the lock variable until it flags: “Go!” high throughput and low latency option for Ethernet networking is the Data Plane Development Kit (DPDK).

2)DPDK dedicates certain CPU Cores to be the packet receiver threads and since this particular core is restricteed only to DPDK, it uses a permanent polling mode in the driver to ensure the quickest possible response to arriving packets.

**What is EAL and PMD used in DPDK?**

1)Poll Mode Driver - Instead of the NIC raising an interrupt to the CPU when a frame is received, the CPU runs a poll
mode driver (PMD) to constantly poll the NIC for new packets. However, this does mean that a CPU core must be dedicated 
and assigned to running PMD.

2)The Environment Abstraction Layer (EAL) is responsible for gaining access to low-level resources such as hardware and memory space.
It provides a generic interface that hides the environment specifics from the applications and libraries. It is the responsibility of 
the initialization routine to decide how to allocate these resources (that is, memory space, PCI devices, timers, consoles, and so on).


<img src="Media/https:/0_E6pvV9ebVYVCCC2L.png.png" width="auto"> 


**How does DPDK help reduce latencies inthe netwroking stcack?**

1)For a typical application on Linux that sends a packet, when the application calls the send() syscall for example, the packet is 
copied from user-space memory into Kernel memory (into an skbuff). The skbuff is then copied into another section of memory by the 
Kernel that is accessible to the NIC. The Kernel signals to the NIC that there is a packet waiting to be sent and the NIC then copies 
the packet from this area of memory (via DMA transfer - Direct Memory Access) into it's hardware tx-ring buffer. This means that to 
get a packet from an application and into the NIC tx-ring buffer, the same packet is copied three times. DPDK allows for the NIC to 
DMA the packet directly from the application memory space (this works by DPDK disconnecting the NIC from the Kernel and mapping the
DMA memory space into the user-land memory space the application is using, hence "Kernel bypass").

2)DPDK does batch processing to avoid latencies.And it achieves fast packet processing and low latency with NVIDIA Poll Mode Driver (PMD) 
in DPDK. DPDK is a set of libraries and optimized network interface card (NIC) drivers for fast packet processing in a user space.
