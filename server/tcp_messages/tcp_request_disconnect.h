#ifndef TCPREQUESTDISCONNECT_H
#define TCPREQUESTDISCONNECT_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPRequestDisconnect : public TCPMessage
{
public:
    TCPRequestDisconnect() : TCPMessage(eMessageType::requestDisconnect) {};
    TCPRequestDisconnect(const QByteArray &a);

    void print() override;

protected:
    QByteArray getInnerByteMessage() override;

private:

};

#endif // TCPREQUESTDISCONNECT_H
