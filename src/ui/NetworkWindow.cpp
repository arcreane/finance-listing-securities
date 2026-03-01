#include "NetworkWindow.h"

#include "../services/ApiClient.h"
#include "../services/NetworkMonitor.h"
#include "SpeedGaugeWidget.h"
#include "../Config.h"

#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QTimer>
#include <QtMath>

static QString normalizeBaseUrl(QString s) {
    s = s.trimmed();
    if (s.endsWith('/')) s.chop(1);
    return s;
}

static QFrame* makeCard(const QString& title, QLabel*& outValue, QWidget* parent=nullptr) {
    auto* card = new QFrame(parent);
    card->setObjectName("card");

    auto* lay = new QVBoxLayout(card);
    lay->setContentsMargins(14, 10, 14, 10);
    lay->setSpacing(6);

    auto* t = new QLabel(title);
    t->setObjectName("cardTitle");

    outValue = new QLabel("—");
    outValue->setObjectName("cardValue");

    lay->addWidget(t);
    lay->addSpacing(6);
    lay->addWidget(outValue);
    lay->addStretch(1);
    return card;
}

static QWidget* statBox(const QString& label, QLabel*& valueOut) {
    auto* w = new QWidget;
    auto* l = new QVBoxLayout(w);
    l->setContentsMargins(0,0,0,0);
    l->setSpacing(2);

    auto* t = new QLabel(label);
    t->setObjectName("cardTitleBold");

    valueOut = new QLabel("—");
    valueOut->setObjectName("cardValue");

    l->addWidget(t);
    l->addWidget(valueOut);
    return w;
}

NetworkWindow::NetworkWindow(QWidget* parent) : QWidget(parent) {
    m_api = new ApiClient(this);
    m_netMon = new NetworkMonitor(this);

    buildUi();
    wire();

    setWindowTitle("Network User");
    resize(1100, 700);

    // Auto refresh ping every 1 second
    m_pingTimer = new QTimer(this);
    m_pingTimer->setInterval(1000);
    connect(m_pingTimer, &QTimer::timeout, this, &NetworkWindow::refreshPing);
    m_pingTimer->start();

    // Auto refresh speed test every 30 seconds
    m_speedTestTimer = new QTimer(this);
    m_speedTestTimer->setInterval(30000); // 30 seconds
    connect(m_speedTestTimer, &QTimer::timeout, this, &NetworkWindow::runSpeedTest);
    m_speedTestTimer->start();

    // Initial refresh
    refreshPing();
    runSpeedTest();
}

