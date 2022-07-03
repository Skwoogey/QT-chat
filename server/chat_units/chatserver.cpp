#include "chatserver.h"

ChatServer::ChatServer(QObject *parent, bool createID) : ChatUnit(parent, createID)
{

}

bool ChatServer::innerFromByteBlock(const QByteArray &a, int &ptr)
{
    tree = new ChatNodeTree(this, false);
    if(!tree->fromByteBlock(a, ptr))
    {
        isValidUnit = false;
        return isValidUnit;
    }

    userList = new ChatUserList(this, false);
    if(!userList->fromByteBlock(a, ptr))
    {
        isValidUnit = false;
        return isValidUnit;
    }

    return isValidUnit;
}

QByteArray ChatServer::innerGetByteBlock()
{
    return tree->getByteBlock().append(userList->getByteBlock());
}

bool ChatServer::fromSQLQuery(const QSqlQuery &qry)
{
    auto idVariant = qry.value(0);
    if(!idVariant.isValid())
    {
        qDebug() << "ERROR(ChatServer): id variant is invalid";
        isValidUnit = false;
        return false;
    }
    id = idVariant.toUInt();
    addUnitToList();

    userList = new ChatUserList(this, false);
    tree = new ChatNodeTree(this, false);
    isValidUnit = (userList->fromSQLQuery(qry) &&
                   tree->fromSQLQuery(qry));


    return isValidUnit;
}

void ChatServer::innerPrint(QString prefix)
{
    tree->print(prefix);
    userList->print(prefix);
}
