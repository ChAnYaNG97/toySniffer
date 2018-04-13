#include "MainWindow.h"
#include "../ui/ui_mainframe.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cout << "main thread:" << QThread::currentThreadId() << endl;
    pcap_if_t * devices;
    char errbuf[100];
    currentNumber = 0;

    pcap_findalldevs(&devices, errbuf);
    for(pcap_if_t * pDevice = devices; pDevice != nullptr; pDevice = pDevice->next)
    {
        ui->comboBox->addItem(QString::fromStdString(pDevice->name));
    }

    ui->startsniff->setEnabled(false);

    tableViewItemModel = new QStandardItemModel(this);
    treeViewItemModel = new QStandardItemModel(this);
    flowTableViewItemModel = new QStandardItemModel(this);
    QStringList header;
    header.append("No.");
    header.append("Time");
    header.append("Source");
    header.append("Destination");
    header.append("Protocol");
    header.append("Length");
    header.append("Info");
    tableViewItemModel->setHorizontalHeaderLabels(header);
    tableViewItemModel->setColumnCount(7);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    ui->tableView->setModel(tableViewItemModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int tableWidth = ui->tableView->width();
    ui->tableView->setColumnWidth(0, tableWidth / 16);
    ui->tableView->setColumnWidth(1, tableWidth / 16);
    ui->tableView->setColumnWidth(2, tableWidth / 8);
    ui->tableView->setColumnWidth(3, tableWidth / 8);
    ui->tableView->setColumnWidth(4, tableWidth / 16);
    ui->tableView->setColumnWidth(5, tableWidth / 16);
    ui->tableView->setColumnWidth(6, tableWidth / 2 + 3);


    QStringList flowHeader;
    flowHeader.append("Source IP address");
    flowHeader.append("Flow");
    ui->tableView_2->setModel(flowTableViewItemModel);
    flowTableViewItemModel->setHorizontalHeaderLabels(flowHeader);
    flowTableViewItemModel->setColumnCount(2);
//    flowTableViewItemModel->setTex
    int flowTableWidth = ui->tableView_2->width();
    ui->tableView_2->setColumnWidth(0, flowTableWidth / 2 - 1);
    ui->tableView_2->setColumnWidth(1, flowTableWidth / 2 - 1);
    ui->tableView_2->verticalHeader()->hide();
    ui->tableView_2->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableView_2->setShowGrid(false);



}
 
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDevice()
{
    QString deviceName = ui->comboBox->currentText();
    deviceHandle = pcap_open_live(deviceName.toStdString().c_str(), 65536, 1, 523, nullptr);
    ui->startsniff->setEnabled(true);
}
void MainWindow::handleResult(struct pcap_pkthdr* header, const u_char * data)
{
    struct ether_header * ethhdr = (struct ether_header*) data;

    switch(ntohs(ethhdr->ether_type))
    {
        case 0x0800:
            handleIpPacket(header, data);
            break;
        case 0x0806:
            handleArpPacket(header, data);
            break;
        case 0x86dd:
            //TODO:handleIpv6Package(header, data);
            //Not intended to finish
            break;
        default:
            break;
    }


}

