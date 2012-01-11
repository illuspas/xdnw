#-------------------------------------------------
#
# Project created by QtCreator 2011-02-16T21:46:19
#
#-------------------------------------------------

QT       += core gui
QT+=xml
TARGET = xdnw
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    dnw.cpp \
    modelsetup.cpp

HEADERS  += widget.h \
    dnw.h \
    modelsetup.h

FORMS    += widget.ui \
    modelsetup.ui

LIBS += -lusb

