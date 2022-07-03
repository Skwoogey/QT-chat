#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "chat_units/chatmessage.h"

class ChatMessageWidget
{
public:
    ChatMessageWidget(ChatMessage* nmsg);
    static void enableRoom(ChatRoom* room);
    static void clear();

    QFrame* getWidget() { return widget; }

private:
    QFrame* makeNewMessage(ChatMessage* msg);
    void makeVisibleMsgsOfRoom(ChatRoom* room);

    QFrame* widget;
    ChatMessage* msg;

    static QList<ChatMessageWidget*> allMessages;
};

#endif // CHATMESSAGEWIDGET_H
