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

void ClientList::addClient(QString client)
{
    appendClient(client);
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
