#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "chatclient.h"
#include "chat_units/chat.h"
#include "chattreewidgetitem.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void userConnectionStatusChanged(ChatClient::eConnectionStatus newStatus);
    void updateUserStatus(ChatUser* usr);
    void newMessage(ChatMessage* msg);
    void newChatHistory(ChatRoom* room);
    void newMetaData();
    void requestConnectionRecieved(TCPHandleConnection::eConnectionStatus);
    void socketError(QAbstractSocket::SocketError socketError);

    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

    void on_chatTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_sendButton_clicked();

    void on_textEdit_textChanged();

private:
    Ui::Dialog *ui;
    ChatClient* client;
    QMap<ChatRoom*, QTreeWidgetItem*> rooms;
    QMap<ChatUser*, QListWidgetItem*> users;
    QStatusBar* statusBar;
    ChatRoom* currentVisibleChat = nullptr;

};
#endif // DIALOG_H
