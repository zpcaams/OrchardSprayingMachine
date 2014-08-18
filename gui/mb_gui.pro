#-------------------------------------------------
#
# Project created by QtCreator 2014-03-03T11:13:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mb_gui
TEMPLATE = app
VERSION = 0.0.1

SOURCES += main.cpp\
        mb_gui.cpp\
        libmodbus-3.0.5/src/modbus.c \
        libmodbus-3.0.5/src/modbus-data.c \
        libmodbus-3.0.5/src/modbus-rtu.c \
    qcustomplot/qcustomplot.cpp \
    mb_data_show.cpp

HEADERS  += mb_gui.h\
    modbus.h\
    modbus_rtu.h \
    qcustomplot/qcustomplot.h \
    mb_data_show.h

INCLUDEPATH += libmodbus-3.0.5\src  \
    qcustomplot

FORMS    += mb_gui.ui \
    mb_data_show.ui
