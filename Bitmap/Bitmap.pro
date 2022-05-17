HEADERS += \
    widget.h
SOURCES += main.cpp \
    widget.cpp
CONFIG += qt warn_on
CONFIG += debug
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
