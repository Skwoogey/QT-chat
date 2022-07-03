#include "tcp_request_connection.h"

TCPRequestConnection::TCPRequestConnection(const QByteArray &a) : TCPMessage(a, eMessageType::requestConnection)
{
    if(!isValidMessage)
        return;

    if(msgSize < sizeof(msgSize) + 2)
    {
        qDebug() << "ERROR(RequestConnection): Message too small! length: " << a.length()
                 << " msgSize: " << msgSize;

        isValidMessage = false;
        return;
    }

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    quint8 nameLength = *(quint8*)getArrayPtr(a, msgPtr, sizeof(nameLength));
    quint8 passwordLength = *(quint8*)getArrayPtr(a, msgPtr, sizeof(passwordLength));

    if(msgSize != sizeof(msgSize)
            + sizeof(msgType)
            + sizeof(nameLength)
            + sizeof(passwordLength)
            + nameLength
            + passwordLength)
    {
        qDebug() << "ERROR(RequestConnection): Message size doesn't match!"
                 << "\nmsgSize: " << msgSize
                 << "\nnameLength: " << nameLength
                 << "\npasswordLength: " << passwordLength;
        isValidMessage = false;
        return;
    }

    username = QString::fromUtf8(getArrayPtr(a, msgPtr, nameLength), nameLength);
    password = QString::fromUtf8(getArrayPtr(a, msgPtr, passwordLength), passwordLength);
}

bool TCPRequestConnection::setUsername(const QString &name)
{
    if (name.length() > 63 || name.length() == 0)
        return false;

    username = name;
    return true;
}

bool TCPRequestConnection::setPassword(const QString &passwd)
{
    if (passwd.length() > 63 || passwd.length() == 0)
        return false;

    password = passwd;
    return true;
}

void TCPRequestConnection::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;

    qDebug() << "username" << username;
    qDebug() << "password" << password;
}

QByteArray TCPRequestConnection::getInnerByteMessage()
{
    auto nameByteArray = username.toUtf8();
    auto passwordByteArray = password.toUtf8();

    quint8 nameLength = nameByteArray.length();
    quint8 passwordLength = passwordByteArray.length();

    quint16 totalLength = 2;
    totalLength += nameLength;
    totalLength += passwordLength;

    QByteArray message(totalLength, Qt::Initialization::Uninitialized);

    int msgPtr = 0;
    insertIntoArray(message, (char*)&nameLength, 1, msgPtr);
    insertIntoArray(message, (char*)&passwordLength, 1, msgPtr);
    insertIntoArray(message, nameByteArray.data(), nameByteArray.length(), msgPtr);
    insertIntoArray(message, passwordByteArray.data(), passwordByteArray.length(), msgPtr);

    return message;
}
