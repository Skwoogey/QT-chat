#ifndef TCPNEWMESSAGE_H
#define TCPNEWMESSAGE_H

#include <QObject>
#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "chat_units/chatmessage.h"
#include "utils.h"

class TCPNewTextMessage : public TCPMessage
{
public:
    TCPNewTextMessage() : TCPMessage(eMessageType::newTextMessage) {};
    TCPNewTextMessage(const QByteArray &a);

    void print() override;

    void setMessage(ChatMessage* nmsg) { msg = nmsg; }
    ChatMessage* getMessage() { return msg; }

protected:
    QByteArray getInnerByteMessage() override;
    ChatMessage* msg;

};

#endif // TCPNEWMESSAGE_H
