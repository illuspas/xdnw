#-------------------------------------------------
#
# Project created by QtCreator 2011-02-16T21:46:19
#
#-------------------------------------------------

QT       += core gui dbus

TARGET = xdnw
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    dnw.cpp

HEADERS  += widget.h \
    dnw.h

FORMS    += widget.ui

LIBS += -lusb

