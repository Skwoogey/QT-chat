#include "chatbackend.h"
#include <QSqlQuery>

ChatBackend::ChatBackend(QString DBFile, QObject *parent) : QObject(parent)
{
    if(!loadFromDB(DBFile))
    {
        valid = false;
        if(DB.open()) DB.close();
    }

    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(1000);
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    timeoutTimer->start();

    tcpServer = new QTcpServer(this);

    connect(tcpServer, &QTcpServer::newConnection, this, &ChatBackend::newConnection);
}

bool ChatBackend::startServerAt(QHostAddress addr, quint16 port)
{
    if(tcpServer->listen(addr, port))
    {
        qDebug() << "Server started";
        return true;
    }
    else
    {
        return false;
    }
}

void ChatBackend::newConnection()
{
    qDebug() << "new user";
    ActiveUser* user = new ActiveUser(tcpServer->nextPendingConnection(), this);

    connect(user, SIGNAL(newTCPMessage(ActiveUser*, const QByteArray&)), this, SLOT(newTCPMessage(ActiveUser*, const QByteArray&)));

    activeUsers.append(user);
}

void ChatBackend::timeout()
{
    for(ActiveUser* user :activeUsers)
    {
        if(user->isTimedOut())
        {
            qDebug() << "user timed out";

            activeUsers.removeOne(user);
        }
    }
}

void ChatBackend::newTCPMessage(ActiveUser *from, const QByteArray &msg)
{
    qDebug() << "message from user";
    qDebug() << msg.toHex();
    typedef TCPMessage::eMessageType eMT;
    from->resetTimeout();


    try {
        eMT msgType;
        msgType = TCPMessage::getMsgType(msg);



        if(msgType != eMT::requestConnection && !from->getUser())
        {
            qDebug() << msgType << !from->getUser();
            TCPBadRequest br;
            br.setReason(TCPBadRequest::eBadRequestReason::userNotLogged);
            from->sendTCPMessage(br.getByteMessage());
            qDebug() << "bad message from not logged user";
            return;
        }

        qDebug() << msgType;

        switch(msgType)
        {
        case eMT::requestConnection:
        {
            if(from->getUser() != nullptr)
            {
                TCPBadRequest br;
                br.setReason(TCPBadRequest::eBadRequestReason::illegalRequest);
                from->sendTCPMessage(br.getByteMessage());
            }

            TCPRequestConnection rc(msg);
            if(!rc.isValid()) throw(-1);



            ChatUser* user = chat->getUserList().findUser(rc.getUsername(), rc.getPassword());

            qDebug() << user << rc.getUsername() << rc.getPassword();

            TCPHandleConnection hc;
            if(!user)
            {
                hc.setStatus(TCPHandleConnection::eConnectionStatus::invalidPasswordOrUsername);
                qDebug() << "bad credentials";
            }
            else if(user->getStatus() == ChatUser::eUserStatus::userOnline)
            {
                hc.setStatus(TCPHandleConnection::eConnectionStatus::userAlreadyOnline);
                qDebug() << "user already online";
            }
            else
            {
                hc.setStatus(TCPHandleConnection::eConnectionStatus::connectionAccepted);
                hc.setUserId(user->getID());
            }

            from->sendTCPMessage(hc.getByteMessage());

            if(hc.getStatus() == TCPHandleConnection::eConnectionStatus::connectionAccepted)
            {
                qDebug() << "user accpeted";
                from->setUser(user);

                user->setStatus(ChatUser::eUserStatus::userOnline);
                TCPUpdateUserStatus uss;
                uss.setUser(user);
                sendToAllBut(uss.getByteMessage(), from);
            }
            else
            {
                qDebug() << "connection failed, closing";
                from->disconnect();
                activeUsers.removeOne(from);
            }
            break;
        }
        case eMT::requestDisconnect:
        {


            ChatUser* user = from->getUser();
            user->setStatus(ChatUser::eUserStatus::userOffline);
            TCPUpdateUserStatus uss;
            uss.setUser(user);
            sendToAllBut(uss.getByteMessage(), from);

            from->disconnect();
            activeUsers.removeOne(from);

            break;
        }
        case eMT::requestMetaData:
        {
            TCPMetaData md;
            md.setMetaData(chat);

            from->sendTCPMessage(md.getByteMessage());
            break;
        }
        case eMT::keepAlive:
        {
            TCPKeepAlive ka;
            if(!ka.isValid()) throw(-1);

            from->sendTCPMessage(ka.getByteMessage());
            break;
        }
        case eMT::sendTextMessage:
        {
            TCPSendTextMessage stm(msg);
            if(!stm.isValid()) throw(-1);

            ChatUser* user = from->getUser();
            ChatRoom* room = dynamic_cast<ChatRoom*>(ChatUnit::getUnitByID(stm.getRoomID()));
            ChatMessage* cm = new ChatMessage(room);
            cm->setText(stm.getText());
            cm->setAuthor(user);

            cm->print();
            room->addMessage(cm);

            QSqlQuery qry;

            qry.prepare("INSERT INTO messages "
                        "VALUES (:id, :aid, :rid, :ts, :txt);");
            qry.bindValue(":id", cm->getID());
            qry.bindValue(":aid", cm->getAuthor().getID());
            qry.bindValue(":rid", room ->getID());
            qry.bindValue(":ts", cm->getTimeStamp());
            qry.bindValue(":txt", cm->getText());


            if(!qry.exec())
            {
                qDebug() << qry.lastQuery();
                qDebug() << "Querry failed" << qry.lastError().text();
            }
            updateIDCounter();

            TCPNewTextMessage ntm;
            ntm.setMessage(cm);

            sendToAllLogged(ntm.getByteMessage());
            break;
        }
        case eMT::getChatHistory:
        {
            TCPRequestChatHistory rch(msg);
            if(!rch.isValid()) throw(-1);

            ChatRoom* room = dynamic_cast<ChatRoom*>(ChatUnit::getUnitByID(rch.getRoomID()));

            TCPChatHistory ch;
            ch.setRoom(room);

            from->sendTCPMessage(ch.getByteMessage());
            break;
        }
        // illegal messages from client
        case eMT::handleConnection:
        case eMT::MetaData:
        case eMT::updateUserStatus:
        case eMT::newTextMessage:
        case eMT::chatHistory:
        case eMT::badRequest:
        default:
            TCPBadRequest br;
            br.setReason(TCPBadRequest::eBadRequestReason::illegalRequest);

            from->sendTCPMessage(br.getByteMessage());
        }
    } catch(int expected_size) {
        qDebug() << "bad size";
        TCPBadRequest br;
        br.setReason(TCPBadRequest::eBadRequestReason::failedToParseMessage);



        from->sendTCPMessage(br.getByteMessage());
    }
    qDebug() << "message from  user handled";
}

