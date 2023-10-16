/*
 * Copyright (C) 2015-2018,  Netronome Systems, Inc.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file          apps/wire/wire_main.c
 * @brief         Main program for ME cut-through wire app
 *
 * This application acts as a "wire" which by default will return packets
 * back out on the port they were received on.
 *
 *
 * It also doubles up as test code for the header extract
 * code. The packet headers are extracted and counts are maintained
 * for different types of packets.
 */

/* Flowenv */
#include <nfp.h>
#include <stdint.h>
#include<string.h>

#include <pkt/pkt.h>
#include <net/eth.h>

#include <net/ip.h>
#include <net/udp.h>

#include <nfp/mem_atomic.h>
#include <nfp/mem_bulk.h>

#include <nfp6000/nfp_nbi_tm.h>
#include <nfp/tmq.h>
#include <nfp/tm_config.h>



// #include <nfp/mac_time.h>
#include <nfp/me.h>

/*
 * Mapping between channel and TM queue
 */
#ifndef NBI
#define NBI 0
#endif

#ifndef PKT_NBI_OFFSET
#define PKT_NBI_OFFSET 128
#endif

/* DEBUG MACROS */

__volatile __export __emem uint32_t debug[8192*64];
__volatile __export __emem uint32_t debug_idx;
#define DEBUG(_a, _b, _c, _d) do { \
    __xrw uint32_t _idx_val = 4; \
    __xwrite uint32_t _dvals[4]; \
    mem_test_add(&_idx_val, \
            (__mem40 void *)&debug_idx, sizeof(_idx_val)); \
    _dvals[0] = _a; \
    _dvals[1] = _b; \
    _dvals[2] = _c; \
    _dvals[3] = _d; \
    mem_write_atomic(_dvals, (__mem40 void *)\
                    (debug + (_idx_val % (1024 * 64))), sizeof(_dvals)); \
    } while(0)


/* Counters */
struct counters {
    uint64_t no_vlan;
    uint64_t vlan_2;
    uint64_t vlan_3;
    uint64_t vlan_other;
    
};

struct statistics {
    uint64_t no_vlan;
    uint64_t vlan_2;
    uint64_t vlan_3;
    uint64_t vlan_other;
};

// Add metadata structure to be used for debugging with nfp-rtsym
struct CustomData {
    uint8_t custom1;
    uint8_t custom2;
    uint16_t custom3;
    uint32_t custom4;
    uint64_t custom5;
    uint64_t custom6;
    uint64_t custom7;
    uint64_t custom8[20];
    uint64_t custom9[20];
};

//declare a global variable to store moving average of queue as in when each packet comes
uint64_t movavg=0;

//declare a global variable to store time when the last packet came for each packet
uint32_t lasttime=0;

//declare array to store movavg values after every 100microseconds 
uint64_t avgqueuelevel[20];

//declare array to store current time after every 100milli seconds 
uint64_t  mytime[20];

// __declspec(shared ctm) is one copy shared by all threads in an ME, in CTM
// __declspec(shared export ctm) is one copy shared by all MEs in an island in CTM (CTM default scope for 'export' of island)
// __declspec(shared export imem) is one copy shared by all MEs on the chip in IMU (IMU default scope for 'export' of global)
__declspec(shared scope(island) export cls) struct statistics stats;
__declspec(shared scope(global) export imem) struct counters counters;

// Add one instance of the above defined metadata structure to imem memory
__declspec(shared scope(global) export imem) struct CustomData customData;


struct pkt_hdr {
    struct {
        uint32_t mac_timestamp;
        uint32_t mac_prepend;
    };
    struct eth_vlan_hdr pkt;
    struct vlan_hdr pkthdr_vlan;
};

struct pkt_rxed {
    struct nbi_meta_catamaran nbi_meta;
    struct pkt_hdr            pkt_hdr;
};
/*
 * STATS PACKET FUNCTION
 */

__intrinsic void
stats_packet( struct pkt_rxed *pkt_rxed,
              __mem40 struct pkt_hdr *pkt_hdr )
{
    __xwrite uint32_t bytes_to_add;
    SIGNAL   sig;
    int address;

    bytes_to_add = pkt_rxed->nbi_meta.pkt_info.len;

    if (pkt_rxed->pkt_hdr.pkt.tpid!=0x8100) {
        address = (uint32_t) &(stats.no_vlan);
    } else {
        if ((pkt_rxed->pkt_hdr.pkt.tci & 0xfff)==2) {
            address = (uint32_t) &(stats.vlan_2);
        } else if ((pkt_rxed->pkt_hdr.pkt.tci & 0xfff)==3) {
            address = (uint32_t) &(stats.vlan_3);
        } else {
            address = (uint32_t) &(stats.vlan_other);
        }
    }

    __asm {
        cls[statistic, bytes_to_add, address, 0, 1], ctx_swap[sig]
    }
}

