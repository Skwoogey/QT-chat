QT       += core gui
QT       += network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += console

INCLUDEPATH += ../server/

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../server/utils.cpp \
    ../server/chat_units/chatbranch.cpp \
    ../server/chat_units/chatmessage.cpp \
    ../server/chat_units/chatnode.cpp \
    ../server/chat_units/chatnodetree.cpp \
    ../server/chat_units/chatroom.cpp \
    ../server/chat_units/chatserver.cpp \
    ../server/chat_units/chatunit.cpp \
    ../server/chat_units/chatuser.cpp \
    ../server/chat_units/chatuserlist.cpp \
    chatclient.cpp \
    chatmessagewidget.cpp \
    chattreewidgetitem.cpp \
    main.cpp \
    dialog.cpp \
    ../server/tcp_messages/tcp_bad_request.cpp \
    ../server/tcp_messages/tcp_chat_history.cpp \
    ../server/tcp_messages/tcp_handle_connection.cpp \
    ../server/tcp_messages/tcp_keep_alive.cpp \
    ../server/tcp_messages/tcp_metadata.cpp \
    ../server/tcp_messages/tcp_new_message.cpp \
    ../server/tcp_messages/tcp_request_chat_history.cpp \
    ../server/tcp_messages/tcp_request_connecton.cpp \
    ../server/tcp_messages/tcp_request_disconnect.cpp \
    ../server/tcp_messages/tcp_request_metadata.cpp \
    ../server/tcp_messages/tcp_send_text_message.cpp \
    ../server/tcp_messages/tcp_update_user_status.cpp \
    ../server/tcp_messages/tcpmessage.cpp

HEADERS += \
    ../server/utils.h \
    ../server/chat_units/chat.h \
    ../server/chat_units/chatbranch.h \
    ../server/chat_units/chatmessage.h \
    ../server/chat_units/chatnode.h \
    ../server/chat_units/chatnodetree.h \
    ../server/chat_units/chatroom.h \
    ../server/chat_units/chatserver.h \
    ../server/chat_units/chatunit.h \
    ../server/chat_units/chatuser.h \
    ../server/chat_units/chatuserlist.h \
    chatclient.h \
    chatmessagewidget.h \
    chattreewidgetitem.h \
    dialog.h \
    tcp_messages.lnk/chat_tcp_messages.h \
    tcp_messages.lnk/chat_tcp_messages.h \
    tcp_messages.lnk/tcp_bad_request.h \
    tcp_messages.lnk/tcp_bad_request.h \
    tcp_messages.lnk/tcp_chat_history.h \
    tcp_messages.lnk/tcp_chat_history.h \
    tcp_messages.lnk/tcp_handle_connection.h \
    tcp_messages.lnk/tcp_handle_connection.h \
    tcp_messages.lnk/tcp_keep_alive.h \
    tcp_messages.lnk/tcp_keep_alive.h \
    tcp_messages.lnk/tcp_metadata.h \
    tcp_messages.lnk/tcp_metadata.h \
    tcp_messages.lnk/tcp_new_message.h \
    tcp_messages.lnk/tcp_new_message.h \
    tcp_messages.lnk/tcp_request_chat_history.h \
    tcp_messages.lnk/tcp_request_chat_history.h \
    tcp_messages.lnk/tcp_request_connection.h \
    tcp_messages.lnk/tcp_request_connection.h \
    tcp_messages.lnk/tcp_request_disconnect.h \
    tcp_messages.lnk/tcp_request_disconnect.h \
    tcp_messages.lnk/tcp_request_metadata.h \
    tcp_messages.lnk/tcp_request_metadata.h \
    tcp_messages.lnk/tcp_send_text_message.h \
    tcp_messages.lnk/tcp_send_text_message.h \
    tcp_messages.lnk/tcp_update_user_status.h \
    tcp_messages.lnk/tcp_update_user_status.h \
    tcp_messages.lnk/tcpmessage.h \
    tcp_messages.lnk/tcpmessage.h

FORMS += \
    dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
