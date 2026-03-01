QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

TEMPLATE = app

SOURCES += \
    tst_apiclient.cpp \
    ../src/services/ApiClient.cpp

HEADERS += \
    ../src/services/ApiClient.h

TARGET = tst_apiclient
