#ifndef DOMINATOR_H
#define DOMINATOR_H

#include <QObject>
#include <QThreadPool>
#include <QTimer>
#include "RingMemory.h"
#include <QFile>
#include "Source.h"
#include "Destiny.h"
class Dominator : public QObject
{
    Q_OBJECT
public:
    explicit Dominator(QObject *parent = nullptr);
    ~Dominator();
private:
    QTimer *alarm;
    QThreadPool *runner;
    RingMemory *cache;
    Source *source;
    Destiny *destiny;
signals:
public slots:
    void onElected();
    void work();
    void onElectionLost();
};

#endif // DOMINATOR_H
