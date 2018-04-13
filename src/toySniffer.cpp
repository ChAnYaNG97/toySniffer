#include <iostream>
#include "MainWindow.h"
#include <QApplication>

using namespace std;
int total = 0;

void process_packet_callback(u_char* args, const struct pcap_pkthdr* header, const u_char * buffer);
void print_udp_packet(const u_char * buffer, int size);
int main(int argc, char* argv[]) 
{
    // pcap_if_t* alldevsp, * device;
    // pcap_t* handle;
    // char errbuf[100], *devname, devs[100][100];
    // cout << "Finding available devices..." << endl;
    // if(pcap_findalldevs(&alldevsp, errbuf))
    // {
    //     cout << "Error finding devices!" << endl;
    //     exit(1);
    // }

    // cout << "Done!" << endl;
    // int count = 1, n;
    // cout << "Available devices are:" << endl;
    // for(device = alldevsp ; device != NULL ; device = device->next)
    // {
    //     printf("%d. %s - %s\n" , count , device->name , device->description);
    //     if(device->name != NULL)
    //     {
    //         strcpy(devs[count] , device->name);
    //     }
    //     count++;
    // }
    // cin >> n;
    // devname = devs[n];
    // cout << "Opening " + string(devname) + " for sniffing!" << endl;
    // handle = pcap_open_live(devname, 65536, 1, 512, errbuf);

    // if(handle == nullptr)
    // {
    //     cerr << "Cannot open the device " + string(devname) << endl;
    //     exit(1);
    // }

    // cout << "Open handle Done!" << endl;
    // pcap_loop(handle, -1, process_packet_callback, NULL);
    // return 0;

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    app.exec();


}
    
    




// }

// void process_packet_callback(u_char* arg, const struct pcap_pkthdr* header, const u_char * buffer)
// {
//     int size = header->len;
//     struct ip* iph = (struct ip *)(buffer + sizeof(struct ether_header));
    
//     if(iph->ip_p == 17) //UDP
//     {
//         print_udp_packet(buffer, size);
//     }
//     if(iph->ip_p == )

// }
// void print_udp_packet(const u_char * buffer, int size)
// {
//     struct ip* iph = (struct ip *)(buffer + sizeof(struct ether_header));
//     unsigned int iphdrlength = iph -> ip_len;

//     struct udphdr* udph = (struct udphdr*)(buffer + sizeof(struct ether_header) + iphdrlength);
//     cout << string(inet_ntoa(iph->ip_src)) << endl;
//     cout << string(inet_ntoa(iph->ip_dst)) << endl;
//     cout << endl;

// }