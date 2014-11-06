#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QtNetwork>
#include <QTimer>

class ClientInfo : public QObject
{
    Q_OBJECT
public:
    ClientInfo() {}
    ClientInfo(QObject *parent, QString clientAddress);
    void restartTimer();
    QString getAddress() const;
private:
    QTimer *timer;
    QString address;
private slots:
    void timerExpired();
signals:
    void clientTimeout();
};


class ClientList : public QAbstractListModel
{
    Q_OBJECT
public:
    ClientList(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHostAddress getAddressAt(const QModelIndex &index);
    bool hasAddress(QString address);
private:
    //QStringList clients;
    QList<ClientInfo *> clients;
public slots:
    void appendClient(QString clientAddress);
};

#endif // MODEL_H
