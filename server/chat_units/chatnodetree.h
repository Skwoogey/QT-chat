#ifndef CHATNODETREE_H
#define CHATNODETREE_H

#include <QObject>
#include <QtCore>
#include "chatunit.h"
#include "chatbranch.h"
#include "chatroom.h"

class ChatServer;
class ChatNode;
class ChatNodeTree : public ChatUnit
{
    Q_OBJECT
public:
    ChatNodeTree(QObject *parent = nullptr, bool createID = true);
    bool addRootNode(ChatNode* node);
    const QList<ChatNode*>&  getRootNodes() const { return  rootNodes; }
    bool fromSQLQuery(const QSqlQuery &qry) override;

protected:
    void innerPrint(QString prefix) override;
    QByteArray innerGetByteBlock() override;
    bool innerFromByteBlock(const QByteArray &a, int& ptr) override;


private:
    QList<ChatNode*> rootNodes;
};

#endif // CHATNODETREE_H
