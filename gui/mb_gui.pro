#-------------------------------------------------
#
# Project created by QtCreator 2014-03-03T11:13:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mb_gui
TEMPLATE = app
VERSION = 0.0.1

SOURCES += main.cpp\
        mb_gui.cpp\
        libmodbus-3.0.5/src/modbus.c \
        libmodbus-3.0.5/src/modbus-data.c \
        libmodbus-3.0.5/src/modbus-rtu.c \
    com_thread.cpp

HEADERS  += mb_gui.h\
    modbus.h\
    modbus_rtu.h \
    com_thread.h

INCLUDEPATH += libmodbus-3.0.5\src

FORMS    += mb_gui.ui
