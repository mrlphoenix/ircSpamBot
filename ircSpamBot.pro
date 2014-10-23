#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T07:50:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ircSpamBot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grabmessageircsocket.cpp \
    ircprivmessageparser.cpp \
    spammessagesocket.cpp \
    abstractircsocket.cpp \
    spamsocketmanager.cpp

HEADERS  += mainwindow.h \
    grabmessageircsocket.h \
    ircprivmessageparser.h \
    spammessagesocket.h \
    abstractircsocket.h \
    spamsocketmanager.h

FORMS    += mainwindow.ui
