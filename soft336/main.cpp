#include "mainwindow.h"
#include <QApplication>
#include <QListView>
#include "model.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QListView listView;
    //ClientList clientList(0);
    //listView.setModel(&clientList);
    //listView.show();
    MainWindow w;
    w.show();

    return a.exec();
}
