#ifndef ACTIVEUSER_H
#define ACTIVEUSER_H

#include <QObject>
#include <QtCore>
#include <QSqlDatabase>
#include "chat_units/chat.h"
#include "tcp_messages/chat_tcp_messages.h"

class ActiveUser : public QObject
{
    Q_OBJECT
public:

    explicit ActiveUser(QTcpSocket* skt, QObject *parent = nullptr);

    bool isTimedOut();
    void resetTimeout();

    void sendTCPMessage(const QByteArray& msg);
    void disconnect();

    void setUser(ChatUser* usr) { user = usr; }
    ChatUser* getUser() { return user; }

signals:
    void newTCPMessage(ActiveUser* from, const QByteArray& msg);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void error(QAbstractSocket::SocketError socketError);

private:
    int timeout = 10;
    bool loggedUser = false;
    ChatUser* user = nullptr;
    QTcpSocket* socket;
    bool hasPendingMessage = false;
    QByteArray lastMsg;

};

#endif // ACTIVEUSER_H
