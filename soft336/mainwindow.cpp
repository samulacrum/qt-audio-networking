#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(size()); //prevent window resizing
    getDeviceInfo(); //read available input devices

    //start server
    QAudioDeviceInfo devinfo = ui->deviceComboBox->itemData(ui->deviceComboBox->currentIndex()).value<QAudioDeviceInfo>();
    server = new Server(devinfo, this);

    //start client
    client = new Client(this);
    connect(client, SIGNAL(clientBroadcastReceived(QString)), server, SLOT(appendClient(QString)));

    QThread serverThread;
    QThread clientThread;

    server->moveToThread(&serverThread);
    client->moveToThread(&clientThread);

    serverThread.start();
    clientThread.start();

    //ui->clientListView->setModel(server->clientList);
    ui->clientListTableView->setModel(server->clientList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listenButton_clicked()
{
    ui->listenButton->setEnabled(false);
    ui->stopListenButton->setEnabled(true);
    client->startListen();
}

void MainWindow::getDeviceInfo()
{
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if (devices.isEmpty()) QMessageBox::warning(this, "Error", "No input device found!");
    for(int i = 0; i < devices.size(); ++i)
        ui->deviceComboBox->addItem(devices.at(i).deviceName(), qVariantFromValue(devices.at(i)));
}

void MainWindow::on_broadcastButton_clicked()
{
    ui->deviceComboBox->setEnabled(false);
    ui->broadcastButton->setEnabled(false);
    ui->endBroadcastButton->setEnabled(true);

    server->startAudioSend();
}

void MainWindow::on_stopListenButton_clicked()
{
    ui->listenButton->setEnabled(true);
    ui->stopListenButton->setEnabled(false);
    client->endListen();
}

void MainWindow::on_endBroadcastButton_clicked()
{
    ui->deviceComboBox->setEnabled(true);
    ui->broadcastButton->setEnabled(true);
    ui->endBroadcastButton->setEnabled(false);

    server->endAudioSend();
}


void MainWindow::on_outputVolumeControl_sliderMoved(int position)
{
    client->setVolume((float)position / 10);
}



void MainWindow::on_inputVolumeControl_sliderMoved(int position)
{
    server->setVolume((float)position / 10);
}
