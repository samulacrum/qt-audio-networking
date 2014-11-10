#include "model.h"

/**
 * @brief ClientInfo::ClientInfo
 * @param parent
 * @param clientAddress
 */
ClientInfo::ClientInfo(QObject *parent, QString clientAddress) : QObject(parent)
{
    qDebug() << "Client Created";
    address = clientAddress;

    //start the timeout timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
    timer->start(2000);
}

ClientInfo::~ClientInfo()
{
    delete timer;
}

void ClientInfo::timerExpired()
{
    qDebug() << "Client Timeout:" << address;
    emit clientTimeout(address);
}

void ClientInfo::restartTimer()
{
    //qDebug() << "Timer Restarted";
    timer->start(2000);
}

QString ClientInfo::getAddress() const
{
    return address;
}



/**
 * @brief ClientList::ClientList
 * @param parent
 */
ClientList::ClientList(QObject *parent)
    :QAbstractTableModel(parent)
{
    clients = QList<ClientInfo *>();
}

int ClientList::rowCount(const QModelIndex &) const
{
    return clients.size();
}

int ClientList::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant ClientList::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (clients.size() > 0)
            return clients.at(index.row())->getAddress(); //problem?
    }
    return QVariant();
}

void ClientList::addClient(QString clientAddress)
{
    if(!hasAddress(clientAddress)) {
        beginInsertRows(QModelIndex(), 0, 0);
        ClientInfo *newClient = new ClientInfo(this, clientAddress);
        clients.insert(0, newClient);
        connect(newClient, SIGNAL(clientTimeout(QString)), this, SLOT(clientTimeout(QString)));
        endInsertRows();
        qDebug() << "Client Added, size: " << clients.size();
    }
}

QHostAddress ClientList::getAddressAt(const QModelIndex &index)
{
    //qDebug() << "get address at (clientlist) called" << clients.at(index.row())->getAddress();
    if (clients.size() > 0)
        return QHostAddress(clients.at(index.row())->getAddress());
    else
        return QHostAddress();
}

//iterate through our list, return true if we find a matching address (and restart it's timer)
bool ClientList::hasAddress(QString address)
{
    if (clients.size() > 0) {
        for (int i = 0; i < clients.size(); ++i) {
            if (clients.at(i)->getAddress() == address) {
                //qDebug() << "Timer Restarted for: " << address << "at" << clients.at(i)->getAddress();
                clients.at(i)->restartTimer();
                return true;
            }
        }
    }
    return false;
}

void ClientList::clientTimeout(QString address)
{
    int loc;
    //remove the timeout client
    for (int i = 0; i < clients.size(); ++i) {
        if (clients.at(i)->getAddress() == address) {
            loc = i;
        }
    }
    beginRemoveRows(QModelIndex(), loc, loc);
    ClientInfo *temp = clients.at(loc);
    clients.removeAt(loc);
    delete temp;
    endRemoveRows();
    qDebug() << "Client Removed, size: " << clients.size();
}

int ClientList::getSize() const
{
    return clients.size();
}
