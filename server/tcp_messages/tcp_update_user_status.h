#ifndef TCPUPDATEUSERSTATUS_H
#define TCPUPDATEUSERSTATUS_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "chat_units/chatuser.h"
#include "utils.h"

class TCPUpdateUserStatus : public TCPMessage
{
public:
    TCPUpdateUserStatus() : TCPMessage(eMessageType::updateUserStatus) {};
    TCPUpdateUserStatus(const QByteArray &a);

    void print() override;

    void setUser(ChatUser* cu) { user = cu; }
    ChatUser* getUser() { return user; }

protected:
    QByteArray getInnerByteMessage() override;

private:
    ChatUser* user;

};

#endif // TCPUPDATEUSERSTATUS_H
