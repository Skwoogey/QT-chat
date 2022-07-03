#include "tcp_chat_history.h"

TCPChatHistory::TCPChatHistory(const QByteArray &a) : TCPMessage(a, eMessageType::chatHistory)
{
    if(!isValidMessage)
        return;

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    room =  ChatRoom::dispatchChatHistory(a, msgPtr);

    if(room == nullptr)
    {
        isValidMessage = false;
        return;
    }

}

void TCPChatHistory::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
    room->printChatHistory();
}

ChatRoom *TCPChatHistory::getRoom()
{
    if(isValidMessage)
        return room;
    return nullptr;
}

QByteArray TCPChatHistory::getInnerByteMessage()
{
    QByteArray byteBlock((char*)&(room->getID()), sizeof (room->getID()));

    return room->getChatHistory();
}
