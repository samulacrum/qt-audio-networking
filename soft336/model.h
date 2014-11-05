#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QtNetwork>

class ClientList : public QAbstractListModel
{
    Q_OBJECT
public:
    ClientList(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHostAddress getAddressAt(const QModelIndex &index);
private:
    QStringList clients;
public slots:
    void appendClient(QString clientAddress);
};

#endif // MODEL_H
