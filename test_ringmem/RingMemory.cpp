#include "RingMemory.h"
#include <QDebug>
#define PRESS() pthread_mutex_lock(&mutex)
#define RELEASE() pthread_mutex_unlock(&mutex)
const int RingMemory::ERROR_STATUS=-1;
const int RingMemory::EMPTY_STATUS=0;
const int RingMemory::READABLE_STATUS=1;
const int RingMemory::FULL_STATUS=2;
const int RingMemory::WRITE_OVERLAPPED_STATUS=3;
RingMemory::RingMemory(int buffer_size)
    :BUFFER_SIZE(buffer_size),rdIndex(0)
    ,_readableCount(0),_status(0)
    ,mutex(PTHREAD_MUTEX_INITIALIZER)
{
    buffer=new uint8_t[size_t(buffer_size)];
}

RingMemory::~RingMemory()
{
    delete [] buffer;
    buffer=nullptr;
}

int RingMemory::read(uint8_t *dst, int max_count)
{
    int res=0;
    if(max_count<=0) {
        return res;
    }
    PRESS();
    res=_readableCount;
    res=std::min(res,max_count);
    int rem_cnt=res;
    while(rem_cnt>0) {
        int block=std::min(rem_cnt,BUFFER_SIZE-rdIndex);
        memcpy(dst+res-rem_cnt,buffer+rdIndex,size_t(block));
        rdIndex=(rdIndex+block)%BUFFER_SIZE;
        rem_cnt-=block;
        _readableCount-=block;
    }
    if(_readableCount>0) {
        _status=READABLE_STATUS;
    }
    else {
        _status=EMPTY_STATUS;
    }
//    qDebug() << "after popped readable count" << _readableCount;
    RELEASE();
    res=res-rem_cnt;
    return res;
}

int RingMemory::dump(int max_count)
{
    int res=0;
    if(max_count<=0) {
        return res;
    }
    PRESS();
    res=_readableCount;
    res=std::min(res,max_count);
    int rem_cnt=res;
    while(rem_cnt>0) {
        int block=std::min(rem_cnt,BUFFER_SIZE-rdIndex);
        rdIndex=(rdIndex+block)%BUFFER_SIZE;
        rem_cnt-=block;
        _readableCount-=block;
    }
    if(_readableCount>0) {
        _status=READABLE_STATUS;
    }
    else {
        _status=EMPTY_STATUS;
    }
//    qDebug() << "after popped readable count" << _readableCount;
    RELEASE();
    res=res-rem_cnt;
    return res;
}

int RingMemory::remove(int offset, int max_count)
{
    int res=0;
    if(max_count<=0) {
        return res;
    }
    if(offset<0) {
        return res;
    }
    if(offset>=_readableCount) {
        return res;
    }
    PRESS();
    res=_readableCount-offset;
    res=std::min(res,max_count);
    for(int i=0;i<res;++i) {
        buffer[(rdIndex+offset+i)%BUFFER_SIZE]=buffer[(rdIndex+res+offset+i)%BUFFER_SIZE];
    }
    _readableCount-=res;
    RELEASE();
    return res;
}

int RingMemory::write(uint8_t *src, int count)
{
    int res=0;
    if(count<=0) {
        return res;
    }
    PRESS();
    int rem_cnt=count;
    int wrprobe=(rdIndex+_readableCount)%BUFFER_SIZE;
    while(rem_cnt>0) {
        int block=std::min(rem_cnt,BUFFER_SIZE-wrprobe);
        if(block>BUFFER_SIZE-_readableCount) {
            rdIndex=(wrprobe+block)%BUFFER_SIZE;
            _status=WRITE_OVERLAPPED_STATUS;
            qDebug() << "ring memory write overlapped";
        }
        else {
            _status=READABLE_STATUS;
        }
        memcpy(buffer+wrprobe,src+count-rem_cnt,size_t(block));
        wrprobe=(wrprobe+block)%BUFFER_SIZE;
        rem_cnt-=block;
        _readableCount+block<=BUFFER_SIZE?_readableCount+=block:_readableCount=BUFFER_SIZE;
    }
//    qDebug() << "after pushed readable count" << _readableCount;
    RELEASE();
    res=count-rem_cnt;
    return res;
}

int RingMemory::readableCount()
{
    int res=0;
    PRESS();
    res=_readableCount;
    RELEASE();
    return res;
}

int RingMemory::writableCount()
{
    int res=0;
    PRESS();
    res=BUFFER_SIZE-_readableCount;
    RELEASE();
    return res;
}

int RingMemory::copy(uint8_t *dst, int size, int offset)
{
    int res=size;
    PRESS();
    for(int i=0;i<size;i++) {
        dst[i]=buffer[(rdIndex+offset+i)%BUFFER_SIZE];
//        qDebug() << "debug62" << i << offset+i << (offset+i)%BUFFER_SIZE;
    }
    RELEASE();
    return res;
}

int RingMemory::reserve(int size)
{
    int res=0;
    PRESS();
    if(_readableCount>=size) {
        res=size;
    }
    else {
        res=std::min(size,BUFFER_SIZE);
        int rem_cnt=res;
        while(rem_cnt>0) {
            int block=std::min(rem_cnt,BUFFER_SIZE-rdIndex);
            rdIndex=(rdIndex+block)%BUFFER_SIZE;
            rem_cnt-=block;
            _readableCount-=block;
        }
    }
    RELEASE();
    return res;
}

uint8_t RingMemory::at(int index)
{
    PRESS();
    uint8_t d=buffer[(rdIndex+index)%BUFFER_SIZE];
    RELEASE();
    return d;
}

int RingMemory::status()
{
    PRESS();
    int res=0;
    res=_status;
    RELEASE();
    return res;
}
