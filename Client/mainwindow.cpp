#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString host = ui->lineEdit->text();
    client = new Client(host, port, this);

    ui->lineEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
}
