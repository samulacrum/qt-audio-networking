#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T14:37:46
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = soft336
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    model.cpp \
    audioinput.cpp \
    audiooutput.cpp \
    client.cpp \
    server.cpp


HEADERS  += mainwindow.h \
    model.h \
    audioinput.h \
    audiooutput.h \
    client.h \
    server.h

FORMS    += mainwindow.ui

OTHER_FILES +=
