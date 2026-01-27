QT += widgets network
CONFIG += c++17
SOURCES += src/main.cpp \
           src/ui/NetworkWindow.cpp \
           src/ui/SpeedGaugeWidget.cpp \
           src/ui/Theme.cpp \
           src/services/ApiClient.cpp \
           src/services/NetworkMonitor.cpp \
           src/ui/SpeedGaugeWidget.cpp
HEADERS += src/ui/NetworkWindow.h \
           src/ui/SpeedGaugeWidget.h \
           src/ui/Theme.h \
           src/services/ApiClient.h \
           src/services/NetworkMonitor.h \
           src/ui/SpeedGaugeWidget.h
