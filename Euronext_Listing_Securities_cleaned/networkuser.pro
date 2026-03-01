QT += widgets network
CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/ui/NetworkWindow.cpp \
    src/ui/LoginWindow.cpp \
    src/ui/SpeedGaugeWidget.cpp \
    src/ui/Theme.cpp \
    src/services/ApiClient.cpp \
    src/services/NetworkMonitor.cpp

HEADERS += \
    src/ui/NetworkWindow.h \
    src/ui/LoginWindow.h \
    src/ui/SpeedGaugeWidget.h \
    src/ui/Theme.h \
    src/services/ApiClient.h \
    src/services/NetworkMonitor.h

DISTFILES += \
    src/ressources/users.json

# --- Copy users.json next to the executable after build (Windows/MSVC) ---
CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/debug
} else {
    DESTDIR = $$OUT_PWD/release
}

win32 {
    QMAKE_POST_LINK += $$quote(cmd /c copy /y "$$shell_path($$PWD/src/ressources/users.json)" "$$shell_path($$DESTDIR/users.json)")
}
