#ifndef CHATROOM_H
#define CHATROOM_H

#include <QObject>
#include <QtCore>
#include "chatunit.h"
#include "chatnode.h"
#include "chatmessage.h"

class ChatMessage;
class ChatRoom : public ChatNode
{
    Q_OBJECT

public:
    explicit ChatRoom(QObject* parent = nullptr, bool createID = true);

    bool addMessage(ChatMessage *cm);
    const QList<ChatMessage*>& getMessages() {return messages; }

    QByteArray getChatHistory();
    bool fromChatHistory(const QByteArray &a, int& msgPtr);
    void printChatHistory();

    static ChatRoom* dispatchChatHistory(const QByteArray &a, int& msgPtr);

protected:
    QByteArray getNodeSpecifcByteBlock() override;
    bool fromNodeSpecificByteBlock(const QByteArray &a, int &ptr) override;
    void innerPrint(QString prefix) override;

private:
    QList<ChatMessage*> messages;

signals:

};

#endif // CHATROOM_H
