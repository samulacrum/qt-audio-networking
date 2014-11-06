#include "model.h"

ClientList::ClientList(QObject *parent)
    : QAbstractListModel(parent),
      clients()
{
}

int ClientList::rowCount(const QModelIndex &) const
{
    return clients.size();
}

QVariant ClientList::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return clients.at(index.row());
    }
    return QVariant();
}

void ClientList::appendClient(QString clientAddress)
{
    beginInsertRows(QModelIndex(), 0, 0);
    //check if already added
    if(!clients.contains(clientAddress)) {
        clients.append(clientAddress);
    }
    endInsertRows();
}

QHostAddress ClientList::getAddressAt(const QModelIndex &index)
{
    return QHostAddress(clients.at(index.row()));
}


ClientInfo::ClientInfo(QObject *parent, QString clientAddress) : QObject(parent)
{
    clientAddress = clientAddress;
    timer = new QTimer();
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
}

void ClientInfo::timerExpired()
{
    emit clientTimeout();
}
