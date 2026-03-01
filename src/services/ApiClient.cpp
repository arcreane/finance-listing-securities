#include "ApiClient.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

static QString normalizeBaseUrl(QString s) {
    s = s.trimmed();
    if (s.endsWith('/')) s.chop(1);
    return s;
}

ApiClient::ApiClient(QObject* p) : QObject(p) {
    m_nam = new QNetworkAccessManager(this);
}

void ApiClient::ping(const QString& base) {
    const QString b = normalizeBaseUrl(base);
    const qint64 start = QDateTime::currentMSecsSinceEpoch();

    QNetworkRequest req(QUrl(b + "/ping"));
    req.setHeader(QNetworkRequest::UserAgentHeader, "networkuser/1.0");

    auto* r = m_nam->get(req);
    connect(r, &QNetworkReply::finished, this, [this, r, start] {
        const qint64 ms = QDateTime::currentMSecsSinceEpoch() - start;

        QString msg;
        const QByteArray body = r->readAll();

        if (!body.isEmpty()) {
            // Priority: extract server_host to display the real AlwaysData address
            QJsonParseError e{};
            const QJsonDocument doc = QJsonDocument::fromJson(body, &e);
            if (e.error == QJsonParseError::NoError && doc.isObject()) {
                const QJsonObject obj = doc.object();
                const QString serverHost = obj.value("server_host").toString();
                if (!serverHost.isEmpty()) {
                    msg = serverHost;
                } else {
                    msg = obj.value("message").toString();
                    if (msg.isEmpty()) msg = obj.value("status").toString();
                }
            } else {
                msg = QString::fromUtf8(body.left(120));
            }
        }

        const bool ok = (r->error() == QNetworkReply::NoError);
        if (!ok && msg.isEmpty())
            msg = r->errorString();

        emit pingFinished(ok, ms, msg);
        r->deleteLater();
    });
}

void ApiClient::login(const QString& base, const QString& username, const QString& password) {
    const QString b = normalizeBaseUrl(base);

    QNetworkRequest req(QUrl("https://www.google.com"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::UserAgentHeader, "networkuser/1.0");

    QJsonObject payload;
    payload["username"] = username;
    payload["password"] = password;

    const QByteArray body = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    // Important: copy username for the lambda
    const QString userFallback = username;

    auto* r = m_nam->post(req, body);
    connect(r, &QNetworkReply::finished, this, [this, r, userFallback] {
        const QByteArray raw = r->readAll();

        if (r->error() != QNetworkReply::NoError) {
            emit loginFinished(false, "", "", r->errorString());
            r->deleteLater();
            return;
        }

        QJsonParseError e{};
        const QJsonDocument doc = QJsonDocument::fromJson(raw, &e);
        if (e.error != QJsonParseError::NoError || !doc.isObject()) {
            emit loginFinished(false, "", "", "Invalid JSON response");
            r->deleteLater();
            return;
        }

        const QJsonObject o = doc.object();
        const bool success = o.value("success").toBool(false) || o.value("ok").toBool(false);

        if (!success) {
            const QString err = o.value("error").toString("Rejected");
            emit loginFinished(false, "", "", err);
            r->deleteLater();
            return;
        }

        const QString user = o.value("username").toString(o.value("user").toString(userFallback));
        const QString token = o.value("token").toString();

        if (token.isEmpty()) {
            emit loginFinished(false, "", "", "Missing token");
            r->deleteLater();
            return;
        }

        emit loginFinished(true, user, token, "");
        r->deleteLater();
    });
}

void ApiClient::testDownloadSpeed(const QString& base) {
    const QString b = normalizeBaseUrl(base);
    const qint64 start = QDateTime::currentMSecsSinceEpoch();

    // Request a download payload from server (e.g., 1MB of data)
    QNetworkRequest req(QUrl(b + "/speedtest/download"));
    req.setHeader(QNetworkRequest::UserAgentHeader, "networkuser/1.0");

    auto* r = m_nam->get(req);
    connect(r, &QNetworkReply::finished, this, [this, r, start] {
        const qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - start;

        if (r->error() != QNetworkReply::NoError) {
            emit downloadSpeedFinished(false, -1, r->errorString());
            r->deleteLater();
            return;
        }

        const QByteArray data = r->readAll();
        const qint64 bytes = data.size();

        // Calculate speed: bytes / seconds, then convert to Mbps
        // Mbps = (bytes * 8) / (elapsed_ms / 1000) / 1,000,000
        //      = (bytes * 8 * 1000) / (elapsed_ms * 1,000,000)
        //      = (bytes * 8) / (elapsed_ms * 1000)
        double speedMbps = 0.0;
        if (elapsed > 0) {
            speedMbps = (static_cast<double>(bytes) * 8.0) / (static_cast<double>(elapsed) * 1000.0);
        }

        emit downloadSpeedFinished(true, speedMbps, QString("Downloaded %1 bytes").arg(bytes));
        r->deleteLater();
    });
}

void ApiClient::testUploadSpeed(const QString& base) {
    const QString b = normalizeBaseUrl(base);

    // Create upload payload (1MB of random data)
    const int uploadSize = 1 * 1024 * 1024; // 1 MB
    QByteArray payload(uploadSize, 'X');

    QNetworkRequest req(QUrl(b + "/speedtest/upload"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    req.setHeader(QNetworkRequest::UserAgentHeader, "networkuser/1.0");

    const qint64 start = QDateTime::currentMSecsSinceEpoch();

    auto* r = m_nam->post(req, payload);
    connect(r, &QNetworkReply::finished, this, [this, r, start, uploadSize] {
        const qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - start;

        if (r->error() != QNetworkReply::NoError) {
            emit uploadSpeedFinished(false, -1, r->errorString());
            r->deleteLater();
            return;
        }

        // Calculate speed in Mbps
        double speedMbps = 0.0;
        if (elapsed > 0) {
            speedMbps = (static_cast<double>(uploadSize) * 8.0) / (static_cast<double>(elapsed) * 1000.0);
        }

        emit uploadSpeedFinished(true, speedMbps, QString("Uploaded %1 bytes").arg(uploadSize));
        r->deleteLater();
    });
}

