#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>

class ClientList : public QAbstractListModel
{
    Q_OBJECT
public:
    ClientList(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};


#endif // MODEL_H
