#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>

#include "client.h"
#include "server.h"
#include "audioinput.h"

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

private:
    Ui::MainWindow *ui;
    Client  *client;
    Server *server;
    AudioInput *input;
};

#endif // MAINWINDOW_H
