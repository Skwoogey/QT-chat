#include "tcp_request_disconnect.h"

TCPRequestDisconnect::TCPRequestDisconnect(const QByteArray &a) : TCPMessage(a, eMessageType::requestDisconnect)
{
    if(!isValidMessage)
        return;
}

void TCPRequestDisconnect::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
}

QByteArray TCPRequestDisconnect::getInnerByteMessage()
{
    return QByteArray(0, Qt::Initialization::Uninitialized);
}
