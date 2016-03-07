#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T20:02:12
#
#-------------------------------------------------

QT       += core gui uitools network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scopegui
TEMPLATE = app

VERSION = $$system(git describe --tags)
isEmpty(VERSION) {
    VERSION=$${VERSION}
}

SOURCES += main.cpp\
        mainwindow.cpp \
    info.cpp \
    connection.cpp

HEADERS  += mainwindow.h \
    info.h \
    connection.h

FORMS    += mainwindow.ui \
    info.ui \
    connection.ui

RESOURCES += \
    resources.qrc

LIBS += -lprotobuf

include(protobuf.pri)
PROTOS += scopeproto/scope_msg_request.proto \
          scopeproto/scope_msg_response.proto

VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"
