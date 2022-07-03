#include "utils.h"

void insertIntoArray(QByteArray &msg, char* data, int length, int& msg_ptr)
{
    msg.replace(msg_ptr, length, data, length);
    msg_ptr += length;
}

const char* getArrayPtr(const QByteArray &a, int& msgPtr, int length)
{
    if(a.size() < msgPtr + length)
        throw(msgPtr + length);

    const char* data = (a.data() + msgPtr);

    msgPtr += length;

    return data;
}

const char* getArrayPtrConst(const QByteArray &a, const int msgPtr, int length)
{
    if(a.size() < msgPtr + length)
        throw(msgPtr + length);

    const char* data = (a.data() + msgPtr);

    return data;
}

QByteArray getBAfromString(const QString &a)
{
    auto BA = a.toUtf8();

    quint32 BAlength = BA.length();
    BA.insert(0, (char*)&BAlength, sizeof(BAlength));

    return BA;
}

QString getStringFromBA(const QByteArray &a, int &msgPtr)
{
    quint32 stringBAlength = *(quint32*)getArrayPtr(a, msgPtr, sizeof(stringBAlength));
    QString string = QString::fromUtf8(getArrayPtr(a, msgPtr, stringBAlength), stringBAlength);

    return string;
}

void insertStringIntoArray(QByteArray &msg, QString &str, int &msgPtr)
{
    auto strBA = getBAfromString(str);
    msg.replace(msgPtr, strBA.length(), strBA);

    msgPtr += strBA.length();
}

QByteArray getBAfromQDT(const QDateTime &dt)
{
    qint32 dta[6];
    dta[0] = dt.date().year();
    dta[1] = dt.date().month();
    dta[2] = dt.date().day();

    dta[3] = dt.time().hour();
    dta[4] = dt.time().minute();
    dta[5] = dt.time().second();

    return QByteArray((char*)dta, sizeof(dta));
}

QDateTime getQDTfromBA(const QByteArray& ba, int& msgPtr)
{
    qint32 dta[6];

    dta[0] = *getValuePtrFromArray<qint32>(ba, msgPtr);
    dta[1] = *getValuePtrFromArray<qint32>(ba, msgPtr);
    dta[2] = *getValuePtrFromArray<qint32>(ba, msgPtr);

    dta[3] = *getValuePtrFromArray<qint32>(ba, msgPtr);
    dta[4] = *getValuePtrFromArray<qint32>(ba, msgPtr);
    dta[5] = *getValuePtrFromArray<qint32>(ba, msgPtr);

    return QDateTime{
        QDate(dta[0], dta[1], dta[2]),
        QTime(dta[3], dta[4], dta[5])
    };
}
