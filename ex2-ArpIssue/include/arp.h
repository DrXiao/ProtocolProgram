#ifndef __ARP_H__
#define __ARP_H__

#include <pcap/pcap.h>
#include "common.h"

/*
 * ARP_PADDING : size of data part of myEthArp_t type
 *
 * ARP_ETH_TYPE :
 * ARP_OP_REQUEST :
 * ARP_OP_REPLY :
 * */
#define ARP_PADDING (46 - sizeof(myArp_t))

#define ARP_ETH_TYPE 0x0100
#define ARP_OP_REQUEST 0x0100
#define ARP_OP_REPLY 0x0200
/*
 *  Network layer
 *  Address Resolution Protocol - Packet Structure
 *
 *  0         8         16         24        31 (bit)
 *  |--------------------|--------------------|
 *  |  Ethernet Type     |  Protocol Type     |
 *  |--------------------|--------------------|
 *  |  ethLen |  ipLen   |  Operation Type    |
 *  |--------------------|--------------------|
 *  |  Source Ethernet Address (0~3 Byte)     |
 *  |--------------------|--------------------|
 *  |  srcEthAddr  (4~5) |  Src IP Addr (0~1) |
 *  |--------------------|--------------------|
 *  |  Src IP Addr (2~3) |  destEthAddr (0~1) |
 *  |--------------------|--------------------|
 *  |  Destination Ethernet Address  (2~5)    |
 *  |--------------------|--------------------|
 *  |  Destination IP Address (0~3 Byte)      |
 *  |--------------------|--------------------|
 *
 * Using a structure called myArp_t to represent
 * the above data.
 * */

typedef struct {
    uint16_t ethType;
    uint16_t ipType;
    uint8_t ethLen;
    uint8_t ipLen;
    uint16_t op;
    uint8_t srcEth[ETH_ADDR_LEN];
    uint8_t srcIP[IPV4_ADDR_LEN];
    uint8_t destEth[ETH_ADDR_LEN];
    uint8_t destIP[IPV4_ADDR_LEN];
} myArp_t;

/*
 * Link layer
 * The structure of Ethernet Frame
 *
 * 0         8         16         24        31 (bit)
 * |--------------------|--------------------|
 * |    Destination MAC Address (0~3 Byte)   |
 * |--------------------|--------------------|
 * |  DestMACAddr (4~5) |  SrcMACAddr (0~1)  |
 * |--------------------|--------------------|
 * |    Source MAC Address (2~5 Byte)        |
 * |--------------------|--------------------|
 * |    Ethernet Type   |   Payload          |
 * |--------------------|--------------------|
 * |                Payload ...              |
 * |--------------------|--------------------|
 *
 * myEthArp_t is a structure having the header of
 * ethernet frame and arp packet and it can be sent
 * to other place by setting the data by yourself.
 *
 * Because ethernet frame must have at least 60 bytes
 * and the size of structure of ethernet frame plus arp packet
 * is only 42 bytes, we need add extra data called padding 
 * into the myEthArp_t so that it can reach 60 bytes.
 * 
 * padding will be filled zeros.
 * */

typedef struct {
    uint8_t ethDest[ETH_ADDR_LEN];
    uint8_t ethSrc[ETH_ADDR_LEN];
    uint16_t ethType;

    myArp_t arp;

    uint8_t padding[ARP_PADDING];
} myEthArp_t;

/*
 * the declaration of the necessary functions.
 * */
extern void arpRequest(pcap_t *, uint8_t *);
extern void arpReply(pcap_t *, uint8_t *, uint8_t *);
extern void arpMainFunc(pcap_t *, uint8_t *, int);

#endif /* __ARP_H__*/