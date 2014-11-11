#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel> //remove
#include <QAbstractTableModel>
#include <QtNetwork>
#include <QTimer>
#include <QBrush>

#define TCP_PORT 8003

/**
 * @brief The ClientInfo class, holds client IP address, and a timeout timer.
 */
class ClientInfo : public QObject
{
    Q_OBJECT
public:
    ClientInfo(QObject *parent, QString clientAddress);
    ~ClientInfo();
    void restartTimer();
    QString getAddress() const;
private:
    QTimer *timer;
    QString address;
private slots:
    void timerExpired();
signals:
    void clientTimeout(QString cAddress);
public:
    bool isListening;
    bool isBroadcasting;
};

/**
 * @brief The ClientList class, holds a QList of ClientInfo, which is used to store network clients.
 */
class ClientList : public QAbstractTableModel
{
    Q_OBJECT
public:
    ClientList(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QHostAddress getAddressAt(const QModelIndex &index);
    bool hasAddress(QString address);
private:
    QList<ClientInfo *> clients;
public slots:
    void processClient(QString clientAddress, QString controlString);
private slots:
    void clientTimeout(QString cAddress);
};


#endif // MODEL_H
