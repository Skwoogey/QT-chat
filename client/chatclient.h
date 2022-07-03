#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QtCore>
#include "chat_units/chat.h"
#include "tcp_messages/chat_tcp_messages.h"
#include "chatmessagewidget.h"

class ChatClient : public QObject
{
    Q_OBJECT
public:

    enum eConnectionStatus
    {
        userDisconnected,
        userConnecting,
        userConnected
    };

    explicit ChatClient(QObject *parent = nullptr);

    void resetTimeout() { beforeTimeout = 10; }

    void sendTCPMessage(const QByteArray& msg);

    void setUser(ChatUser* usr) { user = usr; }
    ChatUser* getUser() { return user; }

    ChatServer* getChat() { return chat; }

    void disconnect();
    void connectToServer(const QString& host, quint16 port, const QString& un, const QString& pass);
    void sendMessage(const QString& text, ChatRoom* room);
    void requestChatHistory(ChatRoom* room);

signals:
    void userConnectionStatusChanged(ChatClient::eConnectionStatus newStatus);
    void updateUserStatus(ChatUser* usr);
    void newMessage(ChatMessage* msg);
    void newChatHistory(ChatRoom* room);
    void newMetaData();
    void requestConnectionRecieved(TCPHandleConnection::eConnectionStatus);
    void socketError(QAbstractSocket::SocketError socketError);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void error(QAbstractSocket::SocketError e);

    void timeout();

private:
    QTcpSocket* socket;
    QTimer* timeoutTimer;

    ChatUser* user = nullptr;
    quint32 user_id;
    ChatServer* chat = nullptr;
    int beforeTimeout = 10;
    eConnectionStatus status = eConnectionStatus::userDisconnected;
    bool hasPendingMessage = false;
    bool disconnectSilently = false;

    QString username;
    QString password;


};

#endif // CHATCLIENT_H
