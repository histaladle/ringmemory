#include "Dominator.h"
#include <QDebug>

#define DEBUG_FILE
#define TEST

Dominator::Dominator(QObject *parent) : QObject(parent)
  ,alarm(new QTimer(this))
  ,runner(new QThreadPool(this))
  ,cache(new RingMemory(8*1024*1024))
  ,source(new Source(cache))
  ,destiny(new Destiny(cache))
{
    qDebug() << "po" << runner->maxThreadCount() ;
    alarm->setInterval(1);
    alarm->setSingleShot(true);
    connect(alarm,SIGNAL(timeout()),this,SLOT(work()));
}

void Dominator::work()
{
    if(source->status()) {}
    else {
        while(cache->readableCount()){}
        destiny->command(0);
    }
//    qDebug() << "speed" << source->speed() << destiny->speed();
    if(runner->activeThreadCount()) {
        alarm->start();
    }
    else {
        qDebug() << "src" << source->_sum << source->_clk
                 << "dst" << destiny->_sum << destiny->_clk;
    }
}

void Dominator::onElected()
{
    qDebug() << "celebrate eleceted";
    runner->start(destiny);
    runner->start(source);
    alarm->start();
}

void Dominator::onElectionLost()
{
    alarm->stop();
    source->command(0);
    destiny->command(0);
    runner->waitForDone();
    qDebug() << "election lost";
}

Dominator::~Dominator()
{
    onElectionLost();
}

