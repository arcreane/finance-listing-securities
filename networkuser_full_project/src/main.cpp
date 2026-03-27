#include <QApplication>
#include "ui/NetworkWindow.h"
#include "ui/Theme.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet(Theme::qss());
    NetworkWindow w;
    w.show();
    return app.exec();
}
