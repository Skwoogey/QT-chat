#ifndef UTILS_H
#define UTILS_H

#include <QtCore>
#include <QtNetwork>

void insertIntoArray(QByteArray &msg, char* data, int length, int& msg_ptr);

const char* getArrayPtr(const QByteArray &a, int& msgPtr, int length);
const char* getArrayPtrConst(const QByteArray &a, const int msgPtr, int length);
QByteArray getBAfromString(const QString& a);
QString getStringFromBA(const QByteArray &a, int& msgPtr);
void insertStringIntoArray(QByteArray &msg, QString& str, int &msgPtr);
QByteArray getBAfromQDT(const QDateTime& dt);
QDateTime getQDTfromBA(const QByteArray& ba, int& msgPtr);

template<class T> inline T* getValuePtrFromArray(const QByteArray &a, int& msgPtr)
{
    if(a.size() < msgPtr + sizeof(T))
        throw(msgPtr + sizeof(T));

    T* data = (T*)(a.data() + msgPtr);
    msgPtr += sizeof(T);

    return data;
}

template<class T> inline T* peakValuePtrFromArray(const QByteArray &a, int msgPtr)
{
    if(a.size() < msgPtr + sizeof(T))
        throw(msgPtr + sizeof(T));

    T* data = (T*)(a.data() + msgPtr);

    return data;
}

#endif // UTILS_H
