#include "tcp_request_metadata.h"

TCPRequestMetaData::TCPRequestMetaData(const QByteArray &a) : TCPMessage(a, eMessageType::requestMetaData)
{
    if(!isValidMessage)
        return;
}

void TCPRequestMetaData::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
}

QByteArray TCPRequestMetaData::getInnerByteMessage()
{
    return QByteArray(0, Qt::Initialization::Uninitialized);
}
