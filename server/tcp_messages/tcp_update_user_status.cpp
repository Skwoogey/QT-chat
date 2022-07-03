#include "tcp_update_user_status.h"

TCPUpdateUserStatus::TCPUpdateUserStatus(const QByteArray &a) : TCPMessage(a, eMessageType::updateUserStatus)
{
    if(!isValidMessage)
        return;

    if(msgSize < sizeof(msgSize) + 8)
    {
        qDebug() << "ERROR(TCPSendTextMessage): Message too small! length: " << a.length()
                 << " msgSize: " << msgSize;

        isValidMessage = false;
        return;
    }

    int msgPtr = sizeof(msgSize) + sizeof(msgType);

    quint32 userID = *getValuePtrFromArray<quint32>(a, msgPtr);
    ChatUser::eUserStatus us = *getValuePtrFromArray<ChatUser::eUserStatus>(a, msgPtr);

    user = dynamic_cast<ChatUser*>(ChatUnit::getUnitByID(userID));

    if(user == nullptr)
    {
        qDebug() << "ERROR(TCPUpdateUserStatus): cannot get user of id!";
        isValidMessage = false;
        return;
    }

    user->setStatus(us);
}

void TCPUpdateUserStatus::print()
{
    qDebug() << "msgSize" << msgSize;
    qDebug() << "msgType" << msgType;
    user->print();
}

QByteArray TCPUpdateUserStatus::getInnerByteMessage()
{
    quint32 user_id =user->getID();
    QByteArray byteBlock((char*)&user_id, sizeof(user_id));

    ChatUser::eUserStatus us = user->getStatus();
    byteBlock.append(QByteArray((char*)&us, sizeof(us)));

    return byteBlock;
}
