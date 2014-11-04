#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(size()); //prevent window resizing
    getDeviceInfo(); //read available input devices
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listenButton_clicked()
{
    client = new Client(this);
    ui->listenButton->setEnabled(false);
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
    QAudioDeviceInfo devinfo = ui->deviceComboBox->itemData(ui->deviceComboBox->currentIndex()).value<QAudioDeviceInfo>();
    input = new AudioInput(devinfo, this);
    server = new Server(this);
    connect(input, SIGNAL(dataReady(QByteArray)), server, SLOT(writeData(QByteArray)));

    ui->deviceComboBox->setEnabled(false);
    ui->broadcastButton->setEnabled(false);
}
