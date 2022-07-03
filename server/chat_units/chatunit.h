#ifndef CHATUNIT_H
#define CHATUNIT_H

#include <QObject>
#include <QtCore>
#include <QSqlQuery>
#include "utils.h"

class ChatUnit : public QObject
{
    Q_OBJECT
public:
    explicit ChatUnit(QObject *parent = nullptr, bool createID = true);

    static ChatUnit *getUnitByID(quint32 sid);
    static void setNextID(quint32 nid) { nextID = nid; }
    static void clearAll();
    static const quint32 getNextID() { return nextID; }
    static quint32 peakID(const QByteArray &a, int ptr);

    const quint32& getID() const { return id; };
    void clear();

    QByteArray getByteBlock();
    bool fromByteBlock(const QByteArray &a, int& ptr);
    bool fromByteBlock(const QByteArray &a);
    virtual bool fromSQLQuery(const QSqlQuery& qry) = 0;
    bool isValid() { return isValidUnit; };
    void print(QString prefix = " ");


protected:
    virtual void innerPrint(QString prefix) = 0;
    virtual QByteArray innerGetByteBlock() = 0;
    virtual bool innerFromByteBlock(const QByteArray &a, int& ptr) = 0;

    bool addUnitToList();

    quint32 id;
    quint32 blockSize;
    bool isValidUnit = true;

private:
    static QMap<quint32, ChatUnit*> allUnits;
    static quint32 nextID;



signals:

};

#endif // CHATUNIT_H
