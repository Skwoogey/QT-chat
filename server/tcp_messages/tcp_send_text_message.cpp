#include "tcp_send_text_message.h"

TCPSendTextMessage::TCPSendTextMessage(const QByteArray &a) : TCPMessage(a, eMessageType::sendTextMessage)
{
    if(!isValidMessage)
        return;

    if(msgSize < sizeof(msgSize) + 4)
    {
        qDebug() << "ERROR(TCPSendTextMessage): Message too small! length: " << a.length()
                 << " msgSize: " << msgSize;

        isValidMessage = false;
        return;
    }

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    roomID = *getValuePtrFromArray<quint32>(a, msgPtr);
    text = getStringFromBA(a, msgPtr);
}

void TCPSendTextMessage::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
    qDebug() << "roomID:" << roomID;
    qDebug() << "text:" << text;
}

QByteArray TCPSendTextMessage::getInnerByteMessage()
{
    QByteArray byteBlock((char*)&roomID, sizeof (roomID));

    byteBlock.append(getBAfromString(text));

    return byteBlock;
}