void MainWindow::handleIpPacket(struct pcap_pkthdr * header, const u_char * data)
{
//    cout << packets.size() << endl;
//    cout << currentNumber << endl;
    struct ip * iph = (struct ip *) (data + sizeof(struct ether_header));

    QList<QStandardItem*> list;

    list.append(new QStandardItem(QString::fromStdString(to_string(currentNumber++))));
    struct timeval currentTime;
    gettimeofday(&currentTime, nullptr);
    double time = (currentTime.tv_sec - startTime.tv_sec) * 1e6 + currentTime.tv_usec - startTime.tv_usec;
    list.append(new QStandardItem(QString::fromStdString(to_string(time / 1e6))));
    list.append(new QStandardItem(QString(inet_ntoa(iph->ip_src))));
    list.append(new QStandardItem(QString(inet_ntoa(iph->ip_dst))));


    //14 is the length of ethernet header, ip_len means the total size of package(from the ip layer)
    struct tcphdr * tcp_hdr = (struct tcphdr *)(data + sizeof(struct ether_header) + sizeof(struct ip));
    struct udphdr * udp_hdr = (struct udphdr *)(data + sizeof(struct ether_header) + sizeof(struct ip));

    string info;
    switch((int)iph->ip_p)
    {
        case 6: //TCP
            if(ntohs(tcp_hdr->th_dport) == 80 || ntohs(tcp_hdr->th_sport) == 80)//HTTP
                list.append(new QStandardItem(QString("HTTP")));
            else
                list.append(new QStandardItem(QString("TCP")));

            list.append(new QStandardItem(QString::fromStdString(to_string(ntohs(iph->ip_len) + sizeof(ether_header)))));
            info = to_string(ntohs(tcp_hdr->th_sport)) + " → " + to_string(ntohs(tcp_hdr->th_dport));
            if(ntohs(tcp_hdr->th_flags & TH_ACK))
                info += " [ACK] ";
            if(ntohs(tcp_hdr->th_flags & TH_SYN))
                info += " [SYN] ";
            if(ntohs(tcp_hdr->th_flags & TH_PUSH))
                info += " [PSH] ";
            info += " Seq=" + to_string(ntohs(tcp_hdr->th_seq));
            info += " Ack=" + to_string(ntohs(tcp_hdr->th_ack));
            info += " Win=" + to_string(ntohs(tcp_hdr->th_win));
            list.append(new QStandardItem(QString::fromStdString(info)));
            packets.push_back(pair<int, QByteArray *>((int)(ntohs(iph->ip_len) + sizeof(struct ether_header)), new QByteArray((char*)data + sizeof(ether_header),(int)ntohs(iph->ip_len))));
            tableViewItemModel->appendRow(list);



            break;

        case 17://UDP
            list.append(new QStandardItem(QString("UDP")));


            list.append(new QStandardItem(QString::fromStdString(to_string(ntohs(iph->ip_len) + sizeof(ether_header)))));
            info = to_string(ntohs(udp_hdr->uh_sport)) + " → " + to_string(ntohs(udp_hdr->uh_dport)) + " LENGTH=" + to_string(ntohs(udp_hdr->uh_ulen));
            list.append(new QStandardItem(QString::fromStdString(info)));
            packets.push_back(pair<int, QByteArray *>((int)(ntohs(iph->ip_len) + sizeof(struct ether_header)), new QByteArray((char*)data + sizeof(ether_header),(int)ntohs(iph->ip_len))));
            tableViewItemModel->appendRow(list);
            break;

        case 1://TODO:ICMP 不想做了
            break;

        default:
            break;



    }


    //update flow
    flowTableViewItemModel->removeRows(0, flowTableViewItemModel->rowCount());
    string ip_src = inet_ntoa(iph->ip_src);
    float flow = 0;
    auto search = flowMap.find(ip_src);
    if(search != flowMap.end())
    {
        flow = search->second;
        flow += (1.0 * ntohs(iph->ip_len) + sizeof(ether_header)) / 1e3;
        flowMap[ip_src] = flow;

    }
    else
    {
        flowMap.insert(pair<string, float>(ip_src, (1.0 * ntohs(iph->ip_len) + sizeof(ether_header)) / 1e3));
    }

//    for(auto iter = flowMap.begin(); iter != flowMap.end(); iter++)
//    {
//        cout << iter->first << ":" << iter->second << endl;
//    }


    //sort(flowMap.begin(), flowMap.end(), comp);
    vector<pair<string, float>> elems(flowMap.begin(), flowMap.end());
    sort(elems.begin(), elems.end(), [](pair<string, float> a, pair<string, float> b){return a.second > b.second;});

    QList<QStandardItem *> flowList;
    for(auto iter = elems.begin(); iter != elems.end(); iter++)
    {
        flowList.clear();
        flowList.append(new QStandardItem(QString::fromStdString(iter->first)));
        char flow_2[10];
        sprintf(flow_2, "%.2f", iter->second);
        flowList.append(new QStandardItem(QString::fromStdString(string(flow_2) + "KiB")));
        flowTableViewItemModel->appendRow(flowList);
    }


}

