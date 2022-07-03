#include "chatroom.h"

ChatRoom::ChatRoom(QObject* parent, bool createID) : ChatNode(eNodeType::Room, parent, createID)
{

}

bool ChatRoom::addMessage(ChatMessage *cm)
{
    if(cm->parent() != this)
        return false;

    if(messages.contains(cm))
        return false;

    messages.append(cm);
    return true;
}

QByteArray ChatRoom::getChatHistory()
{
    quint32 messageCount = messages.size();
    QByteArray byteBlock((char*)&id, sizeof (id));
    byteBlock.append((char*)&messageCount, sizeof (messageCount));

    for(ChatMessage* msg : messages)
    {
        byteBlock.append(msg->getByteBlock());
    }

    return byteBlock;
}

bool ChatRoom::fromChatHistory(const QByteArray &a, int& msgPtr)
{
    for( auto msg : messages)
    {
        msg->clear();
    }
    messages.clear();
    qDebug() << "chat history";

    quint32 cid = *getValuePtrFromArray<quint32>(a, msgPtr);
    if(cid != id)
    {
        qDebug() << "ERROR(ChatRoom): id midmatch!"
                 << "\nthis id: " << id
                 << "\nother id: " << cid;
        isValidUnit = false;
        return isValidUnit;
    }

    quint32 messageCount = *getValuePtrFromArray<quint32>(a, msgPtr);
    for(quint32 i = 0; i < messageCount; i++)
    {
        ChatMessage* msg = new ChatMessage(this, false);

        if(!msg->fromByteBlock(a, msgPtr))
        {
            isValidUnit = false;
            return isValidUnit;
        }
    }
}

void ChatRoom::printChatHistory()
{
    print();

    for(auto msg : messages)
    {
        msg->print();
    }
}

ChatRoom* ChatRoom::dispatchChatHistory(const QByteArray &a, int &msgPtr)
{
    quint32 cid = *peakValuePtrFromArray<quint32>(a, msgPtr);

    ChatRoom* targetRoom = dynamic_cast<ChatRoom*>(getUnitByID(cid));

    if(targetRoom == nullptr)
    {
        qDebug() << "ERROR(ChatRoom): cannot find room of id!"
                 << "\nid:" << cid;
        return nullptr;
    }

    targetRoom->fromChatHistory(a, msgPtr);

    return targetRoom;
}

QByteArray ChatRoom::getNodeSpecifcByteBlock()
{
    return QByteArray();
}

bool ChatRoom::fromNodeSpecificByteBlock(const QByteArray &a, int &ptr)
{
    return isValidUnit;
}

void ChatRoom::innerPrint(QString prefix)
{
    qDebug().noquote() << prefix << "ChatRoom";
    qDebug().noquote() << prefix << "name:" << name;
}
