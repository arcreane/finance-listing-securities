#include "SpeedGaugeWidget.h"
#include <QPainter>
#include <QtMath>

SpeedGaugeWidget::SpeedGaugeWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(320, 220);
}

void SpeedGaugeWidget::setRange(double min, double max) {
    m_min = min;
    m_max = qMax(max, min + 1e-6);
    update();
}

void SpeedGaugeWidget::setValue(double v) {
    m_value = qBound(m_min, v, m_max);
    update();
}

void SpeedGaugeWidget::setUnit(const QString& u) {
    m_unit = u;
    update();
}

void SpeedGaugeWidget::setLabel(const QString& l) {
    m_label = l;
    update();
}

void SpeedGaugeWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Background is handled by global QSS; we draw only the gauge.
    const QRectF r = rect().adjusted(12, 12, -12, -12);

    // Center point slightly lower for semicircle gauge
    QPointF c(r.center().x(), r.bottom() - r.height()*0.08);
    const double radius = qMin(r.width()*0.42, r.height()*0.72);

    // Angles: left = 210°, right = -30°  (span 240°)
    const double startDeg = 210.0;
    const double spanDeg  = 240.0;

    // Draw arc background
    QPen bgPen(QColor("#151528"), 18, Qt::SolidLine, Qt::RoundCap);
    p.setPen(bgPen);
    QRectF arcRect(c.x()-radius, c.y()-radius, 2*radius, 2*radius);
    p.drawArc(arcRect, int((startDeg)*16), int((-spanDeg)*16));

    // Draw "active" arc (pseudo-gradient by drawing multiple arcs)
    // Colors close to your screenshot (purple -> pink)
    const int steps = 40;
    for (int i=0;i<steps;i++){
        const double t0 = double(i)/steps;
        const double t1 = double(i+1)/steps;
        const double a0 = startDeg - spanDeg*t0;
        const double a1 = startDeg - spanDeg*t1;

        QColor col;
        // lerp purple (#6a4cff) to pink-ish (#ff4fd8)
        const QColor c0("#6a4cff");
        const QColor c1("#ff4fd8");
        col.setRedF  (c0.redF()   + (c1.redF()   - c0.redF())   * t0);
        col.setGreenF(c0.greenF() + (c1.greenF() - c0.greenF()) * t0);
        col.setBlueF (c0.blueF()  + (c1.blueF()  - c0.blueF())  * t0);

        QPen pen(col, 18, Qt::SolidLine, Qt::RoundCap);
        p.setPen(pen);
        p.drawArc(arcRect, int(a0*16), int((a1-a0)*16));
    }

    // Ticks
    p.setPen(QPen(QColor("#2a2a45"), 2));
    const int majorTicks = 10;
    for (int i=0;i<=majorTicks;i++){
        const double t = double(i)/majorTicks;
        const double ang = qDegreesToRadians(startDeg - spanDeg*t);
        const QPointF p1(c.x() + (radius-6)*qCos(ang),  c.y() - (radius-6)*qSin(ang));
        const QPointF p2(c.x() + (radius-24)*qCos(ang), c.y() - (radius-24)*qSin(ang));
        p.drawLine(p1, p2);
    }

    // Needle
    const double norm = (m_value - m_min) / (m_max - m_min);
    const double needleDeg = startDeg - spanDeg * norm;
    const double ang = qDegreesToRadians(needleDeg);

    const QPointF needleEnd(c.x() + (radius-34)*qCos(ang), c.y() - (radius-34)*qSin(ang));
    p.setPen(QPen(QColor("#f5f5f5"), 3, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(c, needleEnd);

    // Needle cap
    p.setBrush(QColor("#050509"));
    p.setPen(QPen(QColor("#f5f5f5"), 2));
    p.drawEllipse(c, 8, 8);

    // Center value text
    p.setPen(QColor("#f5f5f5"));
    QFont f = font();
    f.setBold(true);
    f.setPointSize(18);
    p.setFont(f);

    const QString valueText = QString::number(int(m_value)) + " " + m_unit;
    QRectF valueRect(c.x()-140, c.y()-72, 280, 40);
    p.drawText(valueRect, Qt::AlignHCenter | Qt::AlignVCenter, valueText);

    // Label
    QFont f2 = font();
    f2.setBold(true);
    f2.setPointSize(9);
    p.setFont(f2);
    p.setPen(QColor("#7a7b85"));
    QRectF labelRect(c.x()-140, c.y()-38, 280, 24);
    p.drawText(labelRect, Qt::AlignHCenter | Qt::AlignVCenter, m_label);
}
