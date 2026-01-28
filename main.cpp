#include <QApplication>
#include "ui/LoginWindow.h"
#include "ui/NetworkWindow.h"
#include "ui/Theme.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Theme::apply(app);

    LoginWindow login;
    NetworkWindow *mainWindow = nullptr;

    QObject::connect(&login, &LoginWindow::loginSuccess, [&]() {
        mainWindow = new NetworkWindow();
        mainWindow->show();
        login.hide();
    });

    login.show();
    return app.exec();
}
