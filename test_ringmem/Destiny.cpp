#include "Destiny.h"
#include <QDebug>
#include <time.h>
Destiny::Destiny(RingMemory *wr_cache) : wr_cache(wr_cache)
  ,wrbuffer_len(2*1024*1024),wrbuffer(new uchar[wrbuffer_len])
  ,_command(1)
  ,_status(0)
  ,sumintv(0),clkintv(0),lastclk(0),_speed(0)
  ,_sum(0),_clk(0)
{
    setAutoDelete(false);
}

void Destiny::run()
{
    sumintv=0; clkintv=0; lastclk=0; _speed=0;
    _sum=0; _clk=0;
    int wrlen=0;
    QFile file("F:/supermarket.dat");
    if(!file.open(QFile::WriteOnly)) {
        qDebug() << "open file failed";
    }
    statusMtx.lock(); _status=1; statusMtx.unlock();
    while(command()) {
        wrlen=wr_cache->pop(wrbuffer,wrbuffer_len);
        if(wrlen==0) {
            continue;
        }
        else if(wrlen<0) {
            qDebug() << "pop failed";
            break;
        }
        file.write(reinterpret_cast<char*>(wrbuffer),wrlen);
        int clk1=clock();
        _sum+=wrlen; _clk+=clk1-lastclk;
        sumintv+=wrlen; clkintv+=clk1-lastclk;
        if(clkintv/(CLOCKS_PER_SEC/1000)>=10) {speedMtx.lock(); _speed=sumintv/clkintv*CLOCKS_PER_SEC; sumintv=0; clkintv=0; speedMtx.unlock();}
        lastclk=clk1;
    }
    statusMtx.lock(); _status=0; statusMtx.unlock();
    file.close();
    qDebug() << "destiny finished";
}
