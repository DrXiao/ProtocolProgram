#include "common.h"

/*
 * How to get your EthAddr and IPAddr ?
 *
 * Using command on terminal !!
 *
 * $ ifconfig
 *
 *  Finding the network interface that you use.
 *      inet -> your IPAddr
 *      ether -> your EthAddr
 *
 * */
uint8_t myEthAddr[] = {0x8c, 0x16, 0x45, 0x80, 0x40, 0x56};

uint8_t myIPAddr[] = {192, 168, 1, 108};

/*
 * Getting your default gateway by command
 *
 * $ ip route
 *
 *  You can see your gateway at first line.
 *
 * */
uint8_t defaultArpIP[] = {192, 168, 1, 1};
