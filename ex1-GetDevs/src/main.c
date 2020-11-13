#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#ifndef PCAP_OPENFLAG_PROMISCUOUS
#define PCAP_OPENFLAG_PROMISCUOUS 1
#endif


#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin)

int main(int argc, char **argv) {

    // errbuf : Recording error mesage
    char errbuf[PCAP_ERRBUF_SIZE];

    /*
     * Finding all network interfaces on computer.
     *
     * libpcap uses pcap_findalldevs function to get all
     * interfaces by using linked list with pointer to
     * pcap_if_t type to record them.
     *
     * So, for the following variables,
     *
     * - allDevs - is the head of linked list recorded all interfaces.
     * - devPtr - is a pointer to traverse entire linked list.
     * - numberOfDevs - records the number of interfaces.
     *
     * */
    int numberOfDevs = 0;
    pcap_if_t *allDevs;
    pcap_if_t *devPtr;

    /*
     * By passing pointer to pointer to function, allDevs will
     * link to the first node of linked list.
     *
     * If there has any error while finding the interfaces,
     * function will put error message into errbuf and return
     * -1. So, if it happens, showing the error message and
     * terminating the program.
     *
     * */
    if (pcap_findalldevs(&allDevs, errbuf) == -1) {
        fprintf(stderr, "Couldn't find default device : %s\n", errbuf);
        exit(1);
    }

    /*
     * By devPtr, traversing entire linked list and printing
     * thier serial number, interface name and description
     * that if they have.
     * */

    for (devPtr = allDevs; devPtr; devPtr = devPtr->next) {
        printf("%2d. %-20s", ++numberOfDevs, devPtr->name);
        if (devPtr->description)
            printf(" : (%s)\n", devPtr->description);
        else
            printf(" : (No description available)\n");
    }

    /*
     * Ending the program by returning -1 if
     * there has no interface found.
     * */
    if (!numberOfDevs) {
        printf("\nNo interface found! Make sure libpcap is installed.\n");
        return -1;
    }

    PAUSE;

    /*
     * Before ending the program, freeing up the memory that
     * be allocated to record ther linked list.
     *
     * pcap_freealldevs will free up entire linked list.
     * */
    pcap_freealldevs(allDevs);
    return 0;
}