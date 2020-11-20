#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"

/*
 * Utilities
 * */

#if (FG_NATIVE_CYGWIN == 1)
#include <sys/select.h>

/*
 * readReady()
 *
 * Unknown ??
 *
 * */

int readReady() {
    fd_set map;
    int fileDescriptor = 0;
    int ret;
    struct timeval _zeroTimeval = {0, 0};

    do {
        FD_ZERO(&map);
        FD_SET(fileDescriptor, &map);
        ret = select(fileDescriptor + 1, &map, NULL, NULL, &_zeroTimeval);
        if (ret >= 0) return ret;
    } while (errno == EINTR);

    return ret;
}

#else

int readReady() {
    extern int _kbhit();
    return _kbhit();
}

#endif /* FG_NATIVE_CYGWIN */

/*
 * time2DecStr()
 *
 * Let the current time be converted to a formatted
 * string and be returned.
 * */

char *time2DecStr(time_t t) {
    static char buf[20 + 1];
    if (t == 0) t = time(0);
    strftime(buf, 20, "%Y/%m/%d %H:%M:%S", localtime(&t));
    return buf;
}

/*
 * my_inet_Addr()
 *
 * Scanning a buffer called ip and find four numbers to
 * put them into ret with ipAddr_t type.
 *
 * Remembering that ipAddr_t is uint32_t type actually.
 *
 * Using a pointer to uint8_t type to set every byte of ret.
 * */

ipAddr_t my_inet_Addr(char *ip) {
    int n0, n1, n2, n3;
    ipAddr_t ret;
    uint8_t *ptr;

    if (sscanf(ip, "%d.%d.%d.%d", &n0, &n1, &n2, &n3) < 4) return 0;
    ptr = (uint8_t *)&ret;
    ptr[0] = n0;
    ptr[1] = n1;
    ptr[2] = n2;
    ptr[3] = n3;

    return ret;
}

/*
 * ip_AddrStr()
 *
 * Let IP address recorded by an array be converted to
 * a formatted string.
 *
 * You can put the formatted string into a buffer called buf, which
 * is an assigned buffer by yourself.
 *
 * Otherwise, it puts the string into a static buffer then returning it.
 * */

char *ip_AddrStr(uint8_t *ip, char *buf) {

    static char ipBuf[BUFLEN_IP];

    if (buf == NULL) buf = ipBuf;
    sprintf(buf, "%d.%d.%d.%d", (int)ip[0], (int)ip[1], (int)ip[2], (int)ip[3]);
    return buf;
}

/*
 * eth_MacAddr()
 *
 * It does the similar operation as ip_AddrStr,
 * but it deals with MAC address now.
 *
 * */

char *eth_MacAddr(const uint8_t *addr, char *buf) {
    static char ethbuf[BUFLEN_ETH];
    if (buf == NULL) buf = ethbuf;

    sprintf(buf, "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x", addr[0], addr[1], addr[2],
            addr[3], addr[4], addr[5]);

    return buf;
}

/*
 * print_IP()
 *
 * Printing the IP address recorded by an array.
 *
 * If you want to print some message after print IP address,
 * you can assign a string to endMsg.
 * */

void print_IP(uint8_t *ip, char *endMsg) {

    int i;

    for (i = 0; i < IPV4_ADDR_LEN; i++) {
        if (i != 0) printf(".");
        printf("%d", (int)ip[i]);
    }

    if (endMsg != NULL) printf("%s", endMsg);
}

/*
 * Printing the data of a packet byte by byte.
 *
 * Passing the beginning pointer of packet and its length.
 * */
void print_Data(const uint8_t *data, int len) {

    int i;

    for (i = 0; (i < len && i < MAX_DUMP_LEN); i++) {
        printf("%.2x ", data[i]);
        if ((i + 1) % MAX_LINE_LEN == 0) printf("\n");
    }
    if (i % MAX_LINE_LEN != 0) printf("\n");
}