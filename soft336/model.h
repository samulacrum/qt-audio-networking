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
    ClientInfo(QObject *parent, QString clientAddress);
    ~ClientInfo();
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
    void clientTimeout(QString cAddress);
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
    int getSize() const;
private:
    QList<ClientInfo *> clients;
public slots:
    void addClient(QString clientAddress);
private slots:
    void clientTimeout(QString cAddress);
};

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = 0);

public slots:
    void sendData(QByteArray data);
private slots:
    void acceptTCPConnection();
private:
    QTcpSocket *socketTCP;
    QTcpServer *serverTCP;

public:

};

#endif // MODEL_H
