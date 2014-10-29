#include "model.h"

ClientList::ClientList(QObject *parent) : QAbstractListModel(parent)
{
}

int ClientList::rowCount(const QModelIndex &) const
{
    return 2;
}

QVariant ClientList::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QString("Row: %1").arg(index.row() + 1);
    }
    return QVariant();
}
