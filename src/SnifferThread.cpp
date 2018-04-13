//
// Created by 杨晨 on 2018/4/9.
//

#include "SnifferThread.h"
void SnifferThread::setDevice(pcap_t * devhdl)
{
    this->deviceHandle = devhdl;
}