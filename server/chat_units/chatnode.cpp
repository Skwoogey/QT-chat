#include "chatnode.h"
#include "chatnodetree.h"

ChatNode::ChatNode(eNodeType ndType, QObject *parent, bool createID) : ChatUnit(parent, createID), ndType(ndType)
{

    if(parent != nullptr)
    {
        auto cnt = dynamic_cast<ChatNodeTree*>(parent);
        if(cnt)
            cnt->addRootNode(this);
        else
            dynamic_cast<ChatBranch*>(parent)->addChildNode(this);
    }

}

ChatNode::eNodeType ChatNode::peakNodeType(const QByteArray &a, int ptr)
{
    return *(eNodeType*)getArrayPtrConst(a, ptr + sizeof(blockSize) + sizeof(id), sizeof(eNodeType));
}

bool ChatNode::innerFromByteBlock(const QByteArray &a, int &ptr)
{
    ndType = *(eNodeType*)getArrayPtr(a, ptr, sizeof(ndType));
    quint32 nameBAlength = *(quint32*)getArrayPtr(a, ptr, sizeof(nameBAlength));
    name = QString::fromUtf8(getArrayPtr(a, ptr, nameBAlength), nameBAlength);

    return fromNodeSpecificByteBlock(a, ptr);
}

QByteArray ChatNode::innerGetByteBlock()
{
    QByteArray nameByteArray = name.toUtf8();
    quint32 nameBAlength = nameByteArray.length();
    QByteArray byteBlock(sizeof(ndType) + sizeof(nameBAlength) + nameBAlength, Qt::Initialization::Uninitialized);

    int ptr = 0;
    insertIntoArray(byteBlock, (char*)&ndType, sizeof(ndType), ptr);
    insertIntoArray(byteBlock, (char*)&nameBAlength, sizeof(nameBAlength), ptr);
    insertIntoArray(byteBlock, nameByteArray.data(), nameBAlength, ptr);

    byteBlock.append(getNodeSpecifcByteBlock());

    return byteBlock;
}

bool ChatNode::fromSQLQuery(const QSqlQuery &qry)
{
    auto idVariant = qry.value(0);
    if(!idVariant.isValid())
    {
        qDebug() << "ERROR(ChatNode): id variant is invalid";
        isValidUnit = false;
        return false;
    }
    id = idVariant.toUInt();

    if(!addUnitToList())
    {
        qDebug() << "ERROR(ChatNode): failed to add unit to list";
        isValidUnit = false;
        return false;
    }

    auto parrentIdVariant = qry.value(1);
    if(!parrentIdVariant.isValid())
    {
        qDebug() << "ERROR(ChatNode): id variant is invalid";
        isValidUnit = false;
        return false;
    }

    ChatUnit* parent =  getUnitByID(parrentIdVariant.toUInt());
    if(parent == nullptr)
    {
        qDebug() << "ERROR(ChatNode): cannot get parent node";
        isValidUnit = false;
        return false;
    }

    setParent(parent);
    if(!attachToParent(parent))
        return false;

    auto nameVariant = qry.value(2);
    if(!nameVariant.isValid())
    {
        qDebug() << "ERROR(ChatNode): name variant is invalid";
        isValidUnit = false;
        return false;
    }
    name = nameVariant.toString();

    return isValidUnit;
}

bool ChatNode::attachToParent(ChatUnit *parent)
{
    qDebug() << parent->getID();
    auto cnt = dynamic_cast<ChatNodeTree*>(parent);
    if(cnt)
    {
        cnt->addRootNode(this);
        return true;
    }
    auto cn = dynamic_cast<ChatBranch*>(parent);
    if(cn)
    {
         cn->addChildNode(this);
         return true;
    }
    qDebug() << "ERROR(ChatNode): failed to attach to parrent";
    isValidUnit = false;
    return false;
}
