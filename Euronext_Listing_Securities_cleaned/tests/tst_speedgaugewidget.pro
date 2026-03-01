QT += testlib widgets
CONFIG += qt warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

TEMPLATE = app

SOURCES += \
    tst_speedgaugewidget.cpp \
    ../src/ui/SpeedGaugeWidget.cpp

HEADERS += \
    ../src/ui/SpeedGaugeWidget.h

TARGET = tst_speedgaugewidget
