#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clientList = new ClientList(this);

    setFixedSize(size()); //prevent window resizing
    getDeviceInfo(); //read available input devices

    //start server
    server = new Server(this);

    //start client
    client = new Client(this);
    connect(client, SIGNAL(clientBroadcastReceived(QString)), server, SLOT(appendClient(QString)));

    ui->clientListView->setModel(server->clientList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listenButton_clicked()
{
    ui->listenButton->setEnabled(false);
    ui->stopListenButton->setEnabled(true);
    client->enableListen();
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
    connect(input, SIGNAL(dataReady(QByteArray)), server, SLOT(writeDatagram(QByteArray)));
}

void MainWindow::on_stopListenButton_clicked()
{
    ui->listenButton->setEnabled(true);
    ui->stopListenButton->setEnabled(false);
    client->disableListen();
}

void MainWindow::on_endBroadcastButton_clicked()
{
    ui->deviceComboBox->setEnabled(true);
    ui->broadcastButton->setEnabled(true);
    ui->endBroadcastButton->setEnabled(false);
    delete input;
}
