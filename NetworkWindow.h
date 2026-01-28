#pragma once
#include <QWidget>

class QFrame;
class SpeedGaugeWidget;
class QLabel;
class QLineEdit;
class QPushButton;
class QTimer;

class ApiClient;
class NetworkMonitor;

class NetworkWindow : public QWidget {
    Q_OBJECT
public:
    explicit NetworkWindow(QWidget* parent=nullptr);

private slots:
    void refreshPing();   // auto refresh (1s)
    void runSpeedTest();  // download + upload speed test

private:
    void buildUi();
    void wire();

    // UI
    QLineEdit* m_backendUrl{};
    QPushButton* m_pingBtn{};
    QLabel* m_pingLatency{};
    QLabel* m_serverStatus{};
    QLabel* m_connectionType{};

    QFrame* m_statusDot{};
    QPushButton* m_rebootBtn{};

    // Gauge + stats
    SpeedGaugeWidget* m_pingGauge{};
    QLabel* m_pingTopValue{};
    QLabel* m_downloadTopValue{};
    QLabel* m_uploadTopValue{};

    // Auto refresh
    QTimer* m_pingTimer{};
    bool m_pingInFlight{false}; // évite d'empiler les requêtes

    // Speed test
    QPushButton* m_speedTestBtn{};
    QTimer* m_speedTestTimer{};
    bool m_downloadInFlight{false};
    bool m_uploadInFlight{false};

    // Services
    ApiClient* m_api{};
    NetworkMonitor* m_netMon{};
};
