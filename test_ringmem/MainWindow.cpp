#include "MainWindow.h"
#include "ui_MainWindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_stop_released()
{
    emit electionLost();
}
