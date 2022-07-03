#ifndef CHATBRANCH_H
#define CHATBRANCH_H

#include <QObject>
#include <QtCore>
#include "chatunit.h"
#include "chatnode.h"
#include "chatroom.h"

class ChatBranch : public ChatNode
{
    Q_OBJECT
public:
    ChatBranch(QObject* parent = nullptr, bool createID = true);

    bool addChildNode(ChatNode* node);
    const QList<ChatNode*>& getChildNodes() { return childNodes; }

protected:
    QByteArray getNodeSpecifcByteBlock() override;
    bool fromNodeSpecificByteBlock(const QByteArray &a, int &ptr) override;
    void innerPrint(QString prefix) override;

private:
    QList<ChatNode*> childNodes;


signals:

};

#endif // CHATBRANCH_H
