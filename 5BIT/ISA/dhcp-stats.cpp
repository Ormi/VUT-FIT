/******************************************************************************/
/*  @brief              Monitorovanie DHCP Komunikacie                        */
/*                             3BIT ISA Projekt                               */
/*  @author         Michal Ormos xormos00@stud.fit.vutbr.cz                   */
/*  @date                         Nov 2016                                    */
/*  @file                      dhcp-stats.cpp                                 */
/******************************************************************************/

#include "dhcp-stats.h"

// Pre rozsah siete
// addr_min
// addr_max
vector<string> sub;

// pre max pocet adries a ich obsadenost
// max_num_addr
// real_num_addr
vector<int> pub;

// Pocitadlo casu pre adresy
vector<int> tub;

// Pamat na pouzite IP adresy k casu
vector<string> iub;

int NumOfDomains;     // Pocet domen zadanych pri stupe programu
bool output;          // vypis do suboru vs. vypis ncurse
pcap_t *handle;       // packet capture handle
int ToFile;           // premenna pre -c <int>
int fileCount = 0;    // pocitadlo pre -c <int>
int n = 0;            // packets counter
ofstream myfile;      //file operator

/**
 * Function for converting hexadecimal value to decimal representation of lease time
 * Input is 4 decimal number (representation of hexadecimal)
 * @param hexadecimalne cisla z packetu
 * @return dekadicke cislo reprezentujuce lease time
 */
int HexToDec(int p1, int p2, int p3, int p4)
{
  int n, c, k, v;
  char str[31] = "\0";

  for (v = 1; v <=4; v++) {
    switch(v) {
    case 1: n = p1; break;
    case 2: n = p2; break;
    case 3: n = p3; break;
    case 4: n = p4; break;
    }
    for (c = 7; c >= 0; c--)
    {
      k = n >> c;
      if (k & 1) {
        strcat(str, "1");
      } else {
        strcat(str, "0");
      }
    }
  }
  int j, s;
  int sum = 0;

  for(sum=0, j=0, s=strlen(str)-1; s >= 0; s--, ++j){
      if(str[s] == '1'){
          sum = sum + pow(2,j);
      }
  }
  return sum;
}

/**
 * Riadenia vypisu programu. V pripade prepinace -c do suboru, inak implicitne
 * pomocou kniznice ncurse na terminal.
 */
void PrintNcurse() {
  if (output) {
    if (myfile.is_open() && fileCount == ToFile)
    {
      fileCount = 0;
      myfile << "IP Prefix,Max hosts,Allocated addresses,Utilization\n";
      float utilization;
      for (int i = 0; i < NumOfDomains*2; i++) {
        utilization = float(pub[i+1])/float(pub[i])*100;
        if(i%2==0) {
          myfile << sub[i].c_str() << "," << pub[i] << "," << pub[i+1] << "," << utilization << "\n";
        }
      }
    }
  } else {
    initscr();			/* Start curses mode 		  */
    erase();
    printw("IP Prefix\tMax hosts\tAllocated addresses\tUtilization\n");
    float utilization;
    for (int i = 0; i < NumOfDomains*2; i++) {
      utilization = float(pub[i+1])/float(pub[i])*100;
      if(i%2==0) {
        printw("%s\t%d\t\t%d\t\t\t%.2f%\n", sub[i].c_str(), pub[i], pub[i+1], utilization);
      }
    }
    refresh();		/* Print it on to the real screen */
    timeout(1000);
  }
}

/**
 * Function for convertion array of char from pcap ipbv4 address sniffer to
 * binary representation for comparision
 * @param pole znakov tvoriace ip adresu
 * @return ip adresa v binarnom tvare
 */
unsigned int CharToBin(const char* ip) {
  unsigned char s1, s2, s3, s4;
  unsigned int uip;

  sscanf(ip,"%hhu.%hhu.%hhu.%hhu",&s1,&s2,&s3,&s4);
  uip = (s1<<24) | (s2<<16) | (s3<<8) | s4; //store all values in 32bits unsigned int
  return uip;
}

/**
* Function for convertion string representation of ipv4 address to binary
* representation for comparision
* @param ip adresa v tvare stringu
* @return ip adresa v tvare poli znakov
*/
const char* StringToChar(string str) {
  char *cstr;
  cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  return cstr;
}

/**
* Spracovanie zachytenych packetov
* funkcia je volana od pcap_loop()
* @param man pcap
*/
void mypcap_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
  struct ip *my_ip;               // Pointer na zaciatok IP hlavicky
  struct ether_header *eptr;      // Pointer na zaciatok Ethernet hlavicky
  u_int size_ip;
  int lease_time;
  n++;

  // Citanie ethernet hlavicky
  eptr = (struct ether_header *) packet;
  if (ntohs(eptr->ether_type) == ETHERTYPE_IP) {
    my_ip = (struct ip*) (packet+SIZE_ETHERNET);        // preskoc ethernet hlavicku
    size_ip = my_ip->ip_hl*4;                           // dlzka ip hlavicky
    if (my_ip->ip_p == 17) {
      struct dhcp_packet *my_dhcp;
      my_dhcp = (struct dhcp_packet *) (packet+SIZE_ETHERNET+size_ip+8); // 8 je UDP hlavicka ale da sa krajsie nejake sizeof(udphdr) co je 8 tiez...

          // DHCP ACK
          if (my_dhcp->options[6] == 5) {
            lease_time = HexToDec(my_dhcp->options[9], my_dhcp->options[10], my_dhcp->options[11], my_dhcp->options[12]);
            add_address(lease_time, inet_ntoa(my_dhcp->yiaddr));
          }
          // DHCP RELEASE
          if (my_dhcp->options[6] == 7) {
            check_release(inet_ntoa(my_dhcp->ciaddr));
          }
    }
  }
}

