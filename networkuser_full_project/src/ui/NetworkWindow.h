#pragma once
#include <QWidget>

// Forward declarations (UNE SEULE FOIS)
class QFrame;
class SpeedGaugeWidget;
class QLabel;
class QLineEdit;
class QPushButton;

class ApiClient;
class NetworkMonitor;

class NetworkWindow : public QWidget {
    Q_OBJECT
public:
    explicit NetworkWindow(QWidget* parent=nullptr);

private:
    void buildUi();
    void wire();

    // UI
    QLineEdit* m_backendUrl{};
    QPushButton* m_pingBtn{};
    QLabel* m_pingLatency{};
    QLabel* m_serverStatus{};
    QLabel* m_connectionType{};

    QLineEdit* m_username{};
    QLineEdit* m_password{};
    QPushButton* m_loginBtn{};
    QLabel* m_loginStatus{};

    QFrame* m_statusDot{};
    QPushButton* m_rebootBtn{};

    // Gauge + stats
    SpeedGaugeWidget* m_pingGauge{};
    QLabel* m_pingTopValue{};
    QLabel* m_downloadTopValue{};
    QLabel* m_uploadTopValue{};

    // Services
    ApiClient* m_api{};
    NetworkMonitor* m_netMon{};
};
