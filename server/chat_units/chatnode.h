#ifndef CHATNODE_H
#define CHATNODE_H

#include <QObject>
#include <QtCore>
#include "chatunit.h"

class ChatNodeTree;
class ChatNode : public ChatUnit
{
    Q_OBJECT
public:
    enum eNodeType
    {
        Branch,
        Room
    };

    explicit ChatNode(eNodeType ndType, QObject *parent = nullptr, bool createID = true);

    bool fromSQLQuery(const QSqlQuery &qry) override;

    eNodeType getNodeType() { return ndType; };
    void setNodeName(QString new_name){ name = new_name; };
    const QString& getNodeName(){ return name; };

    static eNodeType peakNodeType(const QByteArray& a, int ptr);

protected:
    bool innerFromByteBlock(const QByteArray &a, int &ptr) override;
    QByteArray innerGetByteBlock() override;

    virtual QByteArray getNodeSpecifcByteBlock() = 0;
    virtual bool fromNodeSpecificByteBlock(const QByteArray& a, int &ptr) = 0;

    bool attachToParent(ChatUnit* parent);

    QString name;
    eNodeType ndType;


signals:

};

#endif // CHATNODE_H
