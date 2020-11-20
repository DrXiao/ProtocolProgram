#ifndef __COMMON_H__
#define __COMMON_H__

#include <pcap/pcap.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#ifndef PCAP_OPENFLAG_PROMISCUOUS
#define PCAP_OPENFLAG_PROMISCUOUS 1
#endif

#define FG_NATIVE_CYGWIN 1

// Sending an arp request at beginning if this flag is 1.
#define FG_ARP_SEND_REQUEST 1

/*
 * Flags
 *
 * If a flag produced by macro is 1, it will
 * print certain message when executing the program.
 * */

#define DEBUG_PACKET 0
#define DEBUG_PACKET_DUMP 0
#define DEBUG_ARP 1
#define DEBUG_ARP_REQUEST 1
#define DEBUG_ARP_REPLY 0
#define DEBUG_ARP_DUMP 0

/*
 * Constants
 *
 * MAX_CAP_LEN : Maximum size of a captured packet.
 * MAX_DUMP_PKT : Unknown??
 * */
#define MAX_CAP_LEN 1514
#define MAX_DUMP_PKT 5

/*
 * BUFLEN_ETH : Converting Mac address with 6 bytes to a string
 *              -> "XX:XX:XX:XX:XX:XX"
 *          It needs    6    *    2       +         5       +     1
 *                 (Bytes)  (Character)   (':' character)   ('\0' character)
 *          = 18 characters. -> the size of string.
 *
 * BUFLEN_IP :  Let IP address be a string. ("A.B.C.D")
 *              For A, B, C and D, each of them may be at most 3 digits.
 *
 *              So the maximal size of string is 4 * 3 + 3 + 1 = 16.
 *
 * MAX_DUMP_LEN : Unknown??
 *
 * MAX_LINE_LEN :   If we print the byte data of a packet, it print at most
 *                  '16' bytes at a line.
 *
 * MAX_LINEBUF :    If it must to receive the input from user, it only gets
 *                  at most '256' characters from input.
 * */
#define BUFLEN_ETH 18
#define BUFLEN_IP 16
#define MAX_DUMP_LEN 80
#define MAX_LINE_LEN 16
#define MAX_LINEBUF 256

/*
 * Assigned Numbers and Parameters
 *
 * Because the domain of network uses 'big-endian' usually
 * and C language is 'little-endian', In order to cooperate
 * with network, we need to use 'big-endian' when writing the
 * parts related to network.
 *
 * For example, ethernet type :
 *      1. 0x08 00 : represents IPv4.
 *      2. 0x08 06 : represents ARP.
 *
 * We have to use reverse order to record them.
 * so,
 *      0x08 00 -> 0x00 08
 *      0x08 06 -> 0x06 08
 * */

#define ETH_IP 0x0008
#define ETH_ARP 0x0608

/*
 * If using an array of unsigned integer type to store the
 * data of ethernet address and IP address,
 *
 * For arrays,
 *  ethernet address needs '6' elements. (ETH_ADDR_LEN)
 *
 *  IP address needs '4' elements. (IPV4_ADDR_LEN)
 *
 * */
#define ETH_ADDR_LEN 6
#define IPV4_ADDR_LEN 4

typedef struct {
    uint8_t ethDest[ETH_ADDR_LEN];
    uint8_t ethSrc[ETH_ADDR_LEN];
    uint16_t ethType;
    uint8_t data[MAX_CAP_LEN];
} myEth_t;

/*
 * Using macro to reduce codes and doing some operations.
 *
 * For the below macro, they means that copying ethernet address or
 * IP address from source buffer to destination buffer.
 * */
#define COPY_ETH_ADDR(dest, src) (memcpy((dest), (src), ETH_ADDR_LEN))
#define COPY_IPV4_ADDR(dest, src) (memcpy((dest), (src), IPV4_ADDR_LEN))

typedef uint32_t ipAddr_t;

/*
 * From config.c .
 * Setting basic information about your
 *  1. ethernet address
 *  2. IP address
 *  3. Default gateway.
 * */

extern uint8_t myEthAddr[ETH_ADDR_LEN];
extern uint8_t myIPAddr[IPV4_ADDR_LEN];
extern uint8_t defaultArpIP[IPV4_ADDR_LEN];

#define getIP(ipAddr) (*((ipAddr_t *)(ipAddr)))
#define isMyIP(ipAddr) ((getIP(ipAddr) == getIP(myIPAddr)))

/*
 * utilities.
 *
 * See util.c .
 * */

extern int readReady();
extern char *time2DecStr(time_t);
extern ipAddr_t my_inet_Addr(char *);
extern char *ip_AddrStr(unsigned char *, char *);
extern char *eth_MacAddr(const unsigned char *, char *);

extern void print_IP(unsigned char *, char *);
extern void print_Data(const unsigned char *, int len);

/*
 * constants
 *
 * Ethernet Broadcast Address   - FF:FF:FF:FF:FF:FF
 * Ethernet NULL Address        - 00:00:00:00:00:00
 *
 * Their definition is in arp.c .
 * */

extern const uint8_t eth_Broadcast_Addr[ETH_ADDR_LEN];
extern const uint8_t eth_Null_Addr[ETH_ADDR_LEN];

#endif /* __COMMON_H__ */