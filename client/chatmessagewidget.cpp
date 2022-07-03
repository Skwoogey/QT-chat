#include "chatmessagewidget.h"

QList<ChatMessageWidget*> ChatMessageWidget::allMessages;

ChatMessageWidget::ChatMessageWidget(ChatMessage* nmsg)
{
    widget = makeNewMessage(nmsg);
    msg = nmsg;

    allMessages.append(this);
}

void ChatMessageWidget::enableRoom(ChatRoom *room)
{
    for(auto msg : allMessages)
    {
        msg->makeVisibleMsgsOfRoom(room);
    }
}

void ChatMessageWidget::clear()
{
    for(auto msg : allMessages)
    {
        msg->widget->deleteLater();
    }
    allMessages.clear();
}

void ChatMessageWidget::makeVisibleMsgsOfRoom(ChatRoom *room)
{
    widget->setHidden(room != &msg->getRoom());
}

QFrame *ChatMessageWidget::makeNewMessage(ChatMessage* msg)
{
    QFrame* frame = new QFrame;
    frame->setHidden(true);
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
    frame->setLineWidth(1);
    frame->setFrameShape(QFrame::Panel);
    frame->setMinimumHeight(100);
    QHBoxLayout* title = new QHBoxLayout;

    QLabel* name = new QLabel(msg->getAuthor().getUserName());
    QFont nameFont;
    nameFont .setBold(true);
    nameFont.setPixelSize(18);
    name->setFont(nameFont);
    name->setSizePolicy(
                QSizePolicy(
                    QSizePolicy::Policy::Minimum,
                    QSizePolicy::Policy::Minimum
                )
    );
    title->addWidget(name, 0, Qt::AlignLeft);

    QLabel* time = new QLabel(msg->getTimeStamp().toString());
    QFont timeFont;
    timeFont.setItalic(true);
    timeFont.setPixelSize(15);
    time->setFont(timeFont);
    time->setSizePolicy(
                QSizePolicy(
                    QSizePolicy::Policy::Minimum,
                    QSizePolicy::Policy::Minimum
                )
    );
    title->addWidget(time, 0, Qt::AlignRight);


    QVBoxLayout* message = new QVBoxLayout;
    frame->setLayout(message);
    message->setMargin(5);
    title->setAlignment(Qt::AlignTop);
    message->addLayout(title, 0);

    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setLineWidth(1);
    line->setFixedHeight(3);
    line->setSizePolicy(
                QSizePolicy(
                    QSizePolicy::Policy::Expanding,
                    QSizePolicy::Policy::Fixed
                )
    );

    message->addWidget(line, 0, Qt::AlignTop);

    QVBoxLayout *msgBody = new QVBoxLayout;
    QLabel *text = new QLabel(msg->getText());
    text->setWordWrap(true);
    text->setSizePolicy(
                QSizePolicy(
                    QSizePolicy::Policy::Expanding,
                    QSizePolicy::Policy::Preferred
                )
    );
    message->addLayout(msgBody, 5);
    msgBody->setAlignment(Qt::AlignTop);
    msgBody->addWidget(text, 0,  Qt::AlignHCenter | Qt::AlignLeft);

    return frame;
}
