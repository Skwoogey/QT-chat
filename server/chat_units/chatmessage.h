#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QObject>
#include <QtCore>
#include "chatroom.h"
#include "chatunit.h"
#include "chatuser.h"

class ChatRoom;
class ChatMessage : public ChatUnit
{
    Q_OBJECT

public:
    explicit ChatMessage(QObject *parent = nullptr, bool createID = true);

    bool fromSQLQuery(const QSqlQuery &qry) override;

    void setTimeStamp(QDateTime ts) { timeStamp = ts; }
    const QDateTime& getTimeStamp() { return timeStamp; }

    void setAuthor(ChatUser* cu) { author = cu; }
    const ChatUser& getAuthor() { return *author; }

    const ChatRoom& getRoom() { return *room; }
    //void setRoom(ChatRoom* rm) { room = rm; }

    void setText(QString msgText) { text = msgText; }
    const QString& getText() { return text; }

    //bool updateDB(const QSqlDatabase& DB);

protected:
    QDateTime timeStamp;
    ChatUser* author;
    ChatRoom* room;
    QString text;

    QByteArray innerGetByteBlock() override;
    void innerPrint(QString prefix) override;
    bool innerFromByteBlock(const QByteArray &a, int &ptr) override;

signals:

};

#endif // CHATMESSAGE_H
