#include "chatclient.h"

ChatClient::ChatClient(QObject *parent) : QObject(parent)
{
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(1000);
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    timeoutTimer->start();

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

}

void ChatClient::sendTCPMessage(const QByteArray &msg)
{
    socket->write(msg);

    hasPendingMessage = true;
}

void ChatClient::disconnect()
{
    //request disconnect
    TCPRequestDisconnect rd;

    sendTCPMessage(rd.getByteMessage());

    socket->disconnectFromHost();

    user = nullptr;
}

void ChatClient::connectToServer(const QString &host, quint16 port, const QString& un, const QString& pass)
{
    username = un;
    password = pass;

    qDebug() << "socket connecting";
    socket->connectToHost(host, port);

    status = eConnectionStatus::userConnecting;
    emit userConnectionStatusChanged(status);
}

void ChatClient::sendMessage(const QString &text, ChatRoom *room)
{
    //send message

    TCPSendTextMessage stm;
    stm.setRoomID(room->getID());
    stm.setText(text);

    sendTCPMessage(stm.getByteMessage());
}

void ChatClient::requestChatHistory(ChatRoom *room)
{
    //request chat history

    TCPRequestChatHistory rch;
    rch.setRoomID(room->getID());

    sendTCPMessage(rch.getByteMessage());
}

void ChatClient::connected()
{
    status = eConnectionStatus::userConnected;

    qDebug() << "socket connected";

    emit userConnectionStatusChanged(status);
    // request connection

    TCPRequestConnection rc;
    rc.setUsername(username);
    rc.setPassword(password);

    qDebug() << rc.getByteMessage().toHex();
    sendTCPMessage(rc.getByteMessage());
}

void ChatClient::disconnected()
{
    status = eConnectionStatus::userDisconnected;

    if(!disconnectSilently)
        emit userConnectionStatusChanged(status);
    else
        disconnectSilently = false;
}

void ChatClient::bytesWritten(qint64 bytes)
{
    hasPendingMessage = false;
}

void ChatClient::readyRead()
{
    qDebug() << "message from server";
    typedef TCPMessage::eMessageType eMT;
    //resetTimeout();

    QByteArray msg = socket->readAll();


    try {
        int ptr = 0;
        while(msg.size() != ptr)
        {
            QByteArray aref = msg.right(msg.size() - ptr);
            eMT msgType;
            msgType = TCPMessage::getMsgType(aref);
            qDebug() << aref.toHex(' ');

            qDebug() << msgType;

            switch(msgType)
            {
            case eMT::handleConnection:
            {
                TCPHandleConnection hc(aref);
                hc.print();
                ptr += hc.getSize();

                emit requestConnectionRecieved(hc.getStatus());



                if(hc.getStatus() == TCPHandleConnection::eConnectionStatus::connectionAccepted)
                {
                    qDebug() << "connected";
                    status = eConnectionStatus::userConnected;
                    user_id = hc.getUserId();
                    //request metadata
                    TCPRequestMetaData rmd;

                    sendTCPMessage(rmd.getByteMessage());
                }
                else
                {
                    status = eConnectionStatus::userDisconnected;
                }

                break;
            }
            case eMT::MetaData:
            {
                ChatUnit::clearAll();

                TCPMetaData md(aref);
                ptr += md.getSize();

                md.print();

                chat = md.getMetaData();
                chat->setParent(this);

                user = dynamic_cast<ChatUser*>(ChatUnit::getUnitByID(user_id));

                emit newMetaData();

                break;
            }
            case eMT::updateUserStatus:
            {
                TCPUpdateUserStatus uus(aref);
                ptr += uus.getSize();

                emit updateUserStatus(uus.getUser());

                break;
            }
            case eMT::newTextMessage:
            {
                TCPNewTextMessage ntm(aref);
                ptr += ntm.getSize();

                emit newMessage(ntm.getMessage());

                break;
            }
            case eMT::chatHistory:
            {
                qDebug() << "got chat history";
                TCPChatHistory ch(aref);
                ptr += ch.getSize();

                emit newChatHistory(ch.getRoom());

                break;
            }
            case eMT::badRequest:
            {
                TCPBadRequest br(aref);
                ptr += br.getSize();

                if(br.getReason() == TCPBadRequest::eBadRequestReason::userNotLogged);
                {
                    status = eConnectionStatus::userDisconnected;
                    emit userConnectionStatusChanged(status);
                    break;
                }

                //else try to ignore, cuz shouldn't happen
                break;
            }
            case eMT::keepAlive:
            {
                TCPKeepAlive ka(aref);
                ptr += ka.getSize();
                resetTimeout();
                break;
            }
            // illegal messages from server
            case eMT::requestConnection:
            case eMT::requestDisconnect:
            case eMT::requestMetaData:
            case eMT::sendTextMessage:
            case eMT::getChatHistory:
            default:
                // shouldn't happen
                qDebug() << "got illegal message";
                break;
            }
        }
    } catch(int expected_size) {
        qDebug() << "bad size";
        TCPBadRequest br;
        br.setReason(TCPBadRequest::eBadRequestReason::failedToParseMessage);
    }
}

void ChatClient::error(QAbstractSocket::SocketError e)
{
    qDebug() << "socket failed";

    qDebug() << e;

    disconnectSilently = true;

    status = eConnectionStatus::userDisconnected;

    socket->flush();
    socket->close();
    emit socketError(e);
}

void ChatClient::timeout()
{
    if(status != eConnectionStatus::userConnected)
        return;
    qDebug() << "KeepAlive";

    TCPKeepAlive ka;

    if(--beforeTimeout == 0)
    {
        disconnect();
        return;
    }

    sendTCPMessage(ka.getByteMessage());
}
