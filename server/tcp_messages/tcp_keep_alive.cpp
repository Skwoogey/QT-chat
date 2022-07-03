#include "tcp_keep_alive.h"

TCPKeepAlive::TCPKeepAlive(const QByteArray &a): TCPMessage(a, eMessageType::keepAlive)
{
    if(!isValidMessage)
        return;
}

void TCPKeepAlive::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
}

QByteArray TCPKeepAlive::getInnerByteMessage()
{
    return QByteArray();
}
