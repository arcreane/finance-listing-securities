#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDial>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>

class NetworkWindow : public QWidget
{
public:
    NetworkWindow(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("Network User");
        resize(960, 600);

        // Style global sombre
        setStyleSheet(
            "QWidget {"
            "  background-color: #050509;"
            "  color: #f5f5f5;"
            "  font-family: 'Segoe UI', 'Roboto', sans-serif;"
            "  font-size: 10pt;"
            "}"
        );

        auto *root = new QVBoxLayout(this);
        root->setContentsMargins(16, 16, 16, 16);
        root->setSpacing(12);

        // --- BARRE DU HAUT ---------------------------------------------------
        auto *topBar = new QHBoxLayout;
        auto *titleLbl = new QLabel("Network User");
        QFont titleFont = titleLbl->font();
        titleFont.setPointSize(14);
        titleFont.setBold(true);
        titleLbl->setFont(titleFont);

        auto *bellLbl = new QLabel(QString::fromUtf8("🔔"));
        auto *dot = new QLabel;
        dot->setFixedSize(10, 10);
        dot->setStyleSheet(
            "background-color: #1dd75b;"
            "border-radius: 5px;"
        );

        topBar->addWidget(titleLbl);
        topBar->addStretch();
        topBar->addWidget(bellLbl);
        topBar->addSpacing(8);
        topBar->addWidget(dot);
        root->addLayout(topBar);

        // --- CONTENU PRINCIPAL (menu gauche + panneau droit) -----------------
        auto *mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(16);

        // ------------------- MENU GAUCHE -------------------------------------
        QWidget *leftPanel = new QWidget;
        leftPanel->setFixedWidth(220);
        leftPanel->setStyleSheet(
            "background-color: #090912;"
            "border-radius: 12px;"
        );

        auto *leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setContentsMargins(20, 20, 20, 20);
        leftLayout->setSpacing(8);

        auto *manageLbl = new QLabel("MANAGE");
        manageLbl->setStyleSheet("color: #7a7b85; font-size: 9pt; font-weight: bold;");
        leftLayout->addWidget(manageLbl);

        auto addNavButton = [&](const QString &text, bool selected = false) {
            auto *btn = new QPushButton(text);
            btn->setCursor(Qt::PointingHandCursor);
            btn->setCheckable(true);
            btn->setChecked(selected);
            btn->setFlat(true);
            btn->setStyleSheet(
                "QPushButton {"
                "  text-align: left;"
                "  padding: 8px 12px;"
                "  border-radius: 8px;"
                "  border: 0px;"
                "  color: #d0d0dc;"
                "  background-color: transparent;"
                "}"
                "QPushButton:hover {"
                "  background-color: #11111b;"
                "}"
                "QPushButton:checked {"
                "  background-color: #151528;"
                "  color: white;"
                "  font-weight: bold;"
                "}"
            );
            leftLayout->addWidget(btn);
        };

        addNavButton("Balance", true);
        addNavButton("Check Online");
        addNavButton("Referrals sys");

        leftLayout->addSpacing(16);

        auto *userLbl = new QLabel("USER");
        userLbl->setStyleSheet("color: #7a7b85; font-size: 9pt; font-weight: bold;");
        leftLayout->addWidget(userLbl);

        addNavButton("Profile");
        addNavButton("Notifications");

        leftLayout->addStretch();

        mainLayout->addWidget(leftPanel);

        // ------------------- PANNEAU DROIT ----------------------------------
        QWidget *rightPanel = new QWidget;
        auto *rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->setContentsMargins(0, 0, 0, 0);
        rightLayout->setSpacing(16);

        // --- petites cartes en grille ---------------------------------------
        auto *grid = new QGridLayout;
        grid->setSpacing(12);

        auto createCard = [&](const QString &title, const QString &value) -> QFrame * {
            auto *card = new QFrame;
            card->setObjectName("card");
            card->setStyleSheet(
                "QFrame#card {"
                "  background-color: #090912;"
                "  border-radius: 12px;"
                "  border: 1px solid #161626;"
                "}"
            );
            auto *lay = new QVBoxLayout(card);
            lay->setContentsMargins(14, 10, 14, 10);

            auto *t = new QLabel(title);
            t->setStyleSheet("color: #7a7b85; font-size: 9pt;");

            auto *v = new QLabel(value);
            QFont f = v->font();
            f.setPointSize(12);
            f.setBold(true);
            v->setFont(f);

            lay->addWidget(t);
            lay->addSpacing(6);
            lay->addWidget(v);
            lay->addStretch();
            return card;
        };

        grid->addWidget(createCard("Server", "AWS"),        0, 0);
        grid->addWidget(createCard("Proxy", "Disabled"),    0, 1);
        grid->addWidget(createCard("Connection Type", "Ethernet"), 1, 0);
        grid->addWidget(createCard("Latency", "0.01 ms"),   1, 1);
        grid->addWidget(createCard("Proxy", ""),            2, 0);
        grid->addWidget(createCard("VPN", "Disconnected"),  2, 1);

        rightLayout->addLayout(grid, /*stretch*/0);

        // --- carte Throughput Test ------------------------------------------
        auto *throughputCard = new QFrame;
        throughputCard->setObjectName("card");
        throughputCard->setStyleSheet(
            "QFrame#card {"
            "  background-color: #090912;"
            "  border-radius: 12px;"
            "  border: 1px solid #161626;"
            "}"
        );
        auto *tpLayout = new QHBoxLayout(throughputCard);
        tpLayout->setContentsMargins(14, 10, 14, 10);
        tpLayout->setSpacing(24);

        // Gauche : titre + jauge (QDial)
        auto *tpLeft = new QVBoxLayout;
        auto *tpTitle = new QLabel("THROUGHPUT TEST");
        tpTitle->setStyleSheet("color: #7a7b85; font-size: 9pt; font-weight: bold;");

        auto *dial = new QDial;
        dial->setMinimum(0);
        dial->setMaximum(100);
        dial->setValue(40);
        dial->setNotchesVisible(false);
        dial->setFixedSize(140, 140);
        dial->setStyleSheet(
            "QDial { background: transparent; }"
            "QDial::handle {"
            "  background: #6a4cff;"
            "  border-radius: 6px;"
            "  width: 12px;"
            "  height: 12px;"
            "}"
        );

        tpLeft->addWidget(tpTitle);
        tpLeft->addWidget(dial, 0, Qt::AlignHCenter);

        tpLayout->addLayout(tpLeft);

        // Droite : débits download / upload
        auto *tpRight = new QVBoxLayout;

        auto addSpeedRow = [&](const QString &label, const QString &value) {
            auto *row = new QHBoxLayout;

            auto *icon = new QPushButton;
            icon->setEnabled(false);
            icon->setFixedSize(28, 28);
            icon->setStyleSheet(
                "QPushButton {"
                "  border-radius: 8px;"
                "  border: 0px;"
                "  background-color: #151528;"
                "}"
            );

            auto *txt = new QLabel(value + "   " + label);
            txt->setStyleSheet("font-size: 10pt;");

            row->addWidget(icon);
            row->addSpacing(8);
            row->addWidget(txt);
            row->addStretch();

            tpRight->addLayout(row);
        };

        tpRight->addStretch();
        addSpeedRow("Downloaded", "24.57 Mb");
        addSpeedRow("Uploaded",   "45.83 Kb");
        tpRight->addStretch();

        tpLayout->addLayout(tpRight);

        rightLayout->addWidget(throughputCard, 1);

        // --- bouton Reboot ---------------------------------------------------
        auto *rebootBtn = new QPushButton("Reboot Connection");
        rebootBtn->setFixedHeight(44);
        rebootBtn->setCursor(Qt::PointingHandCursor);
        rebootBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #6a4cff;"
            "  color: white;"
            "  border-radius: 12px;"
            "  border: 0px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "  background-color: #7f5dff;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #5b3ee6;"
            "}"
        );

        rightLayout->addWidget(rebootBtn);

        mainLayout->addWidget(rightPanel, 1);

        root->addLayout(mainLayout, 1);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    NetworkWindow w;
    w.show();
    return app.exec();
}