/*
 * RECEIVE PACKET FUNCTION
 */

__mem40 struct pkt_hdr *
receive_packet( struct pkt_rxed *pkt_rxed,
                size_t size )
{
    __xread struct pkt_rxed pkt_rxed_in;
    int island, pnum;
    int pkt_off;
    __mem40 struct pkt_hdr *pkt_hdr;

    pkt_nbi_recv(&pkt_rxed_in, sizeof(pkt_rxed->nbi_meta));
    pkt_rxed->nbi_meta = pkt_rxed_in.nbi_meta;

    pkt_off  = PKT_NBI_OFFSET;
    island   = pkt_rxed->nbi_meta.pkt_info.isl;
    pnum     = pkt_rxed->nbi_meta.pkt_info.pnum;
    pkt_hdr  = pkt_ctm_ptr40(island, pnum, pkt_off);

    mem_read32(&(pkt_rxed_in.pkt_hdr), pkt_hdr, sizeof(pkt_rxed_in.pkt_hdr));
    pkt_rxed->pkt_hdr = pkt_rxed_in.pkt_hdr;

    return pkt_hdr;
}

/*
 * REWRITE PACKET FUNCTION
 */

void
rewrite_packet( struct pkt_rxed *pkt_rxed,
                __mem40 struct pkt_hdr *pkt_hdr )
{
    int vlan;

    vlan = 0;
    if (pkt_rxed->pkt_hdr.pkt.tpid==0x8100) {
        vlan = pkt_rxed->pkt_hdr.pkt.tci & 0xfff;

        // Assign Priority according to VLAN ID, in Netronome switch.
        if (vlan==2) {
            pkt_hdr->pkt.tci = pkt_hdr->pkt.tci & 0x1fff; // Priority 0
        } else if (vlan==3) {
            pkt_hdr->pkt.tci = (pkt_hdr->pkt.tci & 0x1fff) | (1 << 13); // Priority 1
        } else if (vlan==4) {
            pkt_hdr->pkt.tci = (pkt_hdr->pkt.tci & 0x1fff) | (1 << 14); // Priority 2
        }
    }
}

/*
 * COUNT PACKET FUNCTION
 */

void
count_packet( struct pkt_rxed *pkt_rxed,
              __mem40 struct pkt_hdr *pkt_hdr )
{
    if (pkt_rxed->pkt_hdr.pkt.tpid!=0x8100) {
        mem_incr64(&counters.no_vlan);
    } else {
        if ((pkt_rxed->pkt_hdr.pkt.tci & 0xfff)==2) {
            mem_incr64(&counters.vlan_2);
        } else if ((pkt_rxed->pkt_hdr.pkt.tci & 0xfff)==3) {
            mem_incr64(&counters.vlan_3);
        } else {
            mem_incr64(&counters.vlan_other);
        }
    }
}
/*
 * SEND PACKET FUNCTION
 */

