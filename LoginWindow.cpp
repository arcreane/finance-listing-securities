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

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Login");
    setFixedSize(300, 160);

    userEdit = new QLineEdit(this);
    userEdit->setPlaceholderText("Pseudo");

    passEdit = new QLineEdit(this);
    passEdit->setPlaceholderText("Mot de passe");
    passEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Connexion", this);
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
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    if (checkCredentials(user, pass)) {
        emit loginSuccess();
        close();
    } else {
        QMessageBox::critical(this, "Erreur", "Identifiants incorrects");
    }
}

bool LoginWindow::checkCredentials(const QString &user, const QString &pass)
{
    // On cherche users.json à côté de l'exécutable (debug/release)
    const QString jsonPath = QCoreApplication::applicationDirPath() + "/users.json";

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible d'ouvrir users.json.\n\n"
                              "Assure-toi que users.json est dans le même dossier que l'exécutable "
                              "(debug/release).");
        return false;
    }

    const auto doc = QJsonDocument::fromJson(file.readAll());
    const auto users = doc.object().value("users").toArray();

    for (const auto &u : users) {
        const auto obj = u.toObject();
        if (obj.value("username").toString() == user &&
            obj.value("password").toString() == pass) {
            return true;
        }
    }
    return false;
}