/**
* Kontrola ci prichadzajuci release je na nejaku z mnou monitorovanych adries
* @param pole znakov, ktore reprezentuje IP adresu
*/
void check_release(const char* ciaddr) {
  unsigned int uip1, uip2;
  const char* ip;
  unsigned int i;
  for (i = 0; i <= iub.size(); i++) {
    uip1 = CharToBin(ciaddr);
    ip = StringToChar(iub[i]);
    uip2 = CharToBin(ip);
    if (uip1 == uip2) {
      delete_adress(i);
      iub.erase (iub.begin() + i);
      tub.erase (tub.begin() + i);
      return;
    }
  }
}

/**
 * Pridava ziskanu IP adresu z sniffu do vektorov v pripade ak zapade do jednej
 * z pozadovanych sieti, do struktury vektorov
 * @param cas vyprsania platnosti adresy
 * @param IP adresa v tvare poli znakov
 */
void add_address(int lease_time, const char* ip_add) {
  unsigned int uip1, uip2;
  const char* ip;
  unsigned int i;
  int j, z;
  j = 0;
  bool add = true;

  for (z = 1; z <= NumOfDomains; z++) {
    add = true;
    uip1 = CharToBin(ip_add);
    ip = StringToChar(sub[j]);
    uip2 = CharToBin(ip);

    if(uip1 > uip2) {
      j++;

      ip = StringToChar(sub[j]);
      uip2 = CharToBin(ip);

      if (uip1 < uip2) {
        pub[j] += 1;
        for (i = 0; i < iub.size(); i++) {
          if (iub[i] == ip_add) {
            add = false;
          }
        }
        if (add) {
          iub.push_back(ip_add);
          tub.push_back(lease_time);
        }
      }
      j++;
    } else {
      j++;
    }
  }
}

/**
 * Odobera adresu z vektorov v pripade ak jej lease time skonci
 * Porovna ku ktorym sietam bola pridana a odtial ich z pocitadla sieti odoberie
 * @param bitova reprezentacia ip adresy
 */
void delete_adress(int ip_add) {
  int j, i;
  const char* ip;
  unsigned int uip1, uip2;
  j = 0;
  for (i = 1; i <= NumOfDomains; i++) {

    ip = StringToChar(iub[ip_add]);
    uip1 = CharToBin(ip);
    ip = StringToChar(sub[j]);
    uip2 = CharToBin(ip);

    if(uip1 > uip2) {
      j++;
      ip = StringToChar(sub[j]);
      uip2 = CharToBin(ip);
      if (uip1 < uip2) {
        pub[j] -= 1;
      }
      j++;
    } else {
      j++;
    }
  }
}

/**
 * Spravuje vstupne adresy sieti X.X.X.X/X do vektorov
 * Vypocita ich max pocet adries, min a max pouzitelnu adresu
 * @param stream ip adries na vstupe spolu s znakom '.' a znakom '/'
 */
void ParseInput(stringstream& test) {
  string segment;
  string str;
  vector<int> array;
  int temp, pref_dec, host_max, x;
  int j = 0;
  unsigned int i;
  while(getline(test, segment, '/'))
  {
      sub.push_back(segment);
      if (j>=1 && (j%2)!=0) {
        str = sub[j-1];
        for (i=0; i<str.length(); i++)
        {
            if (str[i] == '.')
                str[i] = ' ';
        }

        stringstream ss(str);
        while (ss >> temp)
            array.push_back(temp);

        string::size_type sz;
        pref_dec = stoi(sub[j],&sz);
        host_max = pow(2,32-pref_dec);
        if (host_max == 2 || host_max == 1) {
          array[3] += host_max;
          pub.push_back(host_max);
          pub.push_back(0);
        } else {
          pub.push_back(host_max-2);
          pub.push_back(0);
          x = host_max/255;
          host_max -= 1;
          if ((array[3]+host_max) <= 255) {
            array[3] += host_max;
          } else if ((array[2]+x) <= 255) {
            array[2] += x;
          } else {
            printf("Neosetrena chyba pri zistovani max adresy\n");
          }
        }
        stringstream oss;
        oss << array[0] << "." << array[1] << "." << array[2] << "." << array[3];
        sub.pop_back();
        sub.push_back(oss.str());

        array.pop_back();
        array.pop_back();
        array.pop_back();
        array.pop_back();
      }
      j++;
  }
}

