#include "model.h"

/**
 * @brief ClientInfo::ClientInfo default constructor for ClientInfo.
 * @param parent the parent object.
 * @param clientAddress the client IP address.
 */
ClientInfo::ClientInfo(QObject *parent, QString clientAddress) : QObject(parent)
{
    qDebug() << "Client Created";
    address = clientAddress;

    //start the timeout timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
    timer->start(1000);
}

ClientInfo::~ClientInfo()
{
    delete timer;
}

/**
 * @brief ClientInfo::timerExpired called when timeout timer expires, informs the model.
 */
void ClientInfo::timerExpired()
{
    qDebug() << "Client Timeout:" << address;
    emit clientTimeout(address);
}

/**
 * @brief ClientInfo::restartTimer restarts a timer for a client.
 */
void ClientInfo::restartTimer()
{
    //qDebug() << "Timer Restarted";
    timer->start(1000);
}

/**
 * @brief ClientInfo::getAddress get a clients IP address.
 * @return string containing clients IP address
 */
QString ClientInfo::getAddress() const
{
    return address;
}

/**
 * @brief ClientList::ClientList default constructor for ClientList.
 * @param parent the parent object.
 */
ClientList::ClientList(QObject *parent)
    :QAbstractTableModel(parent)
{
    clients = QList<ClientInfo *>();
}

/**
 * @brief ClientList::rowCount gets the size of the ClientList.
 * @return int of number of clients.
 */
int ClientList::rowCount(const QModelIndex &) const
{
    return clients.size();
}

/**
 * @brief ClientList::columnCount returns amount of columns.
 * @return int of the number of columns.
 */
int ClientList::columnCount(const QModelIndex &) const
{
    return 3;
}

/**
 * @brief ClientList::data overloaded function for displaying the data in the table.
 * @param index
 * @param role
 * @return
 */
QVariant ClientList::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole)
    {
        if (clients.size() > 0) {
            if (col == 0)
                return clients.at(row)->getAddress();
        }
    }
    if (role == Qt::BackgroundRole) {
        if (clients.size() > 0) {
            if (col == 1) {
                if(clients.at(row)->isBroadcasting)
                    return QBrush(Qt::green);
                else
                    return QBrush(Qt::red);
            }
            if (col == 2) {
                if(clients.at(row)->isListening)
                    return QBrush(Qt::green);
                else
                    return QBrush(Qt::red);
            }
        }
    }
    return QVariant();
}

/**
 * @brief ClientList::headerData overloaded function for setting the header columns in the table.
 * @param section
 * @param orientation
 * @param role
 * @return
 */
QVariant ClientList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
     {
         if (orientation == Qt::Horizontal) {
             switch (section)
             {
             case 0:
                 return QString("IP");
             case 1:
                 return QString("Broadcasting");
             case 2:
                 return QString("Listening");
             }
         }
     }
     return QVariant();
}

/**
 * @brief ClientList::processClient process a client, adds them to the list if they don't exist, or updates an existing entry.
 * @param clientAddress address of the client.
 * @param controlString control string we have received.
 */
void ClientList::processClient(QString clientAddress, QString controlString)
{
    //add the client if it doesn't exist
    if(!hasAddress(clientAddress)) {
        beginInsertRows(QModelIndex(), 0, 0);
        ClientInfo *newClient = new ClientInfo(this, clientAddress);
        clients.insert(0, newClient);
        connect(newClient, SIGNAL(clientTimeout(QString)), this, SLOT(clientTimeout(QString)));
        endInsertRows();
        qDebug() << "Client Added, size: " << clients.size();
    }

    //process the control string, and update accordingly
    qDebug() << "control string: " << controlString;
    QStringList control = controlString.split(":", QString::SkipEmptyParts);
    if (control.size() == 3) {
        for (int i = 0; i < clients.size(); ++i) {
            if (clients.at(i)->getAddress() == clientAddress) {
                if(control.at(1).contains("broadcasting_yes")) {
                    clients.at(i)->isBroadcasting = true;
                    QModelIndex index = createIndex(i,1);
                    emit dataChanged(index, index);
                }
                if(control.at(1).contains("broadcasting_no")) {
                    clients.at(i)->isBroadcasting = false;
                    QModelIndex index = createIndex(i,1);
                    emit dataChanged(index, index);
                }
                if(control.at(2).contains("listening_yes")) {
                    clients.at(i)->isListening = true;
                    QModelIndex index = createIndex(i,2);
                    emit dataChanged(index, index);
                }
                if(control.at(2).contains("listening_no")) {
                    clients.at(i)->isListening = false;
                    QModelIndex index = createIndex(i,2);
                    emit dataChanged(index, index);
                }
            }
        }
    }
}

/**
 * @brief ClientList::getAddressAt returns a client address at the index specified.
 * @param index index of the client.
 * @return address of the client at index.
 */
QHostAddress ClientList::getAddressAt(const QModelIndex &index)
{
    if (clients.size() > 0)
        return QHostAddress(clients.at(index.row())->getAddress());
    else
        return QHostAddress();
}

/**
 * @brief ClientList::hasAddress checks if the list contains the address, and if it does, restarts its timer.
 * @param address the address to check
 * @return true or false if we have the client already.
 */
bool ClientList::hasAddress(QString address)
{
    if (clients.size() > 0) {
        for (int i = 0; i < clients.size(); ++i) {
            if (clients.at(i)->getAddress() == address) {
                clients.at(i)->restartTimer();
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief ClientList::clientTimeout called when a client timeouts. removes the client, and informs the view.
 * @param address
 */
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
