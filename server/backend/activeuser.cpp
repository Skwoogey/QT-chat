#include "activeuser.h"

ActiveUser::ActiveUser(QTcpSocket* skt, QObject *parent) : QObject(parent)
{
    socket = skt;

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

bool ActiveUser::isTimedOut()
{
    if(--timeout == 0)
    {
        disconnect();

        return true;
    }
    return false;
}

void ActiveUser::resetTimeout()
{
    timeout = 10;
}

void ActiveUser::sendTCPMessage(const QByteArray &msg)
{
    socket->write(msg);
    hasPendingMessage = true;
}

void ActiveUser::disconnect()
{
    socket->flush();
    socket->close();

    if(user)
    {
        user->setStatus(ChatUser::eUserStatus::userOffline);
        qDebug() << "user set offline";
    }
    deleteLater();
}

void ActiveUser::connected()
{
    qDebug() << "What?...";
}

void ActiveUser::disconnected()
{
    deleteLater();
}

void ActiveUser::bytesWritten(qint64 bytes)
{
    hasPendingMessage = false;
}

void ActiveUser::readyRead()
{
    lastMsg = socket->readAll();

    emit newTCPMessage(this, lastMsg);
}

void ActiveUser::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "active user socket error";
    disconnect();
}
