//
// Created by 杨晨 on 2018/4/9.
//

#ifndef VSC_TEST_SNIFFERTHREAD_H
#define VSC_TEST_SNIFFERTHREAD_H
#include <QThread>
#include <iostream>
#include <pcap/pcap.h>
#include <time.h>

using namespace std;

class SnifferThread :public QThread{
    Q_OBJECT

public:
    explicit SnifferThread(QObject* parent = 0) : QThread(parent)
    {
        cout << "pcap Thread:"<< QThread::currentThreadId() << endl;
    };

    void setDevice(pcap_t * devhdl);

protected:
    virtual void run() Q_DECL_OVERRIDE
    {

        while(1)
        {
            if(pcap_next_ex(deviceHandle, &header, &pkt_data)) {
                emit resultReady(header, pkt_data);
            }
        }

    }

private:
    pcap_t* deviceHandle;
    static const int ERROR_BUFFER_LENGTH = 100;
    char errbuf[ERROR_BUFFER_LENGTH];
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    time_t startTime;

signals:
    void resultReady(struct pcap_pkthdr *, const u_char*);

};


#endif //VSC_TEST_SNIFFERTHREAD_H
