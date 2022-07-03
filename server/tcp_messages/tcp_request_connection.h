#ifndef TCP_REQUEST_CONNECTION_H
#define TCP_REQUEST_CONNECTION_H

#include <QtCore>
#include <assert.h>
#include "tcpmessage.h"
#include "utils.h"

class TCPRequestConnection : public TCPMessage
{
public:
    TCPRequestConnection() : TCPMessage(eMessageType::requestConnection) {};
    TCPRequestConnection(const QByteArray &a);

    bool setUsername(const QString &name);
    const QString& getUsername() { return username; }

    bool setPassword(const QString &passwd);
    const QString& getPassword() { return password; }

    void print() override;

protected:
    QByteArray getInnerByteMessage() override;

private:
    QString username;
    QString password;
};

#endif // TCP_REQUEST_CONNECTION_H