void NetworkWindow::buildUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16,16,16,16);
    root->setSpacing(12);

    // ===== Top bar =====
    auto* topBar = new QHBoxLayout;
    auto* titleLbl = new QLabel("Network User");
    QFont tf = titleLbl->font();
    tf.setPointSize(14);
    tf.setBold(true);
    titleLbl->setFont(tf);

    m_statusDot = new QFrame;
    m_statusDot->setFixedSize(10, 10);
    m_statusDot->setObjectName("statusDotOffline");

    topBar->addWidget(titleLbl);
    topBar->addStretch();
    topBar->addWidget(m_statusDot);

    root->addLayout(topBar);

    // Server URL rows — Server 1, Server 2 (local), Real Server (AlwaysData)
    auto* urlRow = new QHBoxLayout;
    auto* srv1Lbl = new QLabel("Server 1 :");
    m_backendUrl = new QLineEdit("http://127.0.0.1:8080");
    auto* srv2Lbl = new QLabel("Server 2 :");
    m_backendUrl2 = new QLineEdit("http://127.0.0.1:8081");
    urlRow->addWidget(srv1Lbl);
    urlRow->addWidget(m_backendUrl, 1);
    urlRow->addWidget(srv2Lbl);
    urlRow->addWidget(m_backendUrl2, 1);
    root->addLayout(urlRow);

    // Real Server row
    auto* realRow = new QHBoxLayout;
    auto* realLbl = new QLabel("Real Server :");
    QFont rf = realLbl->font();
    rf.setBold(true);
    realLbl->setFont(rf);
    m_backendUrlReal = new QLineEdit(Config::getBaseUrl());
    realRow->addWidget(realLbl);
    realRow->addWidget(m_backendUrlReal, 1);
    root->addLayout(realRow);

    // Active server row
    auto* switchRow = new QHBoxLayout;
    m_activeServer = (Config::currentEnv == Config::Environment::Real) ? 3 : 1;
    QString initialLabel = (m_activeServer == 3) ? "Active server: Real (AlwaysData)" : "Active server: 1";
    m_activeServerLbl = new QLabel(initialLabel);
    m_switchBtn = new QPushButton("Switch Server");
    m_switchBtn->setObjectName("btnPrimary");
    m_switchBtn->setCursor(Qt::PointingHandCursor);
    switchRow->addWidget(m_activeServerLbl);
    switchRow->addStretch(1);
    switchRow->addWidget(m_switchBtn);
    root->addLayout(switchRow);

    // Identity Proof Row
    auto* proofRow = new QHBoxLayout;
    m_proofLbl = new QLabel("Proof of Identity: —");
    proofRow->addWidget(m_proofLbl);
    root->addLayout(proofRow);

    // ===== Cards grid =====
    auto* grid = new QGridLayout;
    grid->setSpacing(12);

    QLabel* connValue = nullptr;
    auto* connCard = makeCard("Connection Type", connValue);
    m_connectionType = connValue;

    QLabel* latencyValue = nullptr;
    auto* latencyCard = makeCard("Latency", latencyValue);
    m_pingLatency = latencyValue;

    QLabel* statusValue = nullptr;
    auto* statusCard = makeCard("Server status", statusValue);
    m_serverStatus = statusValue;

    grid->addWidget(connCard,    0, 0);
    grid->addWidget(latencyCard, 0, 1);
    grid->addWidget(statusCard,  1, 0, 1, 2); // span 2 columns

    root->addLayout(grid);

    // ===== Throughput / Gauge card =====
    auto* gaugeCard = new QFrame;
    gaugeCard->setObjectName("card");

    auto* gLay = new QVBoxLayout(gaugeCard);
    gLay->setContentsMargins(18, 14, 18, 18);
    gLay->setSpacing(10);

    // Header row: PING / DOWNLOAD / UPLOAD
    auto* hdr = new QHBoxLayout;
    hdr->setSpacing(24);

    hdr->addWidget(statBox("PING", m_pingTopValue));
    hdr->addWidget(statBox("DOWNLOAD", m_downloadTopValue));
    hdr->addWidget(statBox("UPLOAD", m_uploadTopValue));
    hdr->addStretch(1);

    m_pingTopValue->setText("— ms");
    m_downloadTopValue->setText("— Mbps");
    m_uploadTopValue->setText("— Mbps");

    gLay->addLayout(hdr);

    // Big gauge
    m_pingGauge = new SpeedGaugeWidget;
    m_pingGauge->setRange(0, 1000);
    m_pingGauge->setUnit("ms");
    m_pingGauge->setLabel("PING");
    m_pingGauge->setValue(0);

    gLay->addWidget(m_pingGauge, 0, Qt::AlignHCenter);

    root->addWidget(gaugeCard, 1);

    // ===== Bottom actions =====
    auto* actions = new QHBoxLayout;
    actions->setSpacing(12);

    m_pingBtn = new QPushButton("Ping");
    m_pingBtn->setObjectName("btnPrimary");
    m_pingBtn->setCursor(Qt::PointingHandCursor);

    m_speedTestBtn = new QPushButton("Speed Test");
    m_speedTestBtn->setObjectName("btnPrimary");
    m_speedTestBtn->setCursor(Qt::PointingHandCursor);

    actions->addWidget(m_pingBtn);
    actions->addWidget(m_speedTestBtn);
    actions->addStretch(1);

    root->addLayout(actions);

    // Reboot
    m_rebootBtn = new QPushButton("Reboot Connection");
    m_rebootBtn->setObjectName("btnPrimary");
    m_rebootBtn->setFixedHeight(44);
    m_rebootBtn->setCursor(Qt::PointingHandCursor);
    root->addWidget(m_rebootBtn);
}

