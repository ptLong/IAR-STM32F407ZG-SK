#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"

// Uncomment this if you want to get IP from DHCP server
//#define USE_DHCP

//
// Static IP section
//
// MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5
#define MAC_ADDR0   2
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   0
#define MAC_ADDR4   0
#define MAC_ADDR5   0
 
// Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3
#define IP_ADDR0   192
#define IP_ADDR1   168 
#define IP_ADDR2   0
#define IP_ADDR3   200
   
// NETMASK
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

// Gateway Address
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1  

#endif /* __MAIN_H */


