#include "model.h"

/**
 * @brief ClientInfo::ClientInfo
 * @param parent
 * @param clientAddress
 */
ClientInfo::ClientInfo(QObject *parent, QString clientAddress) : QObject(parent)
{
    qDebug() << "Client Created";
    address = clientAddress;

    //start the timeout timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
    timer->start(2000);
    /*
    //start the control string listener
    socketTCP = new QTcpSocket(this);
    socketTCP->connectToHost(address, TCP_PORT);
    connect(socketTCP, SIGNAL(readyRead()), this, SLOT(readyRead()));*/

    //qDebug() << socketTCP;
}

ClientInfo::~ClientInfo()
{
    delete socketTCP;
    delete timer;
}

void ClientInfo::timerExpired()
{
    qDebug() << "Client Timeout";
    emit clientTimeout(address);
}

void ClientInfo::restartTimer()
{
    //qDebug() << "Timer Restarted";
    timer->start(2000);
}

QString ClientInfo::getAddress() const
{
    return address;
}

void ClientInfo::readyRead()
{
    QByteArray data;

    while(socketTCP->bytesAvailable() > 0)
        data.append(socketTCP->readAll());

    qDebug() << "data received" << data << " from " << address;
    //do something with data here

}

/**
 * @brief ClientList::ClientList
 * @param parent
 */
ClientList::ClientList(QObject *parent)
    :QAbstractTableModel(parent)
{
    clients = QList<ClientInfo *>();
}

int ClientList::rowCount(const QModelIndex &) const
{
    return clients.size();
}

int ClientList::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant ClientList::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (clients.size() > 0)
            return clients.at(index.row())->getAddress(); //problem?
    }
    return QVariant();
}

void ClientList::appendClient(QString clientAddress)
{
    if(!hasAddress(clientAddress)) {
        beginInsertRows(QModelIndex(), 0, 0);
        ClientInfo *newClient = new ClientInfo(this, clientAddress);
        clients.insert(0, newClient);
        connect(newClient, SIGNAL(clientTimeout(QString)), this, SLOT(clientTimeout(QString)));
        endInsertRows();
        qDebug() << "Client Added, size: " << clients.size();
    }
}

QHostAddress ClientList::getAddressAt(const QModelIndex &index)
{
    //qDebug() << "get address at (clientlist) called" << clients.at(index.row())->getAddress();
    if (clients.size() > 0)
        return QHostAddress(clients.at(index.row())->getAddress());
}

//iterate through our list, return true if we find a matching address (and restart it's timer)
bool ClientList::hasAddress(QString address)
{
    if (clients.size() > 0) {
        for (int i = 0; i < clients.size(); ++i) {
            if (clients.at(i)->getAddress() == address) {
                //qDebug() << "Timer Restarted for: " << address << "at" << clients.at(i)->getAddress();
                clients.at(i)->restartTimer();
                return true;
            }
        }
    }
    return false;
}

void ClientList::clientTimeout(QString address)
{
    int loc;
    //remove the timeout client
    for (int i = 0; i < clients.size(); ++i) {
        if (clients.at(i)->getAddress() == address) {
            loc = i;
        }
    }
    beginRemoveRows(QModelIndex(), loc, loc);
    qDebug("Client Removed");
    delete clients.at(loc);
    endRemoveRows();
    qDebug() << "Client Removed, size: " << clients.size();
}

/**
 * @brief TCPServer::TCPServer
 * @param parent
 */
TCPServer::TCPServer(QObject *parent) : QObject(parent)
{
    socketTCP = 0;
    serverTCP = new QTcpServer(this);
    serverTCP->listen(QHostAddress::Any, 8003);

    connect(serverTCP, SIGNAL(newConnection()), this, SLOT(acceptTCPConnection()));
}

void TCPServer::acceptTCPConnection() {
    qDebug() << "new Connection";
    socketTCP = serverTCP->nextPendingConnection();
    connect(socketTCP, SIGNAL(disconnected()), socketTCP, SLOT(deleteLater()));
}

void TCPServer::sendData(QByteArray data)
{
    if (socketTCP)
        socketTCP->write(data);
}
