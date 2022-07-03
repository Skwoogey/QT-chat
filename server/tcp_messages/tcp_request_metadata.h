#ifndef TCPREQUESTMETADATA_H
#define TCPREQUESTMETADATA_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPRequestMetaData : public TCPMessage
{
public:
    TCPRequestMetaData() : TCPMessage(eMessageType::requestMetaData) {};
    TCPRequestMetaData(const QByteArray &a);

    void print() override;

protected:
    QByteArray getInnerByteMessage() override;

private:

};

#endif // TCPREQUESTMETADATA_H
