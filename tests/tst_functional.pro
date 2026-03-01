QT += testlib gui widgets network

TARGET = tst_functional
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += tst_functional.cpp \
    ../../src/services/ApiClient.cpp \
    ../../src/services/NetworkMonitor.cpp \
    ../../src/ui/LoginWindow.cpp \
    ../../src/ui/NetworkWindow.cpp \
    ../../src/ui/SpeedGaugeWidget.cpp \
    ../../src/ui/Theme.cpp

HEADERS += \
    ../../src/services/ApiClient.h \
    ../../src/services/NetworkMonitor.h \
    ../../src/ui/LoginWindow.h \
    ../../src/ui/NetworkWindow.h \
    ../../src/ui/SpeedGaugeWidget.h \
    ../../src/ui/Theme.h
