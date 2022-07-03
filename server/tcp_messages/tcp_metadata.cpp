#include "tcp_metadata.h"

TCPMetaData::TCPMetaData(const QByteArray &a) : TCPMessage(a, eMessageType::MetaData)
{
    if(!isValidMessage)
        return;

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    MD = new ChatServer(nullptr, false);

    MD->fromByteBlock(a, msgPtr);

    if(!MD->isValid())
    {
        qDebug() << "ERROR(MetaData): failed to parse metadata!";
        isValidMessage = false;
        return;
    }
}

void TCPMetaData::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
    MD->print();
}

ChatServer *TCPMetaData::getMetaData()
{
    if(isValidMessage)
        return MD;
    return nullptr;
}

QByteArray TCPMetaData::getInnerByteMessage()
{
    return MD->getByteBlock();
}
