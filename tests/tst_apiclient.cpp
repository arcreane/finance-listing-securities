/**
 * Unit tests for ApiClient class.
 * Tests network communication and signal emission.
 */

#include <QtTest/QtTest>
#include <QSignalSpy>
#include "../src/services/ApiClient.h"

class TestApiClient : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Ping tests
    void testPingSignalEmitted();
    void testPingWithInvalidUrl();
    void testPingMeasuresLatency();

    // Download speed tests
    void testDownloadSpeedSignalEmitted();
    void testDownloadSpeedWithInvalidUrl();

    // Upload speed tests
    void testUploadSpeedSignalEmitted();
    void testUploadSpeedWithInvalidUrl();

private:
    ApiClient* m_api;
    QString m_testServerUrl;
};

void TestApiClient::initTestCase()
{
    m_api = new ApiClient(this);
    // Use localhost test server - make sure server is running for integration tests
    m_testServerUrl = "http://127.0.0.1:8080";
}

void TestApiClient::cleanupTestCase()
{
    delete m_api;
}

void TestApiClient::testPingSignalEmitted()
{
    QSignalSpy spy(m_api, &ApiClient::pingFinished);
    QVERIFY(spy.isValid());

    m_api->ping(m_testServerUrl);

    // Wait for signal (5 second timeout)
    QVERIFY(spy.wait(5000));
    QCOMPARE(spy.count(), 1);

    // Check signal parameters
    QList<QVariant> arguments = spy.takeFirst();
    bool ok = arguments.at(0).toBool();
    qint64 ms = arguments.at(1).toLongLong();

    // If server is running, should be successful
    // Note: This test requires the server to be running
    if (ok) {
        QVERIFY(ms >= 0);
        QVERIFY(ms < 10000); // Should be less than 10 seconds
    }
}

void TestApiClient::testPingWithInvalidUrl()
{
    QSignalSpy spy(m_api, &ApiClient::pingFinished);
    QVERIFY(spy.isValid());

    m_api->ping("http://invalid.nonexistent.url:9999");

    // Wait for signal (longer timeout for DNS resolution failure)
    QVERIFY(spy.wait(10000));
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    bool ok = arguments.at(0).toBool();

    // Should fail with invalid URL
    QVERIFY(!ok);
}

void TestApiClient::testPingMeasuresLatency()
{
    QSignalSpy spy(m_api, &ApiClient::pingFinished);

    m_api->ping(m_testServerUrl);

    if (spy.wait(5000) && spy.count() > 0) {
        QList<QVariant> arguments = spy.takeFirst();
        qint64 ms = arguments.at(1).toLongLong();

        // Latency should be a positive number
        QVERIFY(ms >= 0);
    }
}

void TestApiClient::testDownloadSpeedSignalEmitted()
{
    QSignalSpy spy(m_api, &ApiClient::downloadSpeedFinished);
    QVERIFY(spy.isValid());

    m_api->testDownloadSpeed(m_testServerUrl);

    // Download test may take longer
    QVERIFY(spy.wait(30000));
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    bool ok = arguments.at(0).toBool();
    double speedMbps = arguments.at(1).toDouble();

    if (ok) {
        QVERIFY(speedMbps >= 0);
    }
}

void TestApiClient::testDownloadSpeedWithInvalidUrl()
{
    QSignalSpy spy(m_api, &ApiClient::downloadSpeedFinished);

    m_api->testDownloadSpeed("http://invalid.nonexistent.url:9999");

    QVERIFY(spy.wait(10000));

    QList<QVariant> arguments = spy.takeFirst();
    bool ok = arguments.at(0).toBool();

    QVERIFY(!ok);
}

void TestApiClient::testUploadSpeedSignalEmitted()
{
    QSignalSpy spy(m_api, &ApiClient::uploadSpeedFinished);
    QVERIFY(spy.isValid());

    m_api->testUploadSpeed(m_testServerUrl);

    // Upload test may take longer
    QVERIFY(spy.wait(30000));
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    bool ok = arguments.at(0).toBool();
    double speedMbps = arguments.at(1).toDouble();

    if (ok) {
        QVERIFY(speedMbps >= 0);
    }
}

void TestApiClient::testUploadSpeedWithInvalidUrl()
{
    QSignalSpy spy(m_api, &ApiClient::uploadSpeedFinished);

    m_api->testUploadSpeed("http://invalid.nonexistent.url:9999");

    QVERIFY(spy.wait(10000));

    QList<QVariant> arguments = spy.takeFirst();
    bool ok = arguments.at(0).toBool();

    QVERIFY(!ok);
}

QTEST_MAIN(TestApiClient)
#include "tst_apiclient.moc"
