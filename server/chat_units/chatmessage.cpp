#include "chatmessage.h"

ChatMessage::ChatMessage(QObject *parent, bool createID) : ChatUnit(parent, createID)
{
    timeStamp = QDateTime::currentDateTime();
    if(parent == nullptr)
        return;
    static_cast<ChatRoom*>(parent)->addMessage(this);
    room = static_cast<ChatRoom*>(parent);
}

bool ChatMessage::fromSQLQuery(const QSqlQuery &qry)
{
    auto idVariant = qry.value(0);
    if(!idVariant.isValid())
    {
        qDebug() << "ERROR(ChatMessage): id variant is invalid";
        isValidUnit = false;
        return false;
    }
    id = idVariant.toUInt();

    if(!addUnitToList())
    {
        qDebug() << "ERROR(ChatMessage): failed to add unit to list";
        isValidUnit = false;
        return false;
    }

    auto authorIdVariant = qry.value(1);
    if(!authorIdVariant.isValid())
    {
        qDebug() << "ERROR(ChatMessage): author_id variant is invalid";
        isValidUnit = false;
        return false;
    }
    author =  dynamic_cast<ChatUser*>(getUnitByID(authorIdVariant.toUInt()));
    if(author == nullptr)
    {
        qDebug() << "ERROR(ChatMessage): cannot get author";
        isValidUnit = false;
        return false;
    }

    auto chatIdVariant = qry.value(2);
    if(!chatIdVariant.isValid())
    {
        qDebug() << "ERROR(ChatMessage): chat_id variant is invalid";
        isValidUnit = false;
        return false;
    }
    room =  dynamic_cast<ChatRoom*>(getUnitByID(chatIdVariant.toUInt()));
    if(room == nullptr)
    {
        qDebug() << "ERROR(ChatMessage): cannot get room";
        isValidUnit = false;
        return false;
    }

    setParent(room);
    room->addMessage(this);

    auto dateTimeVariant = qry.value(3);
    if(!dateTimeVariant.isValid())
    {
        qDebug() << "ERROR(ChatMessage): timeStamp variant is invalid";
        isValidUnit = false;
        return false;
    }
    timeStamp = dateTimeVariant.toDateTime();

    auto textVariant = qry.value(4);
    if(!textVariant.isValid())
    {
        qDebug() << "ERROR(ChatMessage): text variant is invalid";
        isValidUnit = false;
        return false;
    }
    text = textVariant.toString();

    return isValidUnit;
}

QByteArray ChatMessage::innerGetByteBlock()
{
    /*
    QDateTime timeStamp;
    ChatUser* author;
    ChatRoom* room;
    QString text;
    */

    auto byteBlock = getBAfromQDT(timeStamp);

    auto author_id = author->getID();
    auto room_id = room->getID();

    byteBlock.append((char*)&author_id, sizeof(author_id));
    byteBlock.append((char*)&room_id, sizeof(room_id));

    byteBlock.append(getBAfromString(text));

    return byteBlock;
}

void ChatMessage::innerPrint(QString prefix)
{
    author->print(prefix);
    room->print(prefix);
    qDebug().noquote() << prefix << "timeStamp:" << timeStamp.toString();
    qDebug().noquote() << prefix << "text:" << text;
}

bool ChatMessage::innerFromByteBlock(const QByteArray &a, int &ptr)
{
    timeStamp = getQDTfromBA(a, ptr);

    quint32 author_id = *getValuePtrFromArray<quint32>(a, ptr);
    quint32 room_id = *getValuePtrFromArray<quint32>(a, ptr);

    author = dynamic_cast<ChatUser*>(getUnitByID(author_id));
    room = dynamic_cast<ChatRoom*>(getUnitByID(room_id));

    setParent(room);
    room->addMessage(this);

    if(author == nullptr || room == nullptr)
    {
        qDebug() << "ERROR(ChatMessage): cannot get room or author!";
        isValidUnit = false;
        return isValidUnit;
    }

    text  = getStringFromBA(a,ptr);

    return isValidUnit;
}














