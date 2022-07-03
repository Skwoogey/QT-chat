#ifndef TCPMESSAGE_H
#define TCPMESSAGE_H

#include <QtCore>
#include <assert.h>
#include "utils.h"

class TCPMessage
{
public:
    enum eMessageType
    {
        requestConnection,
        handleConnection,
        requestDisconnect,

        requestMetaData,
        MetaData,

        keepAlive,
        updateUserStatus,

        sendTextMessage,
        newTextMessage,

        getChatHistory,
        chatHistory,

        badRequest
    };

    TCPMessage(eMessageType msgType) : msgType(msgType) {};
    TCPMessage(const QByteArray &a, eMessageType msgType);

    QByteArray getByteMessage();
    bool isValid() { return isValidMessage; };
    quint32 getSize() { return msgSize; }
    virtual void print() = 0;

    static eMessageType getMsgType(const QByteArray& a);

protected:
    bool isValidMessage = true;
    virtual QByteArray getInnerByteMessage() = 0;
    eMessageType msgType;

    quint32 msgSize;

};

/*


class TCPBadRequest: public TCPMessage
{
public:
    TCPBadRequest() { msgType = eMessageType::badRequest; };
    TCPBadRequest(QByteArray a);

protected:
    QByteArray getInnerByteMessage() override;
};
*/
#endif // TCPMESSAGE_H
