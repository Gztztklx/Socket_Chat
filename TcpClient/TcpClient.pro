#-------------------------------------------------
#
# Project created by QtCreator 2022-02-03T12:57:01
#
#-------------------------------------------------

QT       += core gui
QT       += network#网络编程
QT       += sql#数据库
QT       += multimedia#摄像机

QT       += multimediawidgets #使用多媒体
CONFIG   += mobility
MOBILITY += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    protocol.cpp \
    control.cpp \
    boardcast.cpp \
    abstractvideosurface.cpp \
    dialog.cpp \
    thread.cpp \
    privatechat.cpp \
    thread_3.cpp

HEADERS += \
        widget.h \
    protocol.h \
    control.h \
    boardcast.h \
    abstractvideosurface.h \
    dialog.h \
    thread.h \
    privatechat.h \
    thread_3.h

FORMS += \
        widget.ui \
    control.ui \
    boardcast.ui \
    dialog.ui \
    privatechat.ui

DISTFILES +=

RESOURCES += \
    client.qrc \
    picture.qrc \
    picture.qrc
