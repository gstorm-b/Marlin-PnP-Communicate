QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    blineedit.cpp \
    main.cpp \
    mainwindow.cpp \
    serial/marlin_host.cpp \
    serial/serialsettingdialog.cpp \
    timer/TimeCounter.cpp

HEADERS += \
    blineedit.h \
    mainwindow.h \
    serial/marlin_host.h \
    serial/serialsettingdialog.h \
    timer/TimeCounter.h

FORMS += \
    mainwindow.ui \
    serial/serialsettingdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
