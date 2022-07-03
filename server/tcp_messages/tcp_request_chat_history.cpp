#include "tcp_request_chat_history.h"


TCPRequestChatHistory::TCPRequestChatHistory(const QByteArray &a) : TCPMessage(a, eMessageType::getChatHistory)
{
    if(!isValidMessage)
        return;

    if(msgSize < sizeof(msgSize) + 4)
    {
        qDebug() << "ERROR(TCPRequestChatHistory): Message too small! length: " << a.length()
                 << " msgSize: " << msgSize;

        isValidMessage = false;
        return;
    }

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    roomID = *getValuePtrFromArray<quint32>(a, msgPtr);
}

void TCPRequestChatHistory::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
    qDebug() << "roomID:" << roomID;
}

QByteArray TCPRequestChatHistory::getInnerByteMessage()
{
    QByteArray byteBlock((char*)&roomID, sizeof (roomID));

    return byteBlock;
}
