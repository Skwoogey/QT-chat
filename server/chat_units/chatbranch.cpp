#include "chatbranch.h"

ChatBranch::ChatBranch(QObject* parent, bool createID) : ChatNode(eNodeType::Branch, parent, createID)
{

}

bool ChatBranch::addChildNode(ChatNode *node)
{
    if(node->parent() != this)
        return false;

    if(childNodes.contains(node))
        return true;

    childNodes.append(node);

    return true;
}

QByteArray ChatBranch::getNodeSpecifcByteBlock()
{
    quint32 nodeCount = childNodes.size();
    QByteArray byteBlock((char*)&nodeCount, sizeof (nodeCount));

    for(ChatNode* node : childNodes)
    {
        byteBlock.append(node->getByteBlock());
    }

    return byteBlock;
}

bool ChatBranch::fromNodeSpecificByteBlock(const QByteArray &a, int &ptr)
{
    quint32 nodeCount = *(quint32*)getArrayPtr(a, ptr, sizeof(nodeCount));

    for(quint32 i = 0; i < nodeCount; i++)
    {
        auto nodeType = ChatNode::peakNodeType(a, ptr);
        ChatNode* node;

        if(nodeType  == ChatNode::eNodeType::Branch)
            node = new ChatBranch(this, false);
        else if(nodeType  == ChatNode::eNodeType::Room)
            node = new ChatRoom(this, false);
        else
        {
            qDebug() << "ERROR(ChatBranch): illegal nodeType!"
                     << "\nndType: " << nodeType;
            isValidUnit = false;
            return isValidUnit;
        }

        if(!node->fromByteBlock(a, ptr))
        {
            isValidUnit = false;
            return  isValidUnit;
        }
        addChildNode(node);
    }

    return isValidUnit;
}

void ChatBranch::innerPrint(QString prefix)
{
    qDebug().noquote() << prefix << "ChatBranch";
    qDebug().noquote() << prefix << "childNodes:" << childNodes.size();
    qDebug().noquote() << prefix << "name:" << name;

    for(ChatNode* node : childNodes)
    {
        node->print(prefix);
    }
}
