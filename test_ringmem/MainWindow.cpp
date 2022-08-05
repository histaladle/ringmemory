#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,thread(new QThread())
  ,dominator(new Dominator())
{
    ui->setupUi(this);
    dominator->moveToThread(thread);
    thread->start();
    connect(this,SIGNAL(elected()),dominator,SLOT(onElected()));
    connect(this,SIGNAL(electionLost()),dominator,SLOT(onElectionLost()));
    emit elected();
    char buf[]={char(0xef),char(0xbf),char(0xbd),0};
    qDebug() << "debug18" << QString::fromUtf8(buf);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_stop_released()
{
    emit electionLost();
}
