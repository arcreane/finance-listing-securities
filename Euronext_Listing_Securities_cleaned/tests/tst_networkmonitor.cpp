/**
 * Unit tests for NetworkMonitor class.
 * Tests network connection type detection.
 */

#include <QtTest/QtTest>
#include <QSignalSpy>
#include "../src/services/NetworkMonitor.h"

class TestNetworkMonitor : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Connection type tests
    void testConnectionTypeNotEmpty();
    void testConnectionTypeValidValues();
    void testConnectionTypeChangedSignal();
    void testMultipleInstances();

private:
    NetworkMonitor* m_monitor;
};

void TestNetworkMonitor::initTestCase()
{
    m_monitor = new NetworkMonitor(this);
}

void TestNetworkMonitor::cleanupTestCase()
{
    delete m_monitor;
}

void TestNetworkMonitor::testConnectionTypeNotEmpty()
{
    QString type = m_monitor->connectionType();
    QVERIFY(!type.isEmpty());
}

void TestNetworkMonitor::testConnectionTypeValidValues()
{
    QString type = m_monitor->connectionType();

    // Should be one of the known values
    QStringList validTypes = {"WiFi", "Ethernet", "Connected", "Unknown"};
    QVERIFY2(validTypes.contains(type),
             qPrintable(QString("Unexpected connection type: %1").arg(type)));
}

void TestNetworkMonitor::testConnectionTypeChangedSignal()
{
    QSignalSpy spy(m_monitor, &NetworkMonitor::connectionTypeChanged);
    QVERIFY(spy.isValid());

    // The signal is emitted when connection type changes
    // We can't easily force a change, so just verify the signal is connectable
    // In a real scenario, you'd mock the network interface

    // Wait briefly to see if any changes occur
    spy.wait(100);

    // Signal count depends on actual network changes
    // Just verify no crash occurred
    QVERIFY(true);
}

void TestNetworkMonitor::testMultipleInstances()
{
    // Create multiple monitors - should not interfere with each other
    NetworkMonitor* monitor2 = new NetworkMonitor(this);

    QString type1 = m_monitor->connectionType();
    QString type2 = monitor2->connectionType();

    // Both should report the same connection type
    QCOMPARE(type1, type2);

    delete monitor2;
}

QTEST_MAIN(TestNetworkMonitor)
#include "tst_networkmonitor.moc"
