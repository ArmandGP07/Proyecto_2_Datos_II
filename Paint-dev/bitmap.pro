QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


HEADERS += \
    main_window.h \
    dialog_windows.h \
    commands.h \
    draw_area.h \
    toolbar.h \
    tool.h \
    constants.h
SOURCES += main.cpp \
    main_window.cpp \
    commands.cpp \
    dialog_windows.cpp \
    toolbar.cpp \
    draw_area.cpp \
    tool.cpp
CONFIG += qt warn_on
CONFIG += debug

RESOURCES += \
    icons.qrc



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

