#include "chatuserlist.h"

ChatUserList::ChatUserList(QObject* parent, bool createID) : ChatUnit(parent, createID)
{
    if(parent != nullptr)
        static_cast<ChatServer*>(parent)->setUserList(this);
}

bool ChatUserList::addUser(ChatUser *user)
{
    if(user->parent() != this)
        return false;

    if(users.contains(user))
        return true;

    users.append(user);

    return true;
}

bool ChatUserList::fromSQLQuery(const QSqlQuery &qry)
{
    auto idVariant = qry.value(1);
    if(!idVariant.isValid())
    {
        qDebug() << "ERROR(ChatUserList): id variant is invalid";
        isValidUnit = false;
        return false;
    }

    id = idVariant.toUInt();
    return addUnitToList();
}

ChatUser *ChatUserList::findUser(const QString &n, const QString &p) const
{
    for(ChatUser* user : users)
    {
        qDebug() << user->getUserName() <<user->getUserPassword();
        if(user->getUserName() == n &&
           user->getUserPassword() == p)
            return user;
    }
    return nullptr;
}

void ChatUserList::innerPrint(QString prefix)
{
    qDebug().noquote() << prefix << "UserList";
    qDebug().noquote() << prefix << "usersCount:" << users.size();

    for(ChatUser* user : users)
    {
        user->print(prefix + prefix[0]);
    }
}

bool ChatUserList::innerFromByteBlock(const QByteArray &a, int &ptr)
{
    quint32 userCount = *(quint32*)getArrayPtr(a, ptr, sizeof(userCount));

    for(quint32 i = 0; i < userCount; i++)
    {
        ChatUser* user = new ChatUser(this, false);
        if(!user->fromByteBlock(a, ptr))
        {
            isValidUnit = false;
            return  isValidUnit;
        }
        addUser(user);
    }

    return isValidUnit;
}

QByteArray ChatUserList::innerGetByteBlock()
{
    quint32 userCount = users.size();
    QByteArray byteBlock((char*)&userCount, sizeof(userCount));

    for(ChatUser* user : users)
    {
        byteBlock.append(user->getByteBlock());
    }

    return byteBlock;
}
