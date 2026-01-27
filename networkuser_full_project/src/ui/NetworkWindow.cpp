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
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

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
}

void NetworkWindow::buildUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16,16,16,16);
    root->setSpacing(12);

    // ===== Top bar (NO bell, NO navbar) =====
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

    // ===== Cards grid (NO Server, NO Proxy, NO VPN) =====
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

    grid->addWidget(connCard,   0, 0);
    grid->addWidget(latencyCard,0, 1);
    grid->addWidget(statusCard, 1, 0, 1, 2); // span 2 columns

    root->addLayout(grid);

    // ===== Throughput / Gauge card (style pièce jointe) =====
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
    m_pingGauge->setRange(0, 500);
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

    m_username = new QLineEdit;
    m_username->setPlaceholderText("username");

    m_password = new QLineEdit;
    m_password->setPlaceholderText("password");
    m_password->setEchoMode(QLineEdit::Password);

    m_loginBtn = new QPushButton("Login");
    m_loginBtn->setObjectName("btnPrimary");
    m_loginBtn->setCursor(Qt::PointingHandCursor);

    actions->addWidget(m_pingBtn);
    actions->addWidget(m_username, 1);
    actions->addWidget(m_password, 1);
    actions->addWidget(m_loginBtn);

    root->addLayout(actions);

    m_loginStatus = new QLabel("—");
    m_loginStatus->setObjectName("cardValue");
    root->addWidget(m_loginStatus);

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

    // Ping
    connect(m_pingBtn, &QPushButton::clicked, this, [this]{
        const QString base = normalizeBaseUrl(m_backendUrl->text());
        m_pingLatency->setText("—");
        m_serverStatus->setText("Checking…");
        m_pingTopValue->setText("… ms");
        m_api->ping(base);
    });

    connect(m_api, &ApiClient::pingFinished, this, [this](bool ok, qint64 ms, const QString&){
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

    // Login
    connect(m_loginBtn, &QPushButton::clicked, this, [this]{
        const QString base = normalizeBaseUrl(m_backendUrl->text());
        m_loginStatus->setText("…");
        m_api->login(base, m_username->text().trimmed(), m_password->text());
    });

    connect(m_api, &ApiClient::loginFinished, this,
            [this](bool success, const QString& user, const QString& token, const QString& err){
                if (!success) {
                    m_loginStatus->setText("Rejected: " + err);
                    return;
                }
                QString shortToken = token;
                if (shortToken.size() > 16) shortToken = shortToken.left(16) + "...";
                m_loginStatus->setText(QString("OK (%1) token=%2").arg(user, shortToken));
            });

    // Reboot placeholder
    connect(m_rebootBtn, &QPushButton::clicked, this, [this]{
        m_loginStatus->setText("Reboot requested (front only)");
    });

    // Dummy values download/upload (si tu as un vrai test débit plus tard, tu update ici)
    m_downloadTopValue->setText("37.40 Mbps");
    m_uploadTopValue->setText("10.20 Mbps");
}
