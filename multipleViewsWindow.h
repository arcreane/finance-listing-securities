#ifndef MULTIPLEVIEWSWINDOW_H
#define MULTIPLEVIEWSWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QtCore>
#include <QtGui>
#include <QtCharts>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTableWidget>
#include <QLCDNumber>
#include <Order.h>
#include <SearchableTableWidget.h>
#include "Alert.h"

class MultipleViewsWindow : public QMainWindow
{
    Q_OBJECT

public:
    MultipleViewsWindow(QWidget* parent = nullptr);
    ~MultipleViewsWindow();

	//All the methods to update the different components are in public because they need to be accessed by the observer
    void updatePriceChart(const time_t& time, const double& price);
    void updateOrderBook(const Order& order);
    void updateRawDataOrder(const Order& order);
    void addAlert(const Alert& alert);
    void updateAlertLabelsPositions();

private slots:
    void changePage(int index);

private:
    void createSideMenu();
    void createPages();

    void createPriceChart();
	void createOrderBookDisplay();

    // Basic main window components
    QWidget* centralWidget;
    QHBoxLayout* mainLayout;
    QWidget* sideMenu;
    QVBoxLayout* menuLayout;
    QStackedWidget* stackedWidget;

	// price chart components
    QChart* m_priceChart;
    QLineSeries* m_priceSeries;
    QChartView* m_chartView;

    QScatterSeries* m_alertSeries;
    QList<QGraphicsSimpleTextItem*> m_alertLabels;

	// Order book component
    QTableWidget* m_orderBookTable;

	//Raw data component
	SearchableTableWidget* searchTable;
};

#endif