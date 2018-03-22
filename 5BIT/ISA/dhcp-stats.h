/******************************************************************************/
/*  @brief              Monitorovanie DHCP Komunikacie                        */
/*                             3BIT ISA Projekt                               */
/*  @author         Michal Ormos xormos00@stud.fit.vutbr.cz                   */
/*  @date                         Nov 2016                                    */
/*  @file                       dhcp-stats.h                                  */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <err.h>
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <istream>
#include <vector>
#include <sstream>
#include <ncurses.h>       //Work with Terminal
#include <unistd.h>        //For sleep()
#include <pthread.h>         // std::thread
#include <thread>
#include <cstdlib>
#include <fstream>
#include <signal.h>

using namespace std;

#ifdef __linux__            // for Linux
#include <netinet/ether.h>
#include <time.h>
#include <pcap/pcap.h>
#endif

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE (256)
#endif

#define SIZE_ETHERNET (14)       // offset of Ethernet header to L3 protocol

#define DHCP_UDP_OVERHEAD     (14 + 28 + 8) /* Ethernet header + IP header + UDP header */
#define DHCP_FIXED_NON_UDP    236
#define DHCP_FIXED_LEN        (DHCP_FIXED_NON_UDP + DHCP_UDP_OVERHEAD)
#define DHCP_SNAME_LEN 64
#define DHCP_FILE_LEN 128
#define DHCP_MTU_MAX          1500
#define DHCP_OPTION_LEN       (DHCP_MTU_MAX - DHCP_FIXED_LEN)

struct dhcp_packet {
  u_int8_t  op;
  u_int8_t  htype;
  u_int8_t  hlen;
  u_int8_t  hops;
  u_int32_t xid;
  u_int16_t secs;
  u_int16_t flags;
  struct in_addr ciaddr;
  struct in_addr yiaddr;
  struct in_addr siaddr;
  struct in_addr giaddr;
  unsigned char chaddr [16];
  char sname [DHCP_SNAME_LEN];
  char file [DHCP_FILE_LEN];
  unsigned char options [DHCP_OPTION_LEN];
};

int HexToDec(int p1, int p2, int p3, int p4);
void PrintNcurse();
unsigned int CharToBin(const char* ip);
const char* StringToChar(string str);
void add_address(int lease_time, const char* ip_add);
void delete_adress(int ip_add);
void ParseInput(stringstream& test);
void check_release(const char* ciaddr);
void parse_socket();
void my_handler(int s);
