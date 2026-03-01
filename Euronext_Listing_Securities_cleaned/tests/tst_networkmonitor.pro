QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

TEMPLATE = app

SOURCES += \
    tst_networkmonitor.cpp \
    ../src/services/NetworkMonitor.cpp

HEADERS += \
    ../src/services/NetworkMonitor.h

TARGET = tst_networkmonitor
