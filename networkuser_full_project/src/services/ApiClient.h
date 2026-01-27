#pragma once
#include <QObject>
#include <QString>

class QNetworkAccessManager;

class ApiClient : public QObject {
    Q_OBJECT
public:
    explicit ApiClient(QObject* parent=nullptr);

    void ping(const QString& base);
    void login(const QString& base, const QString& username, const QString& password);

signals:
    void pingFinished(bool ok, qint64 ms, const QString& msg);
    void loginFinished(bool success, const QString& user, const QString& token, const QString& err);

private:
    QNetworkAccessManager* m_nam{};
};
