#ifndef TCPREFUSECONNECTION_H
#define TCPREFUSECONNECTION_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPHandleConnection : public TCPMessage
{
public:
    enum eConnectionStatus
    {
        connectionAccepted,
        invalidPasswordOrUsername,
        userAlreadyOnline
    };

    TCPHandleConnection() : TCPMessage(eMessageType::handleConnection) {};
    TCPHandleConnection(const QByteArray &a);

    void setStatus(eConnectionStatus rsn);
    eConnectionStatus getStatus() { return connectionStatus; };

    void setUserId(quint32 id) { user_id = id; }
    quint32 getUserId() { return user_id; };

    void print() override;

protected:
    QByteArray getInnerByteMessage() override;

private:
    eConnectionStatus connectionStatus;
    quint32 user_id;
};

#endif // TCPREFUSECONNECTION_H
