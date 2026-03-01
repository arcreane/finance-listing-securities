#include "NetworkMonitor.h"

#include <QNetworkInterface>
#include <QTimer>

static bool looksLikeWifi(const QString& name, const QString& human) {
    const QString s = (name + " " + human).toLower();
    // Windows: "wi-fi", "wlan" ; Linux: "wlan", "wifi" ; macOS: "wi-fi", "airport"
    return s.contains("wi-fi") || s.contains("wifi") || s.contains("wlan") || s.contains("airport");
}

static bool looksLikeEthernet(const QString& name, const QString& human) {
    const QString s = (name + " " + human).toLower();
    // Common names: "ethernet", "en", "eth", "lan"
    return s.contains("ethernet") || s.contains("eth") || s.contains("lan");
}

NetworkMonitor::NetworkMonitor(QObject* parent) : QObject(parent) {
    m_cached = detectType();

    // Light polling to detect connection changes (every 2s)
    m_timer = new QTimer(this);
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, this, [this]{
        const QString t = detectType();
        if (t != m_cached) {
            m_cached = t;
            emit connectionTypeChanged(m_cached);
        }
    });
    m_timer->start();
}

QString NetworkMonitor::detectType() const {
    // Strategy:
    // - Iterate over UP + RUNNING interfaces (non-loopback)
    // - Priority: WiFi > Ethernet > Other
    bool foundOther = false;

    const auto ifaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : ifaces) {
        const auto flags = iface.flags();
        if (flags.testFlag(QNetworkInterface::IsLoopBack)) continue;
        if (!flags.testFlag(QNetworkInterface::IsUp)) continue;
        if (!flags.testFlag(QNetworkInterface::IsRunning)) continue;

        // Require at least one IP to consider the interface "active"
        bool hasIp = false;
        for (const auto& addr : iface.addressEntries()) {
            if (!addr.ip().isNull() && addr.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                hasIp = true;
                break;
            }
        }
        if (!hasIp) continue;

        // Use Qt's interface type API first (reliable on Windows 10/11, Linux, macOS)
        const auto ifType = iface.type();
        if (ifType == QNetworkInterface::Wifi) return "WiFi";
        if (ifType == QNetworkInterface::Ethernet) return "Ethernet";

        // Fallback: name-based heuristic for unknown/other types
        const QString name = iface.name();
        const QString human = iface.humanReadableName();

        if (looksLikeWifi(name, human)) return "WiFi";
        if (looksLikeEthernet(name, human)) return "Ethernet";

        foundOther = true;
    }

    if (foundOther) return "Connected";
    return "Unknown";
}
