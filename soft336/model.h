#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <QStringList>

class ClientList : public QAbstractListModel
{
    Q_OBJECT
public:
    ClientList(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void addClient(QString client);
private:
    QStringList clients;
};

#endif // MODEL_H
