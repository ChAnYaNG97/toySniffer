#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QThread>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <pcap/pcap.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
//#include <netinet/ip_icmp.h>   //Provides declarations for icmp header
#include <netinet/udp.h>   //Provides declarations for udp header
#include <netinet/tcp.h>   //Provides declarations for tcp header
#include <netinet/ip.h>    //Provides declarations for ip header
#include <net/if_arp.h>
#include "SnifferThread.h"
#define COMMENT_ONLY
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    pcap_t * deviceHandle;
    Ui::MainWindow *ui;
    QStandardItemModel* tableViewItemModel;
    QStandardItemModel* treeViewItemModel;
    QStandardItemModel* flowTableViewItemModel;

    SnifferThread* sniffer;
    int currentNumber;
    struct timeval startTime;

    void handleIpPacket(struct pcap_pkthdr*, const u_char*);
    void handleArpPacket(struct pcap_pkthdr*, const u_char*);
    vector<pair<int, QByteArray *> > packets;
    vector<int> lengths;

    unordered_map<string, float> flowMap;



private slots:
    void setDevice();
    void startSniff();
    void stopSniff();
    void restartSniff();
    void handleResult(struct pcap_pkthdr*, const u_char*);
    void packetDetail(QModelIndex);
    void disPlayIpPacketDetail(QModelIndex);


};

#endif // MAINWINDOW_H
