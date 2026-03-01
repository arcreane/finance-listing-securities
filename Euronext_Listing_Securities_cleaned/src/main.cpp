#include <QApplication>
#include <QCommandLineParser>
#include "ui/LoginWindow.h"
#include "ui/NetworkWindow.h"
#include "ui/Theme.h"
#include "Config.h"

// Initialize static member
Config::Environment Config::currentEnv = Config::Environment::Local;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("NetworkUser");
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Network User Client");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption envOption(QStringList() << "e" << "env", 
            "Environment to use (local/real)", "env", "local");
    parser.addOption(envOption);
    parser.process(app);

    QString env = parser.value(envOption).toLower();
    if (env == "real") {
        Config::currentEnv = Config::Environment::Real;
    } else {
        Config::currentEnv = Config::Environment::Local;
    }

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
