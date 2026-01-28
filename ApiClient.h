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

    // Speed tests
    void testDownloadSpeed(const QString& base);
    void testUploadSpeed(const QString& base);

signals:
    void pingFinished(bool ok, qint64 ms, const QString& msg);
    void loginFinished(bool success, const QString& user, const QString& token, const QString& err);

    // Speed test signals: speedMbps = -1 on error
    void downloadSpeedFinished(bool ok, double speedMbps, const QString& msg);
    void uploadSpeedFinished(bool ok, double speedMbps, const QString& msg);

private:
    QNetworkAccessManager* m_nam{};
};