void MainWindow::handleArpPacket(struct pcap_pkthdr * header, const u_char * data)
{
    struct arphdr * arp_hdr = (struct arphdr *)(data + sizeof(ether_header));
//    arp_hdr->ar_sha[0];
    struct ether_header * ethhdr = (struct ether_header*)(data);
    u_short  operation = ntohs(arp_hdr->ar_op);

    char src_mac[20];
    char src[5];
    char dst_mac[20];
    char dst[5];
    memset(src_mac, 0 ,20);
    memset(src, 0 ,5);
    memset(dst_mac, 0 ,20);
    memset(dst, 0 ,5);

    for(int i = 0; i < 6; i++)
    {
        sprintf(src, "%02x:", ethhdr->ether_shost[i]);
        sprintf(dst, "%02x:", ethhdr->ether_dhost[i]);
        if(i == 5)
        {
            sprintf(src, "%02x", ethhdr->ether_shost[i]);
            sprintf(dst, "%02x", ethhdr->ether_dhost[i]);
        }

        strcat(src_mac, src);
        strcat(dst_mac, dst);

    }


//    struct	arphdr {
//        u_short	ar_hrd;		/* format of hardware address */
//#define ARPHRD_ETHER 	1	/* ethernet hardware format */
//#define ARPHRD_IEEE802	6	/* token-ring hardware format */
//#define ARPHRD_FRELAY 	15	/* frame relay hardware format */
//#define ARPHRD_IEEE1394	24	/* IEEE1394 hardware address */
//#define ARPHRD_IEEE1394_EUI64 27 /* IEEE1394 EUI-64 */
//        u_short	ar_pro;		/* format of protocol address */
//        u_char	ar_hln;		/* length of hardware address */
//        u_char	ar_pln;		/* length of protocol address */
//        u_short	ar_op;		/* one of: */
//#define	ARPOP_REQUEST	1	/* request to resolve address */
//#define	ARPOP_REPLY	2	/* response to previous request */
//#define	ARPOP_REVREQUEST 3	/* request protocol address given hardware */
//#define	ARPOP_REVREPLY	4	/* response giving protocol address */
//#define ARPOP_INVREQUEST 8 	/* request to identify peer */
//#define ARPOP_INVREPLY	9	/* response identifying peer */
///*
// * The remaining fields are variable in size,
// * according to the sizes above.
// */
//#ifdef COMMENT_ONLY
//        u_char	ar_sha[];	/* sender hardware address */
//        u_char	ar_spa[];	/* sender protocol address */
//        u_char	ar_tha[];	/* target hardware address */
//        u_char	ar_tpa[];	/* target protocol address */
//#endif
//    };
    QList<QStandardItem*> list;

    list.append(new QStandardItem(QString::fromStdString(to_string(currentNumber++))));
    struct timeval currentTime;
    gettimeofday(&currentTime, nullptr);
    double time = (currentTime.tv_sec - startTime.tv_sec) * 1e6 + currentTime.tv_usec - startTime.tv_usec;

    list.append(new QStandardItem(QString::fromStdString(to_string(time / 1e6))));

    list.append(new QStandardItem(QString::fromStdString(string(src_mac))));
    list.append(new QStandardItem(QString::fromStdString(string(dst_mac))));
    list.append(new QStandardItem(QString("ARP")));
    list.append(new QStandardItem(QString("64")));

    char src_proto[20];
    char src_p[5];
    char dst_proto[20];
    char dst_p[5];

    memset(src_proto, 0 ,20);
    memset(src_p, 0 ,5);
    memset(dst_proto, 0 ,20);
    memset(dst_p, 0 ,5);

    u_char * pAr_spa = (u_char *)arp_hdr + 3 * sizeof(u_short) + 2 * sizeof(u_char) + ((int)(arp_hdr->ar_hln)) * sizeof(u_char);
    u_char * pAr_tpa = pAr_spa + ((int)(arp_hdr->ar_hln)) * sizeof(u_char) + ((int)(arp_hdr->ar_pln)) * sizeof(u_char);
    for(int i = 0; i < 4; i++)
    {
        if(i == 3) {
            sprintf(src_p, "%d", pAr_spa[i]);
            sprintf(dst_p, "%d", pAr_tpa[i]);
        }
        else
        {
            sprintf(src_p, "%d.", pAr_spa[i]);
            sprintf(dst_p, "%d.", pAr_tpa[i]);

        }
        strcat(src_proto, src_p);
        strcat(dst_proto, dst_p);

    }

    string info;

    if(operation == ARPOP_REQUEST)//request
    {
        info = "Who has " + string(dst_proto)  + "? Tell " + string(src_proto);
    }
    else if(operation == ARPOP_REPLY)
    {
        info = string(src_proto) + " is at " + string(src_mac);
    }
    list.append(new QStandardItem(QString::fromStdString(info)));

    tableViewItemModel->appendRow(list);
    packets.push_back(pair<int, QByteArray *>(42, new QByteArray((char*)data ,42)));
}

void MainWindow::startSniff()
{

    sniffer = new SnifferThread();
    gettimeofday(&startTime, NULL);

    connect(sniffer, SIGNAL(resultReady(struct pcap_pkthdr *, const u_char*)), this, SLOT(handleResult(struct pcap_pkthdr * , const u_char *)));
    connect(sniffer, SIGNAL(finished()), sniffer, SLOT(deleteLater()));
    sniffer->setDevice(this->deviceHandle);
    sniffer->start();
}

void MainWindow::stopSniff()
{

    sniffer->terminate();
}

void MainWindow::restartSniff()
{
    stopSniff();
    tableViewItemModel->removeRows(0, tableViewItemModel->rowCount());
    flowTableViewItemModel->removeRows(0, flowTableViewItemModel->rowCount());

    packets.clear();
    flowMap.clear();
    gettimeofday(&startTime, NULL);

    currentNumber = 0;
    startSniff();

}


void MainWindow::packetDetail(QModelIndex index)
{

    int length  = packets[index.row()].first;
    QByteArray * array = packets[index.row()].second;
    QString str;
    for(auto a : *array)
    {

        if((a >= 32 && a <= 126) || a == 10)//可显示字符或者LR(回车)
            str += a;
        else
            str += '.';
    }
    ui->hexEdit->clear();
    ui->hexEdit_2->clear();
    ui->hexEdit_2->appendPlainText(str);
    ui->hexEdit->appendPlainText(array->toHex(' '));
}
void MainWindow::disPlayIpPacketDetail(QModelIndex index)
{

}