#include "tcp_new_message.h"

TCPNewTextMessage::TCPNewTextMessage(const QByteArray &a) : TCPMessage(a, eMessageType::newTextMessage)
{
    if(!isValidMessage)
        return;

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    msg = new ChatMessage(nullptr, false);

    msg->fromByteBlock(a, msgPtr);

    if(!msg->isValid())
    {
        qDebug() << "ERROR(TCPNewTextMessage): failed to parse metadata!";
        isValidMessage = false;
        return;
    }
}

void TCPNewTextMessage::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
    msg->print();
}

QByteArray TCPNewTextMessage::getInnerByteMessage()
{
    return msg->getByteBlock();
}
