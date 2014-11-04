#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    clientList(0)
{
    ui->setupUi(this);

    setFixedSize(size()); //prevent window resizing
    getDeviceInfo(); //read available input devices

    //sample data for client list
    clientList.addClient("hello");
    clientList.addClient("hello");
    clientList.addClient("hello");

    ui->clientListView->setModel(&clientList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listenButton_clicked()
{
    ui->listenButton->setEnabled(false);
    ui->stopListenButton->setEnabled(true);

    //qDebug() << clientList.rowCount();
    //ui->clientListView->

    client = new Client(this);
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

    QAudioDeviceInfo devinfo = ui->deviceComboBox->itemData(ui->deviceComboBox->currentIndex()).value<QAudioDeviceInfo>();
    input = new AudioInput(devinfo, this);
    server = new Server(this);
    connect(input, SIGNAL(dataReady(QByteArray)), server, SLOT(writeData(QByteArray)));
}

void MainWindow::on_stopListenButton_clicked()
{
    ui->listenButton->setEnabled(true);
    ui->stopListenButton->setEnabled(false);
    delete client;
}

void MainWindow::on_endBroadcastButton_clicked()
{
    ui->deviceComboBox->setEnabled(true);
    ui->broadcastButton->setEnabled(true);
    ui->endBroadcastButton->setEnabled(false);
    delete server;
}
