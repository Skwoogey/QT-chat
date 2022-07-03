#ifndef TCPCHATHISTORY_H
#define TCPCHATHISTORY_H



#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "chat_units/chatroom.h"
#include "utils.h"

class TCPChatHistory : public TCPMessage
{
public:
    TCPChatHistory() : TCPMessage(eMessageType::chatHistory) {};
    TCPChatHistory(const QByteArray &a);

    void print() override;
    ChatRoom* getRoom();
    void setRoom(ChatRoom* rm) { room = rm; };

protected:
    QByteArray getInnerByteMessage() override;

private:
    ChatRoom* room;

};

#endif // TCPCHATHISTORY_H
