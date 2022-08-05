#ifndef DESTINY_H
#define DESTINY_H

#include <QRunnable>
#include <QMutex>
#include <QFile>
#include "RingMemory.h"
class Destiny : public QRunnable
{
public:
    Destiny(RingMemory *wr_cache);
    RingMemory *wr_cache;
    int wrbuffer_len;
    uchar *wrbuffer;
    void run();
private:
    int _command;
    QMutex commandMtx;
    int _status;
    QMutex statusMtx;
public:
    int command()
    {
        int c;
        commandMtx.lock();
        c=_command;
        commandMtx.unlock();
        return c;
    }
    int command(int cmd)
    {
        commandMtx.lock();
        _command=cmd;
        commandMtx.unlock();
        return cmd;
    }
    int status()
    {
        int s;
        statusMtx.lock();
        s=_status;
        statusMtx.unlock();
        return s;
    }
private:
    int sumintv;
    int clkintv;
    int lastclk;
    int _speed;
    QMutex speedMtx;
public:
    long long _sum;
    long long _clk;
public:
    int speed()
    {
        int s;
        speedMtx.lock();
        s=_speed;
        speedMtx.unlock();
        return s;
    }
};

#endif // DESTINY_H
