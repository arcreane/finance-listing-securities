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
            // Si le backend renvoie du JSON {message:"pong"} on essaie de lire "message"
            QJsonParseError e{};
            const QJsonDocument doc = QJsonDocument::fromJson(body, &e);
            if (e.error == QJsonParseError::NoError && doc.isObject()) {
                msg = doc.object().value("message").toString();
                if (msg.isEmpty()) msg = doc.object().value("status").toString();
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

    QNetworkRequest req(QUrl(b + "/login"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::UserAgentHeader, "networkuser/1.0");

    QJsonObject payload;
    payload["username"] = username;
    payload["password"] = password;

    const QByteArray body = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    // ✅ Important: copier username pour le lambda
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

