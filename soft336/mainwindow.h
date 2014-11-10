#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QThread>

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

signals:
    void startAudio();
    void endAudio();
    void deviceChanged(QAudioDeviceInfo devinfo);
    void sendControlString(QString data);
    void endBroadcast();

private slots:
    void getDeviceInfo();
    void processBroadcast(QString address, QString controlString);

    void on_listenButton_clicked();
    void on_broadcastButton_clicked();
    void on_stopListenButton_clicked();
    void on_endBroadcastButton_clicked();
    void on_outputVolumeControl_sliderMoved(int position);
    void on_inputVolumeControl_sliderMoved(int position);
    void on_deviceComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    Client  *client;
    Server *server;
    ClientList *clients;

    QThread *serverThread;
    QThread *clientThread;
};

#endif // MAINWINDOW_H
