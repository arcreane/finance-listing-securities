#pragma once
#include <QWidget>

class SpeedGaugeWidget : public QWidget {
    Q_OBJECT
public:
    explicit SpeedGaugeWidget(QWidget* parent=nullptr);

    void setRange(double min, double max);
    void setValue(double v);
    double value() const { return m_value; }

    void setUnit(const QString& u);
    void setLabel(const QString& l);

protected:
    void paintEvent(QPaintEvent* e) override;

private:
    double m_min = 0.0;
    double m_max = 500.0;
    double m_value = 0.0;
    QString m_unit = "ms";
    QString m_label = "PING";
};
