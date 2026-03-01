#pragma once
#include <QObject>
#include <QString>

class QTimer;

class NetworkMonitor : public QObject {
    Q_OBJECT
public:
    explicit NetworkMonitor(QObject* parent=nullptr);

    QString connectionType() const { return m_cached; }

signals:
    void connectionTypeChanged(const QString& type);

private:
    QString detectType() const;

    QString m_cached = "Unknown";
    QTimer* m_timer = nullptr;
};
