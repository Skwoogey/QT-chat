#ifndef TCPKEEPALIVE_H
#define TCPKEEPALIVE_H

#include <QObject>
#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPKeepAlive : public TCPMessage
{
public:
    TCPKeepAlive() : TCPMessage(eMessageType::keepAlive) {};
    TCPKeepAlive(const QByteArray &a);

    void print() override;

protected:
    QByteArray getInnerByteMessage() override;

};


#endif // TCPKEEPALIVE_H
