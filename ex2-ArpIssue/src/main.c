#include <stdio.h>
#include <stdlib.h>

#include <pcap.h>

#include "common.h"
#include "arp.h"

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin)

/*
 * mainProc()
 *
 * Sending an arp request and capturing packets continuously.
 *
 * If the captured packet is an arp packet, calling arpMainFunc to
 * send an arp 'reply.'
 *
 *
 * devAdapterHandler :  Every device has its network 'adapter', we can call
 *                      a function to obtain the network adapter of a certain
 *                      device.
 *
 *                      Using a variable called devAdapterHandler with
 *                      pointer to pcap_t type to obtain the network adapter.
 *
 * packetHeader & packetData :  Every packet has their header and data, using
 *                              these two variables to put the relative
 *                              information respectively.
 *                              Noticing that types of two variables are
 *                              different.
 * 
 * responseValue :  We will capture packets by another function called
 *                  pcap_next_ex For the function, it returns a value and 
 *                  responseValue catch the returned value. 
 *                  represented whether capturing packets successfully or not.
 * 
 * packetLen :      The size of entire packet.
 * 
 * key : If user input some characters, it will catch first character.
 *       If it is a newline character ('\n'), ending capturing packets.
 *       else, putting the character into stdin again.
 * 
 * buf : the buffer for user's input.
 * 
 * packet : It is a pointer to a memory address stored ethernet frame + arp packet.
 * 
 * ip : Storing the IP address.
 * 
 * */

int mainProc(pcap_t *devAdapterHandler) {
    struct pcap_pkthdr *packetHeader;
    const u_char *packetData;
    int responseValue, key, packetLen;
    char buf[MAX_LINEBUF];
    myEth_t *packet;
    ipAddr_t ip;
#if (DEBUG_PACKET == 1)
    char addrSrc[BUFLEN_ETH], addrDest[BUFLEN_ETH];
#endif /* DEBUG_PACKET */

#if (FG_ARP_SEND_REQUEST == 1)
    arpRequest(devAdapterHandler, NULL);
#endif /* FG_ARP_SEND_REQUEST */

    while ((responseValue = pcap_next_ex(devAdapterHandler, &packetHeader,
                                         &packetData)) >= 0) {

        if (responseValue > 0) {

#if (DEBUG_PACKET == 1)
            printf("*%s %s => %s (Type = %.2x%.2x/Len = %d)\n",
                   time2DecStr(packetHeader->ts.tv_sec),
                   eth_MacAddr(packetData + 6, addrSrc),
                   eth_MacAddr(packetData, addrDest), packetData[12],
                   packetData[13], packetHeader->len);
#endif /* DEBUG_PACKET */
            packet = (myEth_t *)packetData;
            packetLen = packetHeader->caplen;

#if (DEBUG_PACKET_DUMP == 1)
            print_Data(packetData, packetLen);
#endif

            /*
             * Verifying whether the ethType is arp and 
             * sending an arp reply by arpMainFunc.
             * */
            switch (packet->ethType) {
            case ETH_ARP:
#if (DEBUG_PACKET_DUMP == 0 && DEBUG_ARP_DUMP == 1)
                print_Data(packetData, packetLen);
#endif /* DEBUG_ARP_DUMP */
                arpMainFunc(devAdapterHandler, (uint8_t *)packetData,
                            packetLen);
                break;
            }
            /*
             * 
             * Here can receive user's input.
             * 
             * Scanning the input buffer
             * 
             * If getting a newline character, ending the loop.
             * 
             * else, putting the character to input buffer and get a line
             * of user's input. Parsing the user's input as an IP address then
             * sending an arp request to the IP address the user inputs.
             * 
             * */
            if (!readReady()) continue;
            if ((key = fgetc(stdin)) == '\n') break;
            ungetc(key, stdin);

            if (fgets(buf, MAX_LINEBUF, stdin) == 0)
                printf("Invalid IP (Enter to exit)\n");
            else
                arpRequest(devAdapterHandler, (unsigned char *)&ip);
        }
    }

    if (responseValue == -1) {
        fprintf(stderr, "Error reading the packets: %s\n",
                pcap_geterr(devAdapterHandler));
        return -1;
    }

    return 0;
}

/*
 * MAIN ENTRY
 *
 * Here, you can refer ex1-PacketGet.
 *
 * myPcap_GetDevice is a function to find the name of targeted
 * device that you want.
 * */

char *myPcap_GetDevice(int defn, pcap_if_t **allDevs) {
    pcap_if_t *devPtr;

    int selectDevNum, numbersOfDevs = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    char buf[MAX_LINEBUF];

    if (pcap_findalldevs(allDevs, errbuf) == -1) {
        fprintf(stderr, "Errpr in pcap_findalldevvs: %s\n", errbuf);
        return NULL;
    }

    if (defn > 0) {
        selectDevNum = defn;
        for (devPtr = *allDevs; devPtr; devPtr = devPtr->next, ++numbersOfDevs)
            ;
    }
    else {
        printf("Device list : \n");

        for (devPtr = *allDevs; devPtr; devPtr = devPtr->next) {
            printf("%2d. %-20s\n    ", ++numbersOfDevs, devPtr->name);
            if (devPtr->description)
                printf(" (%s)\n", devPtr->description);
            else
                printf(" (No description available)\n");
        }

        if (numbersOfDevs == 0) {
            fprintf(stderr, "No interfaces found!");
            return NULL;
        }

        printf("Enter the interface number (1-%d) : \n", numbersOfDevs);
        fgets(buf, MAX_LINEBUF, stdin);
        sscanf(buf, "%d", &selectDevNum);
    }

    if (selectDevNum > numbersOfDevs || selectDevNum < 1) {
        printf("\nInterface number out of range.\n");

        pcap_freealldevs(*allDevs);
        *allDevs = NULL;
        return NULL;
    }

    for (devPtr = *allDevs, numbersOfDevs = 0; numbersOfDevs < selectDevNum - 1;
         devPtr = devPtr->next, numbersOfDevs++)
        ;

    printf("device : %s\n", devPtr->name);
    return devPtr->name;
}

int main(int argc, char **argv) {

    pcap_if_t *allDevs;

    char *devName, errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *devAdapterHandler;

    /*
     * Finding the name of the targeted device.
     *
     * then getting the file descriptor by pcap_open_live.
     * */
    if (argc == 2)
        devName = argv[1];
    else if ((devName = myPcap_GetDevice(0, &allDevs)) == NULL)
        return -1;

    if ((devAdapterHandler =
             pcap_open_live(devName, MAX_CAP_LEN, PCAP_OPENFLAG_PROMISCUOUS,
                            100, errbuf)) == NULL) {
        fprintf(stderr, "\nError opening source: %s\n", errbuf);
        return -1;
    }

    /*
     * Starting arp request and capturing packets.
     * */

    mainProc(devAdapterHandler);

    pcap_close(devAdapterHandler);

    if(allDevs) {
        printf("Cleaning the list with devices!!\n");
        pcap_freealldevs(allDevs);
    }

    PAUSE;
    return 0;
}