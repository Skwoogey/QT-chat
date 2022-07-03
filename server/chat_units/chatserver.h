#ifndef CHATMETADATA_H
#define CHATMETADATA_H

#include <QObject>
#include <QtCore>
#include "chatunit.h"
#include "chatnodetree.h"
#include "chatuserlist.h"
#include "utils.h"

class ChatUserList;
class ChatNodeTree;
class ChatServer : public ChatUnit
{
    Q_OBJECT
public:

    explicit ChatServer(QObject *parent = nullptr, bool createID = true);

    bool fromSQLQuery(const QSqlQuery &qry) override;

    void setNodeTree(ChatNodeTree* nTree) { tree = nTree; }
    const ChatNodeTree& getNodeTree() { return *tree; }

    void setUserList(ChatUserList* nList) { userList = nList; }
    const ChatUserList& getUserList() { return *userList; }

protected:
    bool innerFromByteBlock(const QByteArray &a, int &ptr) override;
    QByteArray innerGetByteBlock() override; 
    void innerPrint(QString prefix) override;

    ChatNodeTree* tree;
    ChatUserList* userList;


signals:

};

#endif // CHATMETADATA_H
