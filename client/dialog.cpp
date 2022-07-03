#include "dialog.h"
#include "ui_dialog.h"

#define BROWN QColor(153, 51, 0)

QColor getUserStatusColor(const ChatUser* user)
{
    if(user->getStatus() == ChatUser::eUserStatus::userOnline)
        return Qt::darkGreen;
    return Qt::red;
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->treeChatSplitter->setSizes(QList<int>() << 100 << 500);
    ui->chatUsersSplitter->setSizes(QList<int>() << 500 << 100);
    ui->chatEditSplitter->setSizes(QList<int>() << 700 << 200);

    client = new ChatClient(this);
    connect(client, SIGNAL(userConnectionStatusChanged(ChatClient::eConnectionStatus)),
            this, SLOT(userConnectionStatusChanged(ChatClient::eConnectionStatus)));
    connect(client, SIGNAL(socketError(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(client, SIGNAL(requestConnectionRecieved(TCPHandleConnection::eConnectionStatus)),
            this, SLOT(requestConnectionRecieved(TCPHandleConnection::eConnectionStatus)));
    connect(client, SIGNAL(newMetaData()),
            this, SLOT(newMetaData()));
    connect(client, SIGNAL(updateUserStatus(ChatUser*)),
            this, SLOT(updateUserStatus(ChatUser*)));
    connect(client, SIGNAL(newMessage(ChatMessage*)),
            this, SLOT(newMessage(ChatMessage*)));
    connect(client, SIGNAL(newChatHistory(ChatRoom*)),
            this, SLOT(newChatHistory(ChatRoom*)));

    statusBar = new QStatusBar(this);
    ui->statusBarLayout->addWidget(statusBar);

    ui->portLineEdit->setValidator(new QIntValidator(this));

    /*
    ui->hostLineEdit->setText("127.0.0.1");
    ui->portLineEdit->setText("1234");
    ui->userLineEdit->setText("user1");
    ui->passLineEdit->setText("pass1");
    */
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::userConnectionStatusChanged(ChatClient::eConnectionStatus newStatus)
{
    typedef ChatClient::eConnectionStatus CS;
    switch(newStatus)
    {
    case CS::userConnecting:
        ui->connectButton->setEnabled(false);
        ui->hostLineEdit->setEnabled(false);
        ui->portLineEdit->setEnabled(false);
        ui->userLineEdit->setEnabled(false);
        ui->passLineEdit->setEnabled(false);

        statusBar->showMessage("Connecting...");
        break;
    case CS::userConnected:
        ui->disconnectButton->setEnabled(true);

        statusBar->showMessage("Connected", 2000);
        break;
    case CS::userDisconnected:
        ui->disconnectButton->setEnabled(false);

        ui->connectButton->setEnabled(true);
        ui->hostLineEdit->setEnabled(true);
        ui->portLineEdit->setEnabled(true);
        ui->userLineEdit->setEnabled(true);
        ui->passLineEdit->setEnabled(true);

        statusBar->showMessage("Disconnected", 2000);
        break;
    default:
        qDebug() << "invalid connection status";
    }
}

void Dialog::updateUserStatus(ChatUser *usr)
{
    qDebug() << "updateUserStatus()";
    users[usr]->setTextColor(getUserStatusColor(usr));
}

void Dialog::newMessage(ChatMessage *msg)
{
    auto msgW = new ChatMessageWidget(msg);
    ui->chatLayout->insertWidget(ui->chatLayout->count() - 1, msgW->getWidget(), 0, Qt::AlignTop);
    msgW->getWidget()->setHidden(&msg->getRoom() != currentVisibleChat);

    if(&msg->getAuthor() == client->getUser())
    {
        ui->sendButton->setEnabled(true);
    }
}

void Dialog::newChatHistory(ChatRoom *room)
{
    auto msgs = room->getMessages();

    for(auto msg : msgs)
    {
        auto msgW = new ChatMessageWidget(msg);
        ui->chatLayout->insertWidget(ui->chatLayout->count() - 1, msgW->getWidget(), 0, Qt::AlignTop);
    }

    static_cast<ChatTreeWidgetItem*>(rooms[room])->setPopulated(true);
    qDebug() << "room populated";
    ChatMessageWidget::enableRoom(currentVisibleChat);
}

void Dialog::newMetaData()
{
    ChatServer* chat = client->getChat();

    const ChatUserList& ul = chat->getUserList();

    for( ChatUser* user : ul.getUsers())
    {
        QListWidgetItem* itm = new QListWidgetItem(ui->userList);
        itm->setText(user->getUserName());
        itm->setTextColor(getUserStatusColor(user));

        users[user] = itm;
    }

    const ChatNodeTree& tree = chat->getNodeTree();

    std::function<void(ChatTreeWidgetItem*, const QList<ChatNode*>&)> parseBranch;
    parseBranch = [&parseBranch, this](ChatTreeWidgetItem* parent, const QList<ChatNode*>& branch) {
        for( ChatNode* node : branch)
        {
            ChatTreeWidgetItem* itm = new ChatTreeWidgetItem(parent);
            itm->setText(0, node->getNodeName());
            itm->setNode(node);

            if(node->getNodeType() == ChatNode::eNodeType::Branch)
            {
                itm->setTextColor(0, BROWN);
                auto newBranch = static_cast<ChatBranch*>(node)->getChildNodes();
                parseBranch(itm, newBranch);
            }
            else
            {
                itm->setTextColor(0, QColor(0, 153, 0));
                rooms[static_cast<ChatRoom*>(node)] = itm;
            }
        }
    };

    for( ChatNode* node : tree.getRootNodes())
    {
        ChatTreeWidgetItem* itm = new ChatTreeWidgetItem(ui->chatTreeWidget);
        itm->setText(0, node->getNodeName());
        itm->setNode(node);

        if(node->getNodeType() == ChatNode::eNodeType::Branch)
        {
            itm->setTextColor(0, QColor(153, 51, 0));
            auto newBranch = static_cast<ChatBranch*>(node)->getChildNodes();
            parseBranch(itm, newBranch);
        }
        else
        {
            itm->setTextColor(0, QColor(0, 153, 0));
            rooms[static_cast<ChatRoom*>(node)] = itm;
        }
    }
}

void Dialog::requestConnectionRecieved(TCPHandleConnection::eConnectionStatus status)
{
    typedef TCPHandleConnection::eConnectionStatus eCS;
    qDebug() << "Dialog requestConnectionRecieved";
    switch(status)
    {
    case eCS::userAlreadyOnline:
        ui->connectButton->setEnabled(true);
        ui->hostLineEdit->setEnabled(true);
        ui->portLineEdit->setEnabled(true);
        ui->userLineEdit->setEnabled(true);
        ui->passLineEdit->setEnabled(true);
        ui->disconnectButton->setEnabled(false);

        statusBar->showMessage("User already online");
        break;
    case eCS::connectionAccepted:
        ui->disconnectButton->setEnabled(true);

        statusBar->showMessage("Connected", 2000);
        break;
    case eCS::invalidPasswordOrUsername:
        ui->disconnectButton->setEnabled(false);

        ui->connectButton->setEnabled(true);
        ui->hostLineEdit->setEnabled(true);
        ui->portLineEdit->setEnabled(true);
        ui->userLineEdit->setEnabled(true);
        ui->passLineEdit->setEnabled(true);

        statusBar->showMessage("Invalid credentials", 2000);
        break;
    default:
        qDebug() << "invalid connection status";
    }
}

void Dialog::socketError(QAbstractSocket::SocketError socketError)
{
    if(socketError == QAbstractSocket::SocketError::RemoteHostClosedError &&
       client->getUser() == nullptr)
        return;
    qDebug() << "Dialog socket error";

    ui->disconnectButton->setEnabled(false);

    ui->connectButton->setEnabled(true);
    ui->hostLineEdit->setEnabled(true);
    ui->portLineEdit->setEnabled(true);
    ui->userLineEdit->setEnabled(true);
    ui->passLineEdit->setEnabled(true);

    statusBar->showMessage("Socket error", 2000);
}

void Dialog::on_connectButton_clicked()
{
    if(ui->hostLineEdit->text().length() == 0 ||
       ui->portLineEdit->text().length() == 0 ||
       ui->userLineEdit->text().length() == 0 ||
       ui->passLineEdit->text().length() == 0)
    {
        statusBar->showMessage("Please fill in all of the fields", 2000);
        return;
    }

    client->connectToServer(
        ui->hostLineEdit->text(),
        ui->portLineEdit->text().toInt(),
        ui->userLineEdit->text(),
        ui->passLineEdit->text()
    );
}

void Dialog::on_disconnectButton_clicked()
{
    client->disconnect();

    ui->chatTreeWidget->clear();
    ui->userList->clear();

    ChatMessageWidget::clear();

    currentVisibleChat = nullptr;
}

void Dialog::on_chatTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->textColor(0) == BROWN)
        return;

    auto rmWidget = static_cast<ChatTreeWidgetItem*>(item);
    ChatRoom* room = static_cast<ChatRoom*>(rmWidget->getNode());
    currentVisibleChat = room;
    ChatMessageWidget::enableRoom(currentVisibleChat);
    if(rmWidget->isPopulated())
        return;

    client->requestChatHistory(room);


}

void Dialog::on_sendButton_clicked()
{
    if(!currentVisibleChat)
        return;
    if(ui->textEdit->toPlainText().length() == 0)
        return;

    client->sendMessage(ui->textEdit->toPlainText(), currentVisibleChat);

    ui->textEdit->clear();
    ui->sendButton->setEnabled(false);

}

void Dialog::on_textEdit_textChanged()
{
    if(ui->textEdit->toPlainText().contains('\n'))
    {
        auto text = ui->textEdit->toPlainText().remove('\n');
        ui->textEdit->setText(text);

        ui->sendButton->click();
    }
}
