#include <QCoreApplication>
#include <QtCore>
#include "tcp_messages/chat_tcp_messages.h"
#include "chat_units/chat.h"
#include "backend/chatbackend.h"

void checkAllTCPandChat()
{
    TCPMessage* msg;

    // tcp request connection

    TCPRequestConnection rc_msg;

    rc_msg.setUsername("user");
    rc_msg.setPassword("passwd");

    msg = &rc_msg;

    msg->print();
    auto msgBytes = msg->getByteMessage();
    //msgBytes[4] = 30;

    qDebug() << TCPMessage::getMsgType(msgBytes) << msgBytes.length();
    TCPRequestConnection rc_msg2(msgBytes);

    msg = &rc_msg2;
    qDebug() << msg->isValid();


    // tcp handle connection

    TCPHandleConnection hc_msg;

    hc_msg.setStatus(TCPHandleConnection::eConnectionStatus::invalidPasswordOrUsername);
    hc_msg.setStatus(TCPHandleConnection::eConnectionStatus::userAlreadyOnline);
    hc_msg.setStatus(TCPHandleConnection::eConnectionStatus::connectionAccepted);

    msg = &hc_msg;

    msg->print();
    msgBytes = msg->getByteMessage();

    qDebug() << TCPMessage::getMsgType(msgBytes) << msgBytes.length();
    TCPHandleConnection hc_msg2(msgBytes);

    msg = &hc_msg2;
    qDebug() << msg->isValid();

    msg->print();


    // tcp request metadata

    TCPRequestMetaData rmd_msg;

    msg = &rmd_msg;

    msg->print();
    msgBytes = msg->getByteMessage();

    TCPRequestMetaData rmd_msg2(msgBytes);

    msg = &rmd_msg2;
    qDebug() << msg->isValid();

    msg->print();


    //tcpMetaData
    ChatServer MD;

    ChatNodeTree tree(&MD);

    ChatBranch* branch = new ChatBranch(&tree);
    branch ->setNodeName("branch1");

    ChatBranch* branch2 = new ChatBranch(branch);
    branch2 ->setNodeName("branch11");

    branch2 = new ChatBranch(branch);
    branch2 ->setNodeName("branch12");

    ChatRoom* room = new ChatRoom(branch2);
    room->setNodeName("room121");

    room = new ChatRoom(branch2);
    room->setNodeName("room122");

    room = new ChatRoom(branch);
    room->setNodeName("room11");

    room = new ChatRoom(&tree);
    room->setNodeName("room1");

    ChatUserList ul(&MD);

    ChatUser* usr;
    QString sampleName = "Bob";
    for(int i = 0; i < 5; i++)
    {
        usr = new ChatUser(&ul);
        usr->setUserName(sampleName + QString::number(i));
        usr->setStatus(ChatUser::eUserStatus::userOnline);
    }

    //MD.print("\t");

    TCPMetaData tcpMD;
    tcpMD.setMetaData(&MD);

    auto MDBB = tcpMD.getByteMessage();

    TCPMetaData tcpMD2(MDBB);
    //tcpMD2.print();


    // tcp keep alive

    TCPKeepAlive ka_msg;

    msg = &ka_msg;

    msg->print();
    msgBytes = msg->getByteMessage();

    TCPKeepAlive ka_msg2(msgBytes);

    msg = &ka_msg2;
    qDebug() << msg->isValid();

    msg->print();


    ChatMessage* cm;
    QString sampleText = "Hello";
    for(int i = 0; i < 10; i++)
    {
        cm = new ChatMessage(room);
        cm->setText(sampleText + QString::number(i));
        cm->setAuthor(usr);
    }

    room->printChatHistory();

    auto RBB = room->getChatHistory();

    int ptr = 0;
    room->fromChatHistory(RBB, ptr);

    room->printChatHistory();


    msg = new TCPSendTextMessage;

    static_cast<TCPSendTextMessage*>(msg)->setRoomID(room->getID());
    static_cast<TCPSendTextMessage*>(msg)->setText("Hello");

    msg->print();

    msg = new TCPSendTextMessage(msg->getByteMessage());

    msg->print();

    msg = new TCPNewTextMessage;

    static_cast<TCPNewTextMessage*>(msg)->setMessage(cm);

    msg->print();

    msg = new TCPNewTextMessage(msg->getByteMessage());

    msg->print();

    msg = new TCPRequestChatHistory;

    static_cast<TCPRequestChatHistory*>(msg)->setRoomID(room->getID());

    msg->print();

    msg = new TCPRequestChatHistory(msg->getByteMessage());

    msg->print();

    msg = new TCPChatHistory;

    static_cast<TCPChatHistory*>(msg)->setRoom(room);

    msg->print();

    msg = new TCPChatHistory(msg->getByteMessage());

    msg->print();

    msg = new TCPUpdateUserStatus;

    static_cast<TCPUpdateUserStatus*>(msg)->setUser(usr);

    msg->print();

    msg = new TCPUpdateUserStatus(msg->getByteMessage());

    msg->print();

    msg = new TCPRequestDisconnect;

    msg->print();

    msg = new TCPRequestDisconnect(msg->getByteMessage());

    msg->print();

    msg = new TCPBadRequest;

    static_cast<TCPBadRequest*>(msg)->setReason(TCPBadRequest::eBadRequestReason::failedToParseMessage);

    msg->print();

    msg = new TCPBadRequest(msg->getByteMessage());

    msg->print();

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //checkAllTCPandChat();

    ChatBackend backend("chat.db");
    if(!backend.isValid() ||
       !backend.startServerAt(QHostAddress::LocalHost, 1234))
    return 1;



    return a.exec();
}
