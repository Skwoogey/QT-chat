#ifndef TCPMETADATA_H
#define TCPMETADATA_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "chat_units/chatserver.h"
#include "utils.h"

class TCPMetaData : public TCPMessage
{
public:
    TCPMetaData() : TCPMessage(eMessageType::MetaData) {};
    TCPMetaData(const QByteArray &a);

    void print() override;
    ChatServer* getMetaData();
    void setMetaData(ChatServer* nMD) { MD = nMD; };

protected:
    QByteArray getInnerByteMessage() override;

private:
    ChatServer* MD;

};


#endif // TCPMETADATA_H
