#ifndef TCPREQUESTCHATHISTORY_H
#define TCPREQUESTCHATHISTORY_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPRequestChatHistory : public TCPMessage
{
public:
    TCPRequestChatHistory() : TCPMessage(eMessageType::getChatHistory) {};
    TCPRequestChatHistory(const QByteArray &a);

    void print() override;

    void setRoomID(quint32 rid) { roomID = rid; }
    quint32 getRoomID() { return roomID; }

protected:
    QByteArray getInnerByteMessage() override;

private:
    quint32 roomID;
};
#endif // TCPREQUESTCHATHISTORY_H
