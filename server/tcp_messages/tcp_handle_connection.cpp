#include "tcp_handle_connection.h"

TCPHandleConnection::TCPHandleConnection(const QByteArray &a) : TCPMessage(a, eMessageType::handleConnection)
{
    if(!isValidMessage)
        return;

    if(msgSize != sizeof(msgSize) + sizeof(msgType) + sizeof(connectionStatus) + sizeof(user_id))
    {
        qDebug() << "ERROR(TCPHandleConnection): Message size mismatch!"
                 << "\nlength: " << a.length()
                 << "\nmsgSize: " << msgSize;

        isValidMessage = false;
        return;
    }

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    user_id = *(quint32*)getArrayPtr(a, msgPtr, sizeof(user_id));
    connectionStatus = *(eConnectionStatus*)getArrayPtr(a, msgPtr, sizeof(connectionStatus));
}

void TCPHandleConnection::setStatus(TCPHandleConnection::eConnectionStatus rsn)
{
    connectionStatus = rsn;
}

void TCPHandleConnection::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;

    qDebug() << "Connection Status" << connectionStatus;
}

QByteArray TCPHandleConnection::getInnerByteMessage()
{
    QByteArray message(sizeof(connectionStatus) + sizeof(user_id), Qt::Initialization::Uninitialized);

    int msgPtr = 0;
    insertIntoArray(message, (char*)&user_id, sizeof(user_id), msgPtr);
    insertIntoArray(message, (char*)&connectionStatus, sizeof(connectionStatus), msgPtr);

    return message;
}
