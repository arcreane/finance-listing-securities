#include "NetworkWindow.h"

#include "../services/ApiClient.h"
#include "../services/NetworkMonitor.h"
#include "SpeedGaugeWidget.h"

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

    // Premier refresh immédiat
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

    // Base URL line
    m_backendUrl = new QLineEdit("http://127.0.0.1:8080");
    root->addWidget(m_backendUrl);

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
    connect(m_api, &ApiClient::pingFinished, this, [this](bool ok, qint64 ms, const QString&){
        m_pingInFlight = false;

        m_pingLatency->setText(QString::number(ms) + " ms");
        m_serverStatus->setText(ok ? "UP" : "DOWN");
        m_pingTopValue->setText(QString::number(ms) + " ms");

        // Gauge = ping ms
        m_pingGauge->setValue(double(ms));

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

void NetworkWindow::refreshPing()
{
    if (m_pingInFlight) return; // évite d'empiler si le serveur répond lentement

    const QString base = normalizeBaseUrl(m_backendUrl->text());
    if (base.isEmpty()) return;

    m_pingInFlight = true;
    m_api->ping(base);
}

void NetworkWindow::runSpeedTest()
{
    const QString base = normalizeBaseUrl(m_backendUrl->text());
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
