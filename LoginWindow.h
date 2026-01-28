#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccess();

private slots:
    void onLoginClicked();

private:
    bool checkCredentials(const QString &user, const QString &pass);

    QLineEdit *userEdit;
    QLineEdit *passEdit;
    QPushButton *loginButton;
};

#endif // LOGINWINDOW_H
