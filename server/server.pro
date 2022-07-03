QT -= gui
QT += core network sql

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#QMAKE_CXXFLAGS += -P

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        backend/activeuser.cpp \
        backend/chatbackend.cpp \
        chat_units/chatbranch.cpp \
        chat_units/chatmessage.cpp \
        chat_units/chatnode.cpp \
        chat_units/chatnodetree.cpp \
        chat_units/chatroom.cpp \
        chat_units/chatserver.cpp \
        chat_units/chatunit.cpp \
        chat_units/chatuser.cpp \
        chat_units/chatuserlist.cpp \
        main.cpp \
        tcp_messages/tcp_bad_request.cpp \
        tcp_messages/tcp_chat_history.cpp \
        tcp_messages/tcp_handle_connection.cpp \
        tcp_messages/tcp_keep_alive.cpp \
        tcp_messages/tcp_metadata.cpp \
        tcp_messages/tcp_new_message.cpp \
        tcp_messages/tcp_request_chat_history.cpp \
        tcp_messages/tcp_request_connecton.cpp \
        tcp_messages/tcp_request_disconnect.cpp \
        tcp_messages/tcp_request_metadata.cpp \
        tcp_messages/tcp_update_user_status.cpp \
        tcp_messages/tcpmessage.cpp \
        tcp_messages/tcp_send_text_message.cpp \
        utils.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend/activeuser.h \
    backend/chatbackend.h \
    chat.h \
    chat_units/chat.h \
    chat_units/chatbranch.h \
    chat_units/chatmessage.h \
    chat_units/chatnode.h \
    chat_units/chatnodetree.h \
    chat_units/chatroom.h \
    chat_units/chatserver.h \
    chat_units/chatunit.h \
    chat_units/chatuser.h \
    chat_units/chatuserlist.h \
    tcp_messages/chat_tcp_messages.h \
    tcp_messages/tcp_bad_request.h \
    tcp_messages/tcp_chat_history.h \
    tcp_messages/tcp_handle_connection.h \
    tcp_messages/tcp_keep_alive.h \
    tcp_messages/tcp_metadata.h \
    tcp_messages/tcp_new_message.h \
    tcp_messages/tcp_request_chat_history.h \
    tcp_messages/tcp_request_connection.h \
    tcp_messages/tcp_request_disconnect.h \
    tcp_messages/tcp_request_metadata.h \
    tcp_messages/tcp_update_user_status.h \
    tcp_messages/tcpmessage.h \
    tcp_messages/tcp_send_text_message.h \
    utils.h

DISTFILES +=