void
send_packet( struct nbi_meta_catamaran *nbi_meta,
              __mem40 struct pkt_hdr *pkt_hdr )
{
    int island, pnum, plen;
    int pkt_off;
    __gpr struct pkt_ms_info msi;
    __mem40 char *pbuf;
    uint16_t q_dst = 0;
    uint8_t channel_dst = 0;

    uint32_t QueueNo0 = 0;
    uint32_t QueueNo1 = 1;
    __xread struct nfp_nbi_tm_queue_status tmq_statusE, tmq_statusE_1, tmq_statusE_2;
    static uint64_t Countss = 0;


    struct Timestamp_Store RegTimestamp1;

    uint32_t Count_for_5sec = 197812500;
    uint32_t Count_for_2sec = 79125000;
    uint32_t Count_for_1sec = 39562500;
    uint32_t Count_for_80microseconds = 3165;
    uint32_t Count_for_32microseconds = 1266;
    uint32_t Count_for_100milliseconds = 3956250;
    uint32_t Count_for_40milliseconds = 1582500;
    uint32_t Count_for_400milliseconds = 15825000;


    
    



    /* Write the MAC egress CMD and adjust offset and len accordingly */
    pkt_off = PKT_NBI_OFFSET + 2 * MAC_PREPEND_BYTES;
    island = nbi_meta->pkt_info.isl;
    pnum   = nbi_meta->pkt_info.pnum;
    pbuf   = pkt_ctm_ptr40(island, pnum, 0);
    plen   = nbi_meta->pkt_info.len - MAC_PREPEND_BYTES;

    channel_dst = nbi_meta->port;

    /** Select 1 of the below **/
    /* Set egress tm queue.
     * Set tm_que to mirror pkt to port on which in ingressed. */
//    q_dst = PORT_TO_CHANNEL(channel_dst);

    // Set egress queue to the other port
    // q_dst = PORT_TO_CHANNEL(channel_dst) ? 0 : 128;
    q_dst = 0;

    /** Select egress queue/channel to the other port such that ping and arp etc. can flow. **/
    /** Ping and arp use the 4th queue of the channel, we don't know the reason yet **/
//     if (channel_dst == 3 || channel_dst == 19) {
//         channel_dst = (channel_dst==19) ? 3 : 19;
//     } else {
//         channel_dst = (channel_dst==16) ? 0 : 16;
//     }



    //INCREMENT PACKET COUNT BY 1
    Countss = Countss + 1;


    pkt_mac_egress_cmd_write(pbuf, pkt_off, 1, 1); // Write data to make the packet MAC egress generate L3 and L4 checksums
    mem_add64_imm(avgqueuelevel, &customData.custom8[Countss-1]);
    mem_add64_imm(mytime, &customData.custom9[Countss-1]);

    msi = pkt_msd_write(pbuf, pkt_off - MAC_PREPEND_BYTES); // Write a packet modification script of NULL
    pkt_nbi_send(island,
                 pnum,
                 &msi,
                 plen,
                 NBI,
                 q_dst,
                 nbi_meta->seqr,
                 nbi_meta->seq,
                 PKT_CTM_SIZE_256);

    tmq_status_read(&tmq_statusE, 0, QueueNo0, 1);
    mem_add64_imm(tmq_statusE.queuelevel, &customData.custom5);
    tmq_status_read(&tmq_statusE_1, 0, QueueNo0, 1);
    mem_add64_imm(tmq_statusE_1.queuelevel, &customData.custom6);
    tmq_status_read(&tmq_statusE_2, 0, QueueNo1, 1);
    mem_add64_imm(tmq_statusE_2.queuelevel, &customData.custom7);

    //CODE TO CALCULATE THE AVERAGE QUEUE LEVEL
    uint64_t currtime; //variable declaration to store current system time
    
    //first find the new moving average because of new packet
    
     movavg = (movavg*(Countss-1)+tmq_statusE.queuelevel)/Countss;

    // read current time into a variable
    currtime = me_tsc_read();
    if (currtime - lasttime >= Count_for_100milliseconds)
    {
        avgqueuelevel[Countss-1]=movavg;//if condition holds good 'APPEND' the value of current movavg into an array defined earlier
        movavg=0;//reset movavg to  0
        mytime[Countss-1]=currtime //if condition holds good,'APPEND' the current time to an array >>for plotting purpose
        mem_add64_imm(avgqueuelevel, &customData.custom8[Countss-1]);
        mem_add64_imm(mytime, &customData.custom9[Countss-1]);
        lasttime=currtime;

    }

    
}

int
main(void)
{
    struct pkt_rxed pkt_rxed; /* The packet header received by the thread */
    __mem40 struct pkt_hdr *pkt_hdr;    /* The packet in the CTM */

    /*
     * Endless loop
     *
     * 1. Get a packet from the wire (NBI)
     * 2. Rewrite the packet ready for retransmission
     * 3. Count the packet as required
     * 4. Do statistics on the packet
     * 5. Send the packet back to the wire (NBI)
     */
    for (;;) {
        /* Receive a packet */

        pkt_hdr = receive_packet(&pkt_rxed, sizeof(pkt_rxed));

        /* Rewrite the packet */
        rewrite_packet(&pkt_rxed, pkt_hdr);

        /* Count the packet */
        count_packet(&pkt_rxed, pkt_hdr);

        /* Do stats on the packet */
        //stats_packet(&pkt_rxed, pkt_hdr);

        /* Send the packet */
        send_packet(&pkt_rxed.nbi_meta, pkt_hdr);

    }

    return 0;
}

/* -*-  Mode:C; c-basic-offset:4; tab-width:4 -*- */