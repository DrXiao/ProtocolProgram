#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pcap/pcap.h>

#ifndef PCAP_OPENFLAG_PROMISCUOUS
#define PCAP_OPENFLAG_PROMISCUOUS 1
#endif

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin)

int main(int argc, char **argv) {
    char errbuf[PCAP_ERRBUF_SIZE];
    int numberOfDevs = 0;
    pcap_if_t *allDevs;
    pcap_if_t *devPtr;

    /*
     * Variables
     *
     * selectDevNum :   let user select the interface by inputting a number.
     * 
     * countNumOfForLoop :  After user selects the interface, using a for loop
     *                      and devPtr pointer to reach the selected interface.
     *                      the variable is the counter for loop.
     * 
     * devAdapterHandler :  Every device has its network 'adapter', we can call
     *                      a function to obtain the network adapter of a certain
     *                      device. 
     *                      
     *                      Using a variable called devAdapterHandler with
     *                      pointer to pcap_t type to obtain the network adapter.
     * 
     * responseValue :  We will capture packets by another function called pcap_next_ex
     *                  For the function, it returns a value and responseValue catch the returned value.
     *                  represented whether capturin packets successfully or not.
     * 
     * localTime : Using it to record the local time of your machine.
     * 
     * timeStr :    Let the information of localTime be converted to a string and 
     *              be put into a buffer called timeStr
     * 
     * packetHeader & packetData :  Every packet has their header and data, using these two variables
     *                              to put the relative information respectively.
     * 
     *                              Noticing that types of two variables are different.
     * 
     * localUnixTimevalSec : Recording the Unix time.
     * 
     * */
    int selectDevNum;
    int countNumOfForLoop = 0;
    pcap_t *devAdapterHandler;
    int responseValue;
    struct tm localTime;
    char timeStr[16];
    struct pcap_pkthdr *packetHeader;
    const u_char *packetData;
    time_t localUnixTimevalSec;

    if (pcap_findalldevs(&allDevs, errbuf) == -1) {
        fprintf(stderr, "Couldn't find default device : %s\n", errbuf);
        exit(1);
    }

    for (devPtr = allDevs; devPtr; devPtr = devPtr->next) {
        printf("%2d. %-20s", ++numberOfDevs, devPtr->name);
        if (devPtr->description)
            printf(" : (%s)\n", devPtr->description);
        else
            printf(" : (No description available)\n");
    }

    if (!numberOfDevs) {
        printf("\nNo interface found! Make sure libpcap is installed.\n");
        return -1;
    }

    /*
     * Now, selecting a device and see the packets
     * received from the selected device.
     * */
    printf("Enter the interface number (1-%d) : ", numberOfDevs);
    scanf("%d", &selectDevNum);

    /*
     * If the number of interface that user selects is
     * out of range, freeing up the linked list and
     * ending the program.
     * */
    if (selectDevNum > numberOfDevs || selectDevNum < 1) {
        printf("\nInterface number out of range.\n");

        pcap_freealldevs(allDevs);
        return -1;
    }

    /*
     * By devPtr and countNumOfForLoop, going to the
     * selected interface.
     *
     * devPtr will point to the target interface.
     * */
    for (devPtr = allDevs, countNumOfForLoop = 0;
         countNumOfForLoop < selectDevNum - 1;
         devPtr = devPtr->next, countNumOfForLoop++)
        ;

    /*
     * By pcap_open_live function, it is called by passing following
     * paramaters.
     *
     * devPtr->name : The goaled device name.
     * 65536 : Maximum size of a packet -> packet will not be divided.
     * PCAP_OPENFLAG_PROMICUOUS : Using 'promiscuous' mode.
     * 1000 : Setting timeout time up.
     * errbuf : Providing a buffer to put error message when happening error.
     *
     * If it can open the device, the function will return the pointer to pcap_t
     * type, that is, It needs to use another pointer to catch the returned
     * pointer.
     *
     * Otherwise, the function will return NULL value.
     *
     * So, we use devAdapterHandler to get the returned pointer to pcap_t, and
     * we can capture the packets from this network device that we selected.
     *
     * ~~ An easy idea to understanding ~~
     * You can imagine that the function returns the network 'adapter' and we
     * use a handler to listen in on the selected device.
     * 
     * ~~ Detail of function name ~~
     * pcap_open_live : 'Open' a 'live' capture from the network.
     * */
    devAdapterHandler = pcap_open_live(devPtr->name, 65536,
                                       PCAP_OPENFLAG_PROMISCUOUS, 1000, errbuf);
    printf("%s\n", errbuf);
    // If we get a NULL pointer, ending the program.
    if (devAdapterHandler == NULL) {
        fprintf(
            stderr,
            "\nUnable to open the adapter. %s is not supported by libpcap\n",
            devPtr->name);

        pcap_freealldevs(allDevs);
        return 1;
    }

    printf("\nListening in on %s...\n", devPtr->name);

    /*
     * Because we have gotten the adapter of the selected device,
     * the linked list also has no any use and can be freed up.
     * */
    pcap_freealldevs(allDevs);

    /*
     * pcap_next_ex is a function that can capture a packet and put
     * the header part and data part into packetHeader and packetData,
     * respectively.
     *
     * If it works successfully, it returns 1.
     *
     * else if it comes across timeout situation, it returns 0.
     *
     * Otherwise, it returns a negative value when error happens
     *      All possible negative values are list as follows
     *          * -1 : Capturing packet unsuccessfully.
     *          * -2 : No any packet can be captured.
     *
     * */
    while ((responseValue = pcap_next_ex(devAdapterHandler, &packetHeader,
                                         &packetData) >= 0)) {
        /* 
         * If responseValue is zero, it comes across timeout and continues to
         * capture next packet
         * */
        if (responseValue == 0) continue;

        /*
         * Getting Unix time from header, converting Unix time 
         * to local time and showing the relative message about header.
         * 
         * locoltime_r :    Let the information of time_t type be 
         *                  converted to a representation of tm type.
         * strftime :   Converting the information of tm type to a string representataion
         *              by passing a buffer, buffer size, and a pointer to tm type.
         * 
         * */
        localUnixTimevalSec = packetHeader->ts.tv_sec;
        localtime_r(&localUnixTimevalSec, &localTime);
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &localTime);
        printf("%s, %.6ld len:%d\n", timeStr, packetHeader->ts.tv_usec,
               packetHeader->len);
    }

    pcap_close(devAdapterHandler);

    if (responseValue == -1) {
        printf("Error reading the packets: %s\n",
               pcap_geterr(devAdapterHandler));
        return -1;
    }

    PAUSE;
    return 0;
}