#include "LoginWindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCryptographicHash>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Login");
    setFixedSize(300, 160);

    userEdit = new QLineEdit(this);
    userEdit->setPlaceholderText("Username");

    passEdit = new QLineEdit(this);
    passEdit->setPlaceholderText("Password");
    passEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    loginButton->setObjectName("btnPrimary");


    auto *layout = new QVBoxLayout(this);
    layout->addWidget(userEdit);
    layout->addWidget(passEdit);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    const QString user = userEdit->text().trimmed();
    const QString pass = passEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields.");
        return;
    }

    if (checkCredentials(user, pass)) {
        emit loginSuccess();
        close();
    } else {
        QMessageBox::critical(this, "Error", "Invalid credentials");
    }
}

bool LoginWindow::checkCredentials(const QString &user, const QString &pass)
{
    // Look for users.json next to the executable (debug/release)
    const QString jsonPath = QCoreApplication::applicationDirPath() + "/users.json";

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open users.json.\n\n"
                              "Make sure users.json is in the same folder as the executable "
                              "(debug/release).");
        return false;
    }

    const auto doc = QJsonDocument::fromJson(file.readAll());
    const auto users = doc.object().value("users").toArray();

    const QString passHash = QString::fromLatin1(
        QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha256).toHex()
    );

    for (const auto &u : users) {
        const auto obj = u.toObject();
        if (obj.value("username").toString() == user &&
            obj.value("password").toString() == passHash) {
            return true;
        }
    }
    return false;
}
