#ifndef CHATTREEWIDGETITEM_H
#define CHATTREEWIDGETITEM_H

#include <QObject>
#include <QTreeWidgetItem>
#include "chat_units/chatnode.h"

class ChatTreeWidgetItem : public QTreeWidgetItem
{

public:
    ChatTreeWidgetItem(QTreeWidgetItem* parent = nullptr);
    ChatTreeWidgetItem(QTreeWidget* parent = nullptr) : QTreeWidgetItem(parent) {}

    void setNode(ChatNode* nrm) { room = nrm; }
    ChatNode* getNode() { return room; }

    bool isPopulated() { return populated; }
    void setPopulated(bool pp) { populated = pp; }

private:
    ChatNode* room;
    bool populated = false;
};

#endif // CHATTREEWIDGETITEM_H
