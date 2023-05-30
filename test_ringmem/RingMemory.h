#ifndef RINGMEMORY_H
#define RINGMEMORY_H

#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <cstring>
#include <pthread.h>
class RingMemory
{
public:
    RingMemory(int buffer_size);
    ~RingMemory();
    uint8_t *buffer;
    const int BUFFER_SIZE;
    int dump(int max_count);
    int remove(int offset,int max_count);
    int read(uint8_t *dst, int max_count);
    int write(uint8_t *src,int count);
    int readableCount();
    int writableCount();
    int status();
    int copy(uint8_t *dst,int size,int offset);
    int reserve(int size);
public:
    int rdIndex;
    int _readableCount;
    int _status;
    static const int ERROR_STATUS;
    static const int EMPTY_STATUS;
    static const int READABLE_STATUS;
    static const int FULL_STATUS;
    static const int WRITE_OVERLAPPED_STATUS;
    pthread_mutex_t mutex;
public:
    RingMemory(RingMemory&)=delete ;
    RingMemory& operator=(RingMemory&)=delete ;
    RingMemory& operator=(const RingMemory&)=delete ;
    uint8_t at(int index);
};

#endif // RINGMEMORY_H
