#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    clients(new ClientList(this)),
    serverThread(new QThread(this)),
    clientThread(new QThread(this))
{
    ui->setupUi(this);

    setFixedSize(size()); //prevent window resizing
    getDeviceInfo(); //read available input devices

    //start server
    server = new Server(clients);
    connect(this, SIGNAL(startAudio()), server, SLOT(startAudioSend()));
    connect(this, SIGNAL(endAudio()), server, SLOT(endAudioSend()));
    connect(this, SIGNAL(sendControlString(QString)), server, SLOT(updateBroadcast(QString)));
    connect(this, SIGNAL(deviceChanged(QAudioDeviceInfo)), server, SLOT(changeDevice(QAudioDeviceInfo)));

    //start client
    client = new Client();
    connect(client, SIGNAL(clientBroadcastReceived(QString, QString)), this, SLOT(processBroadcast(QString, QString)));

    //move them to seperate threads
    server->moveToThread(serverThread);
    client->moveToThread(clientThread);

    serverThread->start();
    clientThread->start();

    //finally, set the model for the table view, and resize columns
    ui->clientListTableView->setModel(clients);
    for (int i = 0; i < ui->clientListTableView->horizontalHeader()->count(); ++i)
    {
        ui->clientListTableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
}

void MainWindow::processBroadcast(QString address, QString controlString)
{
    clients->processClient(address, controlString);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serverThread;
    delete clientThread;
    delete server;
    delete client;

}

void MainWindow::on_listenButton_clicked()
{
    ui->listenButton->setEnabled(false);
    ui->stopListenButton->setEnabled(true);
    emit(sendControlString("listening_yes"));
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
    emit(sendControlString("broadcasting_yes"));
    emit(startAudio());
}

void MainWindow::on_stopListenButton_clicked()
{
    ui->listenButton->setEnabled(true);
    ui->stopListenButton->setEnabled(false);
    emit(sendControlString("listening_no"));
    client->endListen();
}

void MainWindow::on_endBroadcastButton_clicked()
{
    ui->deviceComboBox->setEnabled(true);
    ui->broadcastButton->setEnabled(true);
    ui->endBroadcastButton->setEnabled(false);
    emit(sendControlString("broadcasting_no"));
    emit(endAudio());
}

void MainWindow::on_outputVolumeControl_sliderMoved(int position)
{
    client->setVolume((float)position / 10);
}

void MainWindow::on_inputVolumeControl_sliderMoved(int position)
{
    server->setVolume((float)position / 10);
}


void MainWindow::on_deviceComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QAudioDeviceInfo devinfo = ui->deviceComboBox->itemData(ui->deviceComboBox->currentIndex()).value<QAudioDeviceInfo>();
    emit(deviceChanged(devinfo));
}