void NetworkWindow::wire() {
    // Switch server button — cycles 1 → 2 → 3 (Real) → 1
    connect(m_switchBtn, &QPushButton::clicked, this, [this]{
        m_activeServer = (m_activeServer % 3) + 1;  // 1→2→3→1
        m_failCount = 0;
        const QString label = (m_activeServer == 3)
            ? QString("Active server: Real (AlwaysData)")
            : QString("Active server: %1").arg(m_activeServer);
        m_activeServerLbl->setText(label);
        m_pingInFlight = false;
        refreshPing();
    });

    // Connection type
    m_connectionType->setText(m_netMon->connectionType());
    connect(m_netMon, &NetworkMonitor::connectionTypeChanged, this, [this](const QString& t){
        m_connectionType->setText(t);
    });

    // Ping button triggers refresh too
    connect(m_pingBtn, &QPushButton::clicked, this, [this]{
        m_pingLatency->setText("—");
        m_serverStatus->setText("Checking…");
        m_pingTopValue->setText("… ms");
        refreshPing();
    });

    // Receive ping result
    connect(m_api, &ApiClient::pingFinished, this, [this](bool ok, qint64 ms, const QString& msg){
        m_pingInFlight = false;

        if (ok) {
            m_failCount = 0;
            m_pingLatency->setText(QString::number(ms) + " ms");
            m_pingTopValue->setText(QString::number(ms) + " ms");
            m_serverStatus->setText("UP");
            m_pingGauge->setValue(double(ms));

            // Show server_host returned by real server in the proof label
            if (!msg.isEmpty() && m_activeServer == 3 && msg != "pong" && msg != "online" && msg.contains('.')) {
                m_proofLbl->setText(QString("Real server connected: %1").arg(msg));
            } else if (!msg.isEmpty()) {
                m_proofLbl->setText(QString("Server: %1").arg(msg));
            }
        } else {
            m_pingLatency->setText("— ms");
            m_pingTopValue->setText("— ms");
            m_pingGauge->setValue(0);
            m_serverStatus->setText("DOWN");
            m_proofLbl->setText("Server identity: UNKNOWN");

            ++m_failCount;
            // Auto-switch after 3 consecutive failures — cycle 1→2→3→1
            static constexpr int kFailThreshold = 3;
            if (m_failCount >= kFailThreshold) {
                m_failCount = 0;
                const int previous = m_activeServer;
                m_activeServer = (m_activeServer % 3) + 1;
                const QString prevLabel = (previous == 3) ? "Real" : QString::number(previous);
                const QString nextLabel = (m_activeServer == 3) ? "Real" : QString::number(m_activeServer);
                m_activeServerLbl->setText(
                    QString("Active server: %1  [auto-switch from %2]").arg(nextLabel).arg(prevLabel));
                refreshPing();
            }
        }

        // status dot
        m_statusDot->setObjectName(ok ? "statusDotOnline" : "statusDotOffline");
        m_statusDot->style()->unpolish(m_statusDot);
        m_statusDot->style()->polish(m_statusDot);
        m_statusDot->update();
    });

    // Reboot (front only for now)
    connect(m_rebootBtn, &QPushButton::clicked, this, [this]{
        QMessageBox::information(this, "Reboot", "Reboot requested (front only).");
    });

    // Speed test button
    connect(m_speedTestBtn, &QPushButton::clicked, this, &NetworkWindow::runSpeedTest);

    // Download speed result
    connect(m_api, &ApiClient::downloadSpeedFinished, this, [this](bool ok, double speedMbps, const QString&){
        m_downloadInFlight = false;
        if (ok) {
            m_downloadTopValue->setText(QString::number(speedMbps, 'f', 2) + " Mbps");
        } else {
            m_downloadTopValue->setText("Error");
        }
    });

    // Upload speed result
    connect(m_api, &ApiClient::uploadSpeedFinished, this, [this](bool ok, double speedMbps, const QString&){
        m_uploadInFlight = false;
        if (ok) {
            m_uploadTopValue->setText(QString::number(speedMbps, 'f', 2) + " Mbps");
        } else {
            m_uploadTopValue->setText("Error");
        }
    });
}

QString NetworkWindow::activeUrl() const
{
    QString raw;
    if (m_activeServer == 1)      raw = m_backendUrl->text();
    else if (m_activeServer == 2) raw = m_backendUrl2->text();
    else                          raw = m_backendUrlReal->text(); // Server 3 = Real
    QString s = raw.trimmed();
    if (s.endsWith('/')) s.chop(1);
    return s;
}

void NetworkWindow::refreshPing()
{
    if (m_pingInFlight) return; // avoid stacking requests if server is slow

    const QString base = activeUrl();
    if (base.isEmpty()) return;

    m_pingInFlight = true;
    m_api->ping(base);
}

void NetworkWindow::runSpeedTest()
{
    const QString base = activeUrl();
    if (base.isEmpty()) return;

    // Start download test
    if (!m_downloadInFlight) {
        m_downloadInFlight = true;
        m_downloadTopValue->setText("Testing...");
        m_api->testDownloadSpeed(base);
    }

    // Start upload test
    if (!m_uploadInFlight) {
        m_uploadInFlight = true;
        m_uploadTopValue->setText("Testing...");
        m_api->testUploadSpeed(base);
    }
}
