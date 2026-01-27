#include "Theme.h"

namespace Theme {

QString qss() {
    return QStringLiteral(R"QSS(
/* ===== Base (ancien main.cpp) ===== */
QWidget {
  background-color: #050509;
  color: #f5f5f5;
  font-family: 'Segoe UI', 'Roboto', sans-serif;
  font-size: 10pt;
}

/* ===== Sidebar (leftPanel) ===== */
QWidget#leftPanel {
  background-color: #090912;
  border-radius: 12px;
}

/* Section labels "MANAGE", "USER" */
QLabel#sectionLabel {
  color: #7a7b85;
  font-size: 9pt;
  font-weight: bold;
}

/* Nav buttons (addNavButton) */
QPushButton#navBtn {
  text-align: left;
  padding: 8px 12px;
  border-radius: 8px;
  border: 0px;
  color: #d0d0dc;
  background-color: transparent;
}
QPushButton#navBtn:hover {
  background-color: #11111b;
}
QPushButton#navBtn:checked {
  background-color: #151528;
  color: white;
  font-weight: bold;
}

/* ===== Cards (createCard + throughputCard) ===== */
QFrame#card {
  background-color: #090912;
  border-radius: 12px;
  border: 1px solid #161626;
}

QLabel#cardTitle {
  color: #7a7b85;
  font-size: 9pt;
}
QLabel#cardTitleBold {
  color: #7a7b85;
  font-size: 9pt;
  font-weight: bold;
}

QLabel#cardValue {
  font-size: 12pt;
  font-weight: bold;
  color: #f5f5f5;
}

/* ===== Inputs ===== */
QLineEdit {
  background-color: #050509; /* dans ton ancien screenshot, champ très sombre */
  color: #f5f5f5;
  border: 1px solid #161626;
  border-radius: 8px;
  padding: 6px 8px;
}
QLineEdit:focus {
  border: 1px solid #6a4cff;
}

/* ===== Small square icon in throughput rows ===== */
QPushButton#tpIcon {
  border-radius: 8px;
  border: 0px;
  background-color: #151528;
}

/* ===== Primary violet button (Reboot Connection) ===== */
QPushButton#btnPrimary {
  background-color: #6a4cff;
  color: white;
  border-radius: 12px;
  border: 0px;
  font-weight: bold;
  padding: 10px 14px;
}
QPushButton#btnPrimary:hover { background-color: #7f5dff; }
QPushButton#btnPrimary:pressed { background-color: #5b3ee6; }

/* ===== Status dot ===== */
QWidget#statusDotOnline {
  background-color: #1dd75b;
  border-radius: 5px;
}
QWidget#statusDotOffline {
  background-color: #7a7b85;
  border-radius: 5px;
}

/* ===== Dial style (ancien throughput) ===== */
QDial {
  background: transparent;
}
QDial::handle {
  background: #6a4cff;
  border-radius: 6px;
  width: 12px;
  height: 12px;
}
QLabel#cardTitleBold {
  color: #7a7b85;
  font-size: 9pt;
  font-weight: bold;
}
)QSS");
}

} // namespace Theme
