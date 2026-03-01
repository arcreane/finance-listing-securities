/**
 * Unit tests for SpeedGaugeWidget class.
 * Tests gauge value handling and rendering.
 */

#include <QtTest/QtTest>
#include <QApplication>
#include "../src/ui/SpeedGaugeWidget.h"

class TestSpeedGaugeWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Value tests
    void testDefaultValue();
    void testSetValue();
    void testValueClamping();
    void testValueClampingLow();
    void testValueClampingHigh();

    // Range tests
    void testDefaultRange();
    void testSetRange();
    void testRangeWithNegatives();

    // Unit and label tests
    void testSetUnit();
    void testSetLabel();

    // Widget tests
    void testMinimumSize();
    void testRenderWithoutCrash();

private:
    SpeedGaugeWidget* m_gauge;
};

void TestSpeedGaugeWidget::initTestCase()
{
    m_gauge = new SpeedGaugeWidget();
}

void TestSpeedGaugeWidget::cleanupTestCase()
{
    delete m_gauge;
}

void TestSpeedGaugeWidget::testDefaultValue()
{
    SpeedGaugeWidget gauge;
    QCOMPARE(gauge.value(), 0.0);
}

void TestSpeedGaugeWidget::testSetValue()
{
    m_gauge->setRange(0, 100);
    m_gauge->setValue(50);
    QCOMPARE(m_gauge->value(), 50.0);
}

void TestSpeedGaugeWidget::testValueClamping()
{
    m_gauge->setRange(0, 100);

    // Value should be clamped to range
    m_gauge->setValue(150);
    QCOMPARE(m_gauge->value(), 100.0);

    m_gauge->setValue(-50);
    QCOMPARE(m_gauge->value(), 0.0);
}

void TestSpeedGaugeWidget::testValueClampingLow()
{
    m_gauge->setRange(10, 100);
    m_gauge->setValue(5);
    QCOMPARE(m_gauge->value(), 10.0);
}

void TestSpeedGaugeWidget::testValueClampingHigh()
{
    m_gauge->setRange(0, 50);
    m_gauge->setValue(100);
    QCOMPARE(m_gauge->value(), 50.0);
}

void TestSpeedGaugeWidget::testDefaultRange()
{
    SpeedGaugeWidget gauge;
    // Default range is 0-5000 based on implementation
    gauge.setValue(2500);
    QCOMPARE(gauge.value(), 2500.0);

    // Test clamping at default max
    gauge.setValue(6000);
    QCOMPARE(gauge.value(), 5000.0);
}

void TestSpeedGaugeWidget::testSetRange()
{
    m_gauge->setRange(0, 1000);
    m_gauge->setValue(500);
    QCOMPARE(m_gauge->value(), 500.0);

    // Value beyond new range should be clamped
    m_gauge->setValue(1500);
    QCOMPARE(m_gauge->value(), 1000.0);
}

void TestSpeedGaugeWidget::testRangeWithNegatives()
{
    m_gauge->setRange(-100, 100);
    m_gauge->setValue(0);
    QCOMPARE(m_gauge->value(), 0.0);

    m_gauge->setValue(-50);
    QCOMPARE(m_gauge->value(), -50.0);
}

void TestSpeedGaugeWidget::testSetUnit()
{
    m_gauge->setUnit("ms");
    // No getter for unit, but should not crash
    QVERIFY(true);

    m_gauge->setUnit("Mbps");
    QVERIFY(true);
}

void TestSpeedGaugeWidget::testSetLabel()
{
    m_gauge->setLabel("PING");
    // No getter for label, but should not crash
    QVERIFY(true);

    m_gauge->setLabel("DOWNLOAD");
    QVERIFY(true);
}

void TestSpeedGaugeWidget::testMinimumSize()
{
    QSize minSize = m_gauge->minimumSize();
    QVERIFY(minSize.width() >= 320);
    QVERIFY(minSize.height() >= 220);
}

void TestSpeedGaugeWidget::testRenderWithoutCrash()
{
    // Test that rendering doesn't crash at various values
    m_gauge->setRange(0, 1000);

    // Test minimum value
    m_gauge->setValue(0);
    m_gauge->repaint();
    QVERIFY(true);

    // Test maximum value
    m_gauge->setValue(1000);
    m_gauge->repaint();
    QVERIFY(true);

    // Test middle value
    m_gauge->setValue(500);
    m_gauge->repaint();
    QVERIFY(true);

    // Test with different units
    m_gauge->setUnit("Mbps");
    m_gauge->setLabel("DOWNLOAD");
    m_gauge->repaint();
    QVERIFY(true);
}

QTEST_MAIN(TestSpeedGaugeWidget)
#include "tst_speedgaugewidget.moc"
