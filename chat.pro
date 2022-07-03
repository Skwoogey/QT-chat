TEMPLATE = subdirs

SUBDIRS += \
    client \
    server

client.depends = server
