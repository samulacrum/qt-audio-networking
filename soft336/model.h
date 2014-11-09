#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel> //remove
#include <QAbstractTableModel>
#include <QtNetwork>
#include <QTimer>

#define TCP_PORT 8003

class ClientInfo : public QObject
{
    Q_OBJECT
public:
    ClientInfo() {}
    ClientInfo(QObject *parent, QString clientAddress);
    void restartTimer();
    QString getAddress() const;
private:
    QTimer *timer;
    QString address;
    QTcpSocket *socketTCP;
private slots:
    void readyRead();
    void timerExpired();
signals:
    void clientTimeout();
};


class ClientList : public QAbstractTableModel
{
    Q_OBJECT
public:
    ClientList(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHostAddress getAddressAt(const QModelIndex &index);
    bool hasAddress(QString address);
private:
    QList<ClientInfo *> clients;
public slots:
    void appendClient(QString clientAddress);
private slots:
    void clientTimeout();
};

#endif // MODEL_H
