#ifndef CHATBACKEND_H
#define CHATBACKEND_H

#include <QObject>
#include <QtCore>
#include <QtSql>
#include "chat_units/chatserver.h";
#include "activeuser.h"

class ChatBackend : public QObject
{
    Q_OBJECT

public:
    explicit ChatBackend(QString DBFile, QObject *parent = nullptr);
    bool isValid() { return valid; }
    bool startServerAt(QHostAddress addr, quint16 port);

signals:

public slots:
    void newConnection();
    void timeout();
    void newTCPMessage(ActiveUser* from, const QByteArray& msg);

private:
    QSqlDatabase DB;
    ChatServer* chat;

    QTimer* timeoutTimer;
    QTcpServer* tcpServer;
    QList<ActiveUser*> activeUsers;
    bool valid;


    bool loadFromDB(QString DBFile);
    bool updateIDCounter();
    void sendToAllLogged(const QByteArray& msg);
    void sendToAllBut(const QByteArray& msg, ActiveUser* sender);

};

#endif // CHATBACKEND_H
