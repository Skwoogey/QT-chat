#include "chatunit.h"

quint32 ChatUnit::nextID = 0;
QMap<quint32, ChatUnit*> ChatUnit::allUnits;

ChatUnit::ChatUnit(QObject *parent, bool createID) : QObject(parent)
{
    //qDebug() << nextID;
    if(!createID)
        return;

    id = nextID;
    nextID++;

    if(!allUnits.contains(id))
        allUnits[id] = this;
}

ChatUnit *ChatUnit::getUnitByID(quint32 sid)
{
    return allUnits[sid];
}

void ChatUnit::clearAll()
{
    for( ChatUnit* unit : allUnits)
    {
        unit->deleteLater();
    }
    allUnits.clear();
}



quint32 ChatUnit::peakID(const QByteArray &a, int ptr)
{
    try
    {
        quint32 bs = *(quint32*)getArrayPtr(a, ptr, sizeof(blockSize));
        quint32 unit_id = *(quint32*)getArrayPtr(a, ptr, sizeof(unit_id));

        return unit_id;

    } catch(int expected_size) {
        qDebug() << "ERROR(ChatUnit): Index out of range!"
                 << "\Expected Index:" << expected_size;
    }
}

void ChatUnit::clear()
{
    allUnits.remove(getID());
    deleteLater();
}

QByteArray ChatUnit::getByteBlock()
{
    auto byteBlock = innerGetByteBlock();

    quint32 length = byteBlock.size() + sizeof(length) + sizeof(id);

    byteBlock.insert(0, (char*)&id, sizeof(id));
    byteBlock.insert(0, (char*)&length, sizeof(length));

    return byteBlock;
}

bool ChatUnit::fromByteBlock(const QByteArray &a, int& ptr)
{
    try
    {
        int startPtr = ptr;
        isValidUnit = true;

        if(a.size() - ptr < sizeof(blockSize) + sizeof(id))
        {
            qDebug() << "ERROR(ChatUnit): Block is too small, cannot contain must-have fields!"
                     << "\nBlock:" << a.length()
                     << "\nblockSize:" << ptr;

            isValidUnit = false;
            return isValidUnit;
        }

        blockSize = *(quint32*)getArrayPtr(a, ptr, sizeof(blockSize));
        id = *(quint32*)getArrayPtr(a, ptr, sizeof(blockSize));

        if(allUnits.contains(id))
        {
            qDebug() << "WARNING: such id already exists!"
                     << "ID:" << id;
        }
        allUnits[id] = this;

        if(blockSize > a.length() - startPtr)
        {
            qDebug() << "ERROR(ChatUnit): Block Size too large!"
                     << "\nBlock:" << a.length()
                     << "\nblockSize:" << blockSize;

            isValidUnit = false;
            return isValidUnit;
        }
        if(!innerFromByteBlock(a, ptr))
        {
            isValidUnit = false;
            return isValidUnit;
        }

        if(blockSize != ptr - startPtr)
        {
            qDebug() << "ERROR(ChatUnit): Block Size mismatch!"
                     << "\nBlockSize:" << ptr - startPtr
                     << "\nblockSize:" << blockSize;

            isValidUnit = false;
            return isValidUnit;
        }

        return isValidUnit;
    } catch(int expected_size) {
        qDebug() << "ERROR(ChatUnit): Index out of range!"
                 << "\Expected Index:" << expected_size;

        isValidUnit = false;
        return isValidUnit;
    }

}

bool ChatUnit::fromByteBlock(const QByteArray &a)
{
    int ptr = 0;
    return fromByteBlock(a, ptr);
}



void ChatUnit::print(QString prefix)
{
    prefix += prefix[0];
    qDebug().noquote() << prefix << "uid:" << id;
    qDebug().noquote() << prefix << "isValid:" << isValidUnit;

    innerPrint(prefix);
}

bool ChatUnit::addUnitToList()
{
    if(!allUnits.contains(id))
    {
        allUnits[id] = this;
        return true;
    }
    else
    {
        qDebug() << "ChatUnit(FromSQL) id already present";
        isValidUnit = false;
        return false;
    }
}