bool ChatBackend::loadFromDB(QString DBFile)
{
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(DBFile);

    if(!DB.open())
    {
        qDebug() << "Failed to open database";
        return false;
    }
    QSqlQuery qry;

    // build chat
    qDebug() << "building chat..";
    if(qry.exec("SELECT * FROM chat;"))
    {
        if(qry.size() != 1 && qry.size() != -1)
        {
            qDebug() << "There should be only 1 chat in the chat table" << qry.size();
            return false;
        }
        qry.next();
        chat = new ChatServer(this, false);
        if(!chat->fromSQLQuery(qry))
            return false;
    }
    else
    {
        qDebug() << "failed to exec chat query";
        return false;
    }

    // add users
    qDebug() << "building users..";
    if(qry.exec("SELECT * FROM users;"))
    {
        while(qry.next())
        {
            ChatUser* usr = new ChatUser(nullptr, false);
            if(!usr->fromSQLQuery(qry))
                return false;
        }
    }
    else
    {
        qDebug() << "failed to exec users query";
        return false;
    }

    // add tree
    qDebug() << "building node tree..";
    if(qry.exec("SELECT * FROM chat_tree;"))
    {
        while(qry.next())
        {
            ChatNode::eNodeType nodeType = (ChatNode::eNodeType)qry.value(3).toInt();

            if(nodeType == ChatNode::eNodeType::Branch)
            {
                ChatBranch *br = new ChatBranch(nullptr, false);

                if(!br->fromSQLQuery(qry))
                    return false;
            }
            else if (nodeType == ChatNode::eNodeType::Room)
            {
                ChatRoom *br = new ChatRoom(nullptr, false);

                if(!br->fromSQLQuery(qry))
                    return false;
            }
            else
            {
                qDebug() << "bad node type";
                return false;
            }
        }
    }
    else
    {
        qDebug() << "failed to exec chat_tree query";
        exit(1);
    }

    // add messages
    qDebug() << "building messages..";
    if(qry.exec("SELECT * FROM messages;"))
    {
        while(qry.next())
        {
            qDebug() << "got query";
            ChatMessage* msg = new ChatMessage(nullptr, false);

            if(!msg->fromSQLQuery(qry))
            {
                qDebug() << "failed To build message";
                return false;
            }
        }
    }
    else
    {
        qDebug() << "failed to exec messages query";
        return false;
    }

    if(qry.exec("SELECT * FROM vars;"))
    {
        while(qry.next())
        {
            if(qry.value(0).toString() == "nextID")
                ChatUnit::setNextID(qry.value(1).toUInt());
        }
    }
    else
    {
        qDebug() << "failed to exec vars query";
        return false;
    }

    chat->print();

    return true;
}

bool ChatBackend::updateIDCounter()
{
    QSqlQuery qry;
    qry.prepare("UPDATE vars "
                "SET val=:nVal WHERE name='nextID';");
    qry.bindValue(":nVal", ChatUnit::getNextID());


    if(!qry.exec())
    {
        qDebug() << qry.lastQuery();
        qDebug() << "Querry failed" << qry.lastError().text();
        return false;
    }
    return true;
}

void ChatBackend::sendToAllLogged(const QByteArray &msg)
{
    for(ActiveUser* user :activeUsers)
    {
        if(user->getUser())
        {
            user->sendTCPMessage(msg);
        }
    }
}

void ChatBackend::sendToAllBut(const QByteArray &msg, ActiveUser *sender)
{
    for(ActiveUser* user :activeUsers)
    {
        if(user->getUser() && user != sender)
        {
            user->sendTCPMessage(msg);
        }
    }
}
