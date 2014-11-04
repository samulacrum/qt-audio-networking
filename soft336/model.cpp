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

void ClientList::addClient(QString client) {
    clients.append(client);
}
