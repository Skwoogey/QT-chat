#include "tcpmessage.h"

TCPMessage::TCPMessage(const QByteArray &a, eMessageType msgType) : msgType(msgType)
{
    if(a.length() < sizeof(msgSize))
    {
        qDebug() << "ERROR(TCPMessage): Message too small! msgSize:" <<a.length() ;
        isValidMessage = false;
        return;
    }

    int msgPtr = 0;

    msgSize = *(quint32*)getArrayPtr(a, msgPtr, sizeof(msgSize));
    msgType = *(eMessageType*)getArrayPtr(a, msgPtr, sizeof(msgType));

    if(msgSize > a.length())
    {
        isValidMessage = false;
        qDebug() << a.toHex();
        qDebug() << "ERROR(TCPMessage): Message size doesn't match! length: " << a.length()
                 << "msgSize: " << msgSize;
    }
}

QByteArray TCPMessage::getByteMessage()
{
    auto message = getInnerByteMessage();

    msgSize = message.length() + sizeof(msgSize) + sizeof(eMessageType);

    qDebug() << msgSize;

    QByteArray full_message(msgSize, Qt::Initialization::Uninitialized);
    int msgPtr = 0;
    insertIntoArray(full_message, (char*)&msgSize, sizeof(msgSize), msgPtr);
    insertIntoArray(full_message, (char*)&msgType, sizeof(msgType), msgPtr);
    insertIntoArray(full_message, message.data(), message.length(), msgPtr);

    return full_message;
}

TCPMessage::eMessageType TCPMessage::getMsgType(const QByteArray &a)
{
    return *(eMessageType*)getArrayPtrConst(a, sizeof(msgSize), sizeof(msgType));
}
