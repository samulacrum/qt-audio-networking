#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>

#include "model.h"
#include "client.h"
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void getDeviceInfo();
    void on_listenButton_clicked();
    void on_broadcastButton_clicked();
    void on_stopListenButton_clicked();
    void on_endBroadcastButton_clicked();
    void on_outputVolumeControl_sliderMoved(int position);
    void on_inputVolumeControl_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    Client  *client;
    Server *server;
};

#endif // MAINWINDOW_H
