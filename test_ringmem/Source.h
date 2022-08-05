#ifndef SOURCE_H
#define SOURCE_H

#include <QRunnable>
#include <QFile>
#include <QMutex>
#include "RingMemory.h"
class Source : public QRunnable
{
public:
    Source(RingMemory *rd_cache);
    RingMemory *rd_cache;
    int rdbuffer_len;
    uchar *rdbuffer;
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

#endif // SOURCE_H
