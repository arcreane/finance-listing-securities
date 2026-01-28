#include "Theme.h"
#include <QApplication>

void Theme::apply(QApplication &app)
{
    app.setStyleSheet(stylesheet());
}

QString Theme::stylesheet()
{
    // Un thème simple, lisible, proche de ce que tu avais (cards, boutons, inputs)
    return R"QSS(
        QWidget {
            font-family: "Segoe UI";
            font-size: 12px;
            color: #111;
            background: #f6f6f6;
        }

        QLabel#cardTitle {
            color: #444;
            font-weight: 600;
        }

        QLabel#cardTitleBold {
            color: #444;
            font-weight: 700;
            letter-spacing: 0.5px;
        }

        QLabel#cardValue {
            color: #111;
            font-size: 13px;
            font-weight: 500;
        }

        QFrame#card {
            background: white;
            border: 1px solid #e3e3e3;
            border-radius: 8px;
        }

        QLineEdit {
            background: white;
            border: 1px solid #d6d6d6;
            border-radius: 6px;
            padding: 6px 8px;
        }

        QLineEdit:focus {
            border: 1px solid #7a5cff;
        }

        QPushButton {
            background: #e9e9e9;
            border: 1px solid #cfcfcf;
            border-radius: 6px;
            padding: 6px 12px;
        }

        QPushButton:hover {
            background: #dedede;
        }

        QPushButton:pressed {
            background: #d2d2d2;
        }

        QPushButton#btnPrimary {
            background: #7a5cff;
            color: white;
            border: none;
            font-weight: 600;
        }

        QPushButton#btnPrimary:hover {
            background: #6b4fff;
        }

        QPushButton#btnPrimary:pressed {
            background: #5c40ff;
        }

        QFrame#statusDotOnline {
            background: #24c96b;
            border-radius: 5px;
        }

        QFrame#statusDotOffline {
            background: #e34b4b;
            border-radius: 5px;
        }
    )QSS";
}
