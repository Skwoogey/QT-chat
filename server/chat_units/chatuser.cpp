#include "chatuser.h"
#include "chatuserlist.h"

ChatUser::ChatUser(QObject *parent, bool createID) : ChatUnit(parent, createID)
{
    if(parent != nullptr)
        static_cast<ChatUserList*>(parent)->addUser(this);
}

bool ChatUser::innerFromByteBlock(const QByteArray &a, int &ptr)
{
    name = getStringFromBA(a, ptr);
    status = *getValuePtrFromArray<eUserStatus>(a, ptr);

    return isValidUnit;
}

void ChatUser::innerPrint(QString prefix)
{
    qDebug().noquote() << prefix << "CharUser";
    qDebug().noquote() << prefix << "name:" << name;
    qDebug().noquote() << prefix << "status:" << status;
}

bool ChatUser::fromSQLQuery(const QSqlQuery &qry)
{
    auto idVariant = qry.value(0);
    if(!idVariant.isValid())
    {
        qDebug() << "ERROR(ChatUser): id variant is invalid";
        isValidUnit = false;
        return false;
    }

    id = idVariant.toUInt();

    if(!addUnitToList())
    {
        qDebug() << "ERROR(ChatUser): failed to add unit to list";
        isValidUnit = false;
        return false;
    }

    auto userListIdVariant = qry.value(1);
    if(!userListIdVariant.isValid())
    {
        qDebug() << "ERROR(ChatUser): user_list_id variant is invalid";
        isValidUnit = false;
        return false;
    }

    ChatUserList* ul =  dynamic_cast<ChatUserList*>(getUnitByID(userListIdVariant.toUInt()));
    if(ul == nullptr)
    {
        qDebug() << "ERROR(ChatUser): cannot get user list";
        isValidUnit = false;
        return false;
    }

    setParent(ul);

    ul->addUser(this);

    auto nameVariant = qry.value(2);
    if(!nameVariant.isValid())
    {
        qDebug() << "ERROR(ChatUser): name variant is invalid";
        isValidUnit = false;
        return false;
    }
    name = nameVariant.toString();

    auto passVariant = qry.value(3);
    if(!passVariant.isValid())
    {
        qDebug() << "ERROR(ChatUser): password variant is invalid";
        isValidUnit = false;
        return false;
    }
    password = passVariant.toString();

    return isValidUnit;
}

QByteArray ChatUser::innerGetByteBlock()
{
    return getBAfromString(name).append((char*)&status, sizeof(status));
}
