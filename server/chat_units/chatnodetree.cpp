#include "chatnodetree.h"
#include "chatnode.h"
#include "chatserver.h"

ChatNodeTree::ChatNodeTree(QObject *parent, bool createID) : ChatUnit(parent, createID)
{
    if(parent != nullptr)
        static_cast<ChatServer*>(parent)->setNodeTree(this);
}

bool ChatNodeTree::addRootNode(ChatNode *node)
{
    if(node->parent() != this)
        return false;

    if(rootNodes.contains(node))
        return true;

    rootNodes.append(node);
    return true;
}

bool ChatNodeTree::fromSQLQuery(const QSqlQuery &qry)
{
    auto idVariant = qry.value(2);
    if(!idVariant.isValid())
    {
        qDebug() << "ERROR(ChatNodeTree): id variant is invalid";
        isValidUnit = false;
        return false;
    }

    id = idVariant.toUInt();
    return addUnitToList();
}

void ChatNodeTree::innerPrint(QString prefix)
{
    qDebug().noquote() << prefix << "NodeTree";
    qDebug().noquote() << prefix << "rootNodesCount:" << rootNodes.size();

    for(ChatNode* node : rootNodes)
    {
        node->print(prefix);
    }
}

QByteArray ChatNodeTree::innerGetByteBlock()
{
    quint32 nodeCount = rootNodes.size();
    QByteArray byteBlock((char*)&nodeCount, sizeof (nodeCount));

    for(ChatNode* node : rootNodes)
    {
        byteBlock.append(node->getByteBlock());
    }

    return byteBlock;
}

bool ChatNodeTree::innerFromByteBlock(const QByteArray &a, int& ptr)
{
    quint32 nodeCount = *(quint32*)getArrayPtr(a, ptr, sizeof(nodeCount));

    for(quint32 i = 0; i < nodeCount; i++)
    {
        auto nodeType = ChatNode::peakNodeType(a, ptr);
        ChatNode* node;

        if(nodeType == ChatNode::eNodeType::Branch)
            node = new ChatBranch(this, false);
        else if(nodeType  == ChatNode::eNodeType::Room)
            node = new ChatRoom(this, false);
        else
        {
            qDebug() << "ERROR(ChatNodeTree): illegal nodeType!"
                     << "\nndType: " << nodeType;
            isValidUnit = false;
            return isValidUnit;
        }

        if(!node->fromByteBlock(a, ptr))
        {
            isValidUnit = false;
            return  isValidUnit;
        }
        addRootNode(node);
    }

    return isValidUnit;
}
