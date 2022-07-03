#include "tcp_bad_request.h"

TCPBadRequest::TCPBadRequest(const QByteArray &a) : TCPMessage(a, eMessageType::badRequest)
{
    if(!isValidMessage)
        return;

    if(msgSize != sizeof(msgSize) + sizeof(msgType) + sizeof(reason))
    {
        qDebug() << "ERROR(TCPBadRequest): Message size mismatch!"
                 << "\nlength: " << a.length()
                 << "\nmsgSize: " << msgSize;

        isValidMessage = false;
        return;
    }

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    reason = *getValuePtrFromArray<eBadRequestReason>(a, msgPtr);
}

void TCPBadRequest::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
    qDebug() << "badRequestReason" << reason;
}

QByteArray TCPBadRequest::getInnerByteMessage()
{
    return QByteArray((char*)&reason, sizeof(reason));
}
