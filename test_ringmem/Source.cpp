#include "Source.h"
#include <QDebug>
Source::Source(RingMemory *rd_cache) : rd_cache(rd_cache)
  ,rdbuffer_len(2*1024*1024),rdbuffer(new uchar[rdbuffer_len])
  ,_command(1)
  ,_status(0)
  ,sumintv(0),clkintv(0),lastclk(0),_speed(0)
{
    setAutoDelete(false);
}

void Source::run()
{
    sumintv=0; clkintv=0; lastclk=0; _speed=0;
    _sum=0; _clk=0;
    int rdlen=0;
    QFile file("D:/work/video/supermarket.mp4");
    if(!file.open(QFile::ReadOnly)) {
        qDebug() << "open file failed";
    }
    statusMtx.lock(); _status=1; statusMtx.unlock();
    while(command()) {
        if(file.atEnd()) {
            break;
        }
        rdlen=int(file.read(reinterpret_cast<char*>(rdbuffer),rdbuffer_len));
        if(rdlen<0) {
            qDebug() << "read failed";
            break;
        }
        while(command()&&rd_cache->writableCount()<rdlen){}
        rd_cache->write(rdbuffer,rdlen);
        int clk1=clock();
        _sum+=rdlen; _clk+=clk1-lastclk;
        sumintv+=rdlen; clkintv+=clk1-lastclk;
        if(clkintv>=10) {speedMtx.lock(); _speed=sumintv/clkintv; sumintv=0; clkintv=0; speedMtx.unlock();}
        lastclk=clk1;
    }
    statusMtx.lock(); _status=0; statusMtx.unlock();
    file.close();
    qDebug() << "source finished";
}
