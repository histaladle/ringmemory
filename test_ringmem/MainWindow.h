#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Dominator.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
private:
    QThread *thread;
    Dominator *dominator;

signals:
    void elected();
    void electionLost();
private slots:
    void on_pushButton_stop_released();
};

#endif // MAINWINDOW_H