/**
 * Hlavny thread programu opakujuci sa kazdu jednu sekundu
 * Odpocitava lease time a maze adresy, ktorym skonci lease time
 */
void parse_socket() {
  int size_tube;
  while(1) {
    sleep(1);
    fileCount += 1;
    size_tube = tub.size();
    for (int i = 0; i < size_tube; i++) {
      // dekrementuj cas o jedna pre kazdy lease time
      tub[i] -= 1;
      // Ak je nejaky lease-time klesne na nulu, tak ho vymaz s vektora ip adries
      // rovnako ako aj z vektora lease-timeov
      if (tub[i] == 0) {
        delete_adress(i);
        iub.erase (iub.begin() + i);
        tub.erase (tub.begin() + i);
        i = 0;
        size_tube -= 1;
      }
    }
    PrintNcurse();
  }
}

void my_handler(int s) {
  pcap_close(handle);
  endwin();
  myfile.close();
  exit(0);
}

int main(int argc, char *argv[]){
  char errbuf[PCAP_ERRBUF_SIZE];  // constant defined in pcap.h
  char *dev;                      // input device
  struct in_addr a,b;
  bpf_u_int32 netaddr;            // network address configured at the input device
  bpf_u_int32 mask;               // network mask of the input device
  struct bpf_program fp;          // the compiled filter
  const char *port = "portrange 67-68";

  thread first (parse_socket);    // thread pre monitoring
  stringstream test;              //
  int i, z;                       //
  ToFile = 0;                     // -c <int> hodnota
  bool OwnPort = false;
  int Cext = 0;                   // pomocna premenna pre -c
  output = false;                 // vypis do suboru vs vypis ncruse
  struct sigaction sigIntHandler; // signal prerusenia pre ctrl+c

  // Citac preruseni (pre CTRL+C)
  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Osetrenie minimalneho poctu argumentov
  if (argc <= 1)
    errx(1,"Too less amount of arguments\nUsage: %s <ip-prefix> [ <ip-prefix> [ ... ] ]", argv[0]);

  // Spracovanie parametra -c a -i v roznom poradi
  if (strstr(argv[1], "-c") != NULL ) {
    ToFile = atoi(argv[2]);
    Cext = 2;
    output = true;
    myfile.open ("log.csv");
  } else if (strstr(argv[1], "-i") != NULL ) {
    dev = argv[2];
    OwnPort = true;
    Cext = 2;
  }
  if (strstr(argv[3], "-i") != NULL ) {
    dev = argv[4];
    OwnPort = true;
    Cext = 4;
  }
  if (strstr(argv[3], "-c") != NULL ) {
    ToFile = atoi(argv[4]);
    Cext = 4;
    output = true;
    myfile.open ("log.csv");
  }

  // Osetrenie nevalidneho vstupu ipadresa/prefix
  for (i = Cext+1; i < argc; i++) {
    if ((strstr(argv[i], "/") == NULL) || (strstr(argv[i], ".") == NULL)) {
      errx(1,"Zly format vstupu");
    }
  }

  // Spocitanie poctu prefixov pre statistiky na vstupe
  for (i = 0; i < argc; i++) {
      NumOfDomains = i;
  }

  // Osetrenie poctu argumentov s pouzitim prepinaca -c
  if (ToFile > 0 && OwnPort == true) {
      NumOfDomains -= 4;
  } else if (ToFile > 0 || OwnPort == true) {
      NumOfDomains -= 2;
  }

  // Vlozenie vsetkych prefixov na vstupe do stringstreamu a jeho nasledne
  // spracovanie do vektorov
  for (z = Cext+1; z <= NumOfDomains+Cext; z++) {
    test << argv[z] << "/";
  }
  ParseInput(test);

////////////////////////////////////////////////////////////////////////////////

  // open the device to sniff data
  if (!OwnPort) {
    if ((dev = pcap_lookupdev(errbuf)) == NULL)
      err(1,"Can't open input device");
  }

  // get IP address and mask of the sniffing interface
  if (pcap_lookupnet(dev,&netaddr,&mask,errbuf) == -1)
    err(1,"pcap_lookupnet() failed");

  a.s_addr=netaddr;
  printf("Opening interface \"%s\" with net address %s,",dev,inet_ntoa(a));
  b.s_addr=mask;
  printf("mask %s for listening...\n",inet_ntoa(b));

  // open the interface for live sniffing
  if ((handle = pcap_open_live(dev,BUFSIZ,1,1000,errbuf)) == NULL)
    err(1,"pcap_open_live() failed");

  // compile the filter
  if (pcap_compile(handle,&fp,port,0,netaddr) == -1)
    err(1,"pcap_compile() failed");

  // set the filter to the packet capture handle
  if (pcap_setfilter(handle,&fp) == -1)
    err(1,"pcap_setfilter() failed");

  // read packets from the interface in the infinite loop (count == -1)
  // incoming packets are processed by function mypcap_handler
  pcap_loop(handle,-1,mypcap_handler,NULL);
  first.detach();

  return 0;
}
