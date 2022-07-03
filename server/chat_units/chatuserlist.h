#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#include <QObject>
#include <QtCore>
#include "chatunit.h"
#include "chatuser.h"
#include "utils.h"
#include "chatserver.h"

class ChatServer;
class ChatUserList : public ChatUnit
{
    Q_OBJECT

public:
    ChatUserList(QObject* parent= nullptr, bool createID = true);
    bool addUser(ChatUser* user);
    bool fromSQLQuery(const QSqlQuery &qry) override;
    ChatUser *findUser(const QString& n, const QString& p) const;
    const QList<ChatUser*>& getUsers() const { return users; }
protected:
    void innerPrint(QString prefix) override;
    bool innerFromByteBlock(const QByteArray &a, int &ptr) override;
    QByteArray innerGetByteBlock() override;

private:
    QList<ChatUser*> users;

};

#endif // CHATUSERLIST_H
