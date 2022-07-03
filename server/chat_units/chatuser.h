#ifndef CHATUSER_H
#define CHATUSER_H

#include <QObject>
#include <QtCore>
#include "chatunit.h"
#include "utils.h"

class ChatUserList;
class ChatUser : public ChatUnit
{
    Q_OBJECT

public:
    enum eUserStatus
    {
        userOnline,
        userOffline
    };

    explicit ChatUser(QObject *parent = nullptr, bool createID = true);

    bool fromSQLQuery(const QSqlQuery &qry) override;

    void setUserName(QString new_name) { name = new_name; };
    const QString& getUserName() const { return name; };

    const QString& getUserPassword(){ return password; };

    void setStatus(eUserStatus s) { status = s; }
    eUserStatus getStatus() const { return status; }

protected:
    QString name;
    QString password;
    eUserStatus status = eUserStatus::userOffline;

    bool innerFromByteBlock(const QByteArray &a, int &ptr) override;
    void innerPrint(QString prefix) override;
    QByteArray innerGetByteBlock() override;

signals:

};

#endif // CHATUSER_H
