#include <stdio.h>
#include <string.h>
#include "common.h"
#include "arp.h"

// Broadcast ethernet address.
const uint8_t eth_Broadcast_Addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// Null ethernet address.
const uint8_t eth_Null_Addr[] = {0, 0, 0, 0, 0, 0};


/*
 * Sending an arp request to the targeted IP.
 * 
 * If ip is NULL, sending the request to the default gateway.
 *
 * */
void arpRequest(pcap_t *devAdapterHandler, uint8_t *ip) {
    myEthArp_t packet;

    // If no targeted IP address, setting ip to default gateway.
    if (ip == NULL) ip = defaultArpIP;
 
    /*
     * Filling the information of a packet.
     * 
     * Three components 
     * - Ethernet frame
     * - Arp packet
     * - Padding. (Extra data)
     * */

    // Ethernet frame.
    COPY_ETH_ADDR(packet.ethDest, eth_Broadcast_Addr);
    COPY_ETH_ADDR(packet.ethSrc, myEthAddr);
    packet.ethType = ETH_ARP;

    // Arp packet.
    packet.arp = (myArp_t){.ethType = ARP_ETH_TYPE,
                        .ipType = ETH_IP,
                        .ethLen = ETH_ADDR_LEN,
                        .ipLen = IPV4_ADDR_LEN,
                        .op = ARP_OP_REQUEST};
    COPY_ETH_ADDR(packet.arp.srcEth, myEthAddr);
    COPY_IPV4_ADDR(packet.arp.srcIP, myIPAddr);
    COPY_ETH_ADDR(packet.arp.destEth, eth_Null_Addr);
    COPY_IPV4_ADDR(packet.arp.destIP, ip);

    // Filling zeros.
    memset(packet.padding, 0, ARP_PADDING);

    // Sending the packet. If there has any error, printing the error message
    if (pcap_sendpacket(devAdapterHandler, (uint8_t *)&packet, sizeof(packet)) != 0) {
        fprintf(stderr, "\narpRequest Error sending: %s\n",
                pcap_geterr(devAdapterHandler));
    }
#if (DEBUG_ARP_REQUEST == 1)
    printf("arpRequest() to %s\n", ip_AddrStr(ip, NULL));
#endif /* DEBUG_ARP_REQUEST */
}

/*
 * Sending an arp reply to the destination with the assigned ethernet address
 * and IP address.
 * */
void arpReply(pcap_t *devAdapterHandler, uint8_t *destEth, uint8_t *destIP) {
    myEthArp_t packet;

    /*
     * Filling the information of a packet.
     * 
     * Three components 
     * - Ethernet frame
     * - Arp packet
     * - Padding. (Extra data)
     * */

    // Ethernet frame.
    COPY_ETH_ADDR(packet.ethDest, destEth);
    COPY_ETH_ADDR(packet.ethSrc, myEthAddr);
    packet.ethType = ETH_ARP;

    // Arp packet.
    packet.arp = (myArp_t){.ethType = ARP_ETH_TYPE,
                        .ipType = ETH_IP,
                        .ethLen = ETH_ADDR_LEN,
                        .ipLen = IPV4_ADDR_LEN,
                        .op = ARP_OP_REPLY};
    COPY_ETH_ADDR(packet.arp.srcEth, myEthAddr);
    COPY_IPV4_ADDR(packet.arp.srcIP, myIPAddr);
    COPY_ETH_ADDR(packet.arp.destEth, destEth);
    COPY_IPV4_ADDR(packet.arp.destIP, destIP);

    // Filling zeros.
    memset(packet.padding, 0, ARP_PADDING);

    // Sending the packet. If there has any error, printing the error message
    if (pcap_sendpacket(devAdapterHandler, (uint8_t *)&packet, sizeof(packet)) != 0) {
        fprintf(stderr, "\narpReply Error sending: %s\n", pcap_geterr(devAdapterHandler));
    }
#if (DEBUG_ARP_REPLY == 1)
    printf("arpReply() tp %s\n", ip_AddrStr(destIP, NULL));
#endif /* DEBUG_ARP_REPLY */
}


/*
 * The function handling the arp packets.
 * 
 * Printing the information of an arp packet and sending 
 * an arp reply if it receives an arp request.
 * 
 * */
void arpMainFunc(pcap_t *devAdapterHandler, uint8_t *packet, int len) {
    myEthArp_t *arp;
    char srcEth[BUFLEN_ETH], srcIP[BUFLEN_IP];
    char destEth[BUFLEN_ETH], destIP[BUFLEN_IP];

    // Viewing byte data as the packet with ethernet frame and arp packet by pointer.
    arp = (myEthArp_t *)packet;

#if (DEBUG_ARP == 1)

    // Printing the information of the packet.
    printf("ARP Eth = %04x/%d, IP = %04x/%d, Op = %04x\n"
           "\tFrom %s (%s)\n"
           "\tTo   %s (%s)\n",
           (int)arp->arp.ethType, (int)arp->arp.ethLen, (int)arp->arp.ipType,
           (int)arp->arp.ipLen, (int)arp->arp.op,
           eth_MacAddr(arp->arp.srcEth, srcEth),
           ip_AddrStr(arp->arp.srcIP, srcIP),
           eth_MacAddr(arp->arp.destEth, destEth),
           ip_AddrStr(arp->arp.destIP, destIP));

#endif /* DEBUG_ARP */

    // Checking the opcode of the arp packet.
    switch (arp->arp.op) {
    case ARP_OP_REQUEST:
        if (memcmp(arp->arp.destIP, myIPAddr, IPV4_ADDR_LEN) == 0)
            arpReply(devAdapterHandler, arp->arp.srcEth, arp->arp.srcIP);
        break;
    case ARP_OP_REPLY: break;
#if (DEBUG_ARP == 1)
    default: printf("unknown ARP opcode\n");
#endif /* DEBUG_ARP */
    }
}