#ifndef TCPSENDTEXTMESSAGE_H
#define TCPSENDTEXTMESSAGE_H

#include <QObject>
#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPSendTextMessage : public TCPMessage
{
public:
    TCPSendTextMessage() : TCPMessage(eMessageType::sendTextMessage) {};
    TCPSendTextMessage(const QByteArray &a);

    void print() override;

    void setRoomID(quint32 rid) { roomID = rid; }
    quint32 getRoomID() { return roomID; }

    void setText(const QString& nt) { text = nt; }
    QString& getText() { return text; }

protected:
    QByteArray getInnerByteMessage() override;
    quint32 roomID;
    QString text;

};

#endif // TCPSENDTEXTMESSAGE_H
