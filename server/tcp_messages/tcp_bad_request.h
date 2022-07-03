#ifndef TCPBADREQUEST_H
#define TCPBADREQUEST_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPBadRequest : public TCPMessage
{
public:
    enum eBadRequestReason
    {
        userNotLogged,
        failedToParseMessage,
        illegalRequest
    };

    TCPBadRequest() : TCPMessage(eMessageType::badRequest) {};
    TCPBadRequest(const QByteArray &a);

    void setReason(eBadRequestReason rsn) { reason = rsn; }
    eBadRequestReason getReason() { return reason; }

    void print() override;

protected:
    QByteArray getInnerByteMessage() override;

private:
    eBadRequestReason reason;
};

#endif // TCPBADREQUEST_H
