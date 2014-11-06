#include "model.h"

//Client List
ClientList::ClientList(QObject *parent)
    : QAbstractListModel(parent)
{
    clients = QList<ClientInfo *>();
}

int ClientList::rowCount(const QModelIndex &) const
{
    return clients.size();
}

QVariant ClientList::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        //qDebug() << "get address called";
        return clients.at(index.row())->getAddress();
    }
    return QVariant();
}

void ClientList::appendClient(QString clientAddress)
{
    beginInsertRows(QModelIndex(), 0, 0);

    if(!hasAddress(clientAddress)) {
        clients.append(new ClientInfo(this, clientAddress));
    }

    //old insert code
    /*if(!clients.contains(clientAddress)) {

        clients.append(clientAddress);
    }*/
    endInsertRows();
}

QHostAddress ClientList::getAddressAt(const QModelIndex &index)
{
    qDebug() << "called" << clients.at(index.row())->getAddress();
    return QHostAddress(clients.at(index.row())->getAddress());
}

//iterate through our list, return true if we find a matching address
bool ClientList::hasAddress(QString address)
{
    QList<ClientInfo *>::iterator i;
    for (i = clients.begin(); i != clients.end(); ++i) {
        if(((*i)->getAddress() == address)) {
            (*i)->restartTimer();
            return true;
        }
    }
    return false;
}

//Client Info
ClientInfo::ClientInfo(QObject *parent, QString clientAddress) : QObject(parent)
{
    qDebug() << "Client Created";
    address = clientAddress;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
    timer->start(500);
}

void ClientInfo::timerExpired()
{
    qDebug() << "Client Timeout";
    emit clientTimeout();
}

void ClientInfo::restartTimer()
{
    qDebug() << "Timer Restarted";
    timer->start(500);
}

QString ClientInfo::getAddress() const
{
    return address;
}
