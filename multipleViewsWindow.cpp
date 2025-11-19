#include "multipleViewsWindow.h"
#include <Alert.h>

// creation of the window
MultipleViewsWindow::MultipleViewsWindow(QWidget* parent)
    : QMainWindow(parent), 
    m_priceChart(new QChart()),
    m_priceSeries(new QLineSeries()),
    m_alertSeries(new QScatterSeries()),
    m_chartView(new QChartView(m_priceChart)),
	m_orderBookTable(new QTableWidget(10, 4, this))
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);

    searchTable = new SearchableTableWidget(this);

    //Test, to refactor
	createPriceChart();
    createOrderBookDisplay();
    
    createSideMenu();

    stackedWidget = new QStackedWidget;
    createPages();

    mainLayout->addWidget(sideMenu);
    mainLayout->addWidget(stackedWidget);

    setMinimumSize(1280, 720);

    connect(m_priceChart, &QChart::plotAreaChanged, this, &MultipleViewsWindow::updateAlertLabelsPositions);
}

//Destructor
MultipleViewsWindow::~MultipleViewsWindow()
{
}

// creation of the side menu to switch between pages
void MultipleViewsWindow::createSideMenu()
{
    sideMenu = new QWidget;
    menuLayout = new QVBoxLayout(sideMenu);

    // Add of buttons
    QPushButton* page1Button = new QPushButton("Executed scan");
    QPushButton* page2Button = new QPushButton("Order book scan");
    QPushButton* page3Button = new QPushButton("Raw Data");

	// Set side menu properties
    sideMenu->setMaximumWidth(200);
    sideMenu->setStyleSheet("background-color: #f0f0f0;");

	//Add buttons to the menu layout
    menuLayout->addWidget(page1Button);
    menuLayout->addWidget(page2Button);
    menuLayout->addWidget(page3Button);
	menuLayout->addStretch(); // Add a stretch to push the buttons to the top

	//Connect the buttons to the changePage function to put a real action in place
    connect(page1Button, &QPushButton::clicked, [this]() { changePage(0); });
    connect(page2Button, &QPushButton::clicked, [this]() { changePage(1); });
    connect(page3Button, &QPushButton::clicked, [this]() { changePage(2); });
}

// creation of the three pages with QWidget
void MultipleViewsWindow::createPages()
{
    QWidget* page1 = new QWidget;
    QWidget* page2 = new QWidget;
    QWidget* page3 = new QWidget;

	//Page 1 : executed data
    QVBoxLayout* layout1 = new QVBoxLayout(page1);
    layout1->addWidget(new QLabel("Chart on executed data"));
	layout1->addWidget(m_chartView);

    QVBoxLayout* layout2 = new QVBoxLayout(page2);
    layout2->addWidget(new QLabel("Chart on pretrade data"));
	layout2->addWidget(m_orderBookTable);

    
    QVBoxLayout* layout3 = new QVBoxLayout(page3);
    layout3->addWidget(new QLabel("Raw data search"));
	layout3->addWidget(searchTable);

	//Add the pages to the stacked widget
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);
    stackedWidget->addWidget(page3);
}

// change the stack widget index, which allows to switch pages
void MultipleViewsWindow::changePage(int index)
{
    stackedWidget->setCurrentIndex(index);
}

// Create the price chart (1st view = Executed scan, should be trade data normally but actually its orders since we don't have trade data)
void MultipleViewsWindow::createPriceChart()
{
    m_priceSeries->setName("Price");
    m_priceChart->addSeries(m_priceSeries);

    m_alertSeries->setName("Alerts");
    m_alertSeries->setMarkerSize(10);
    m_alertSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_alertSeries->setColor(Qt::red);
    m_priceChart->addSeries(m_alertSeries);

    m_priceChart->legend()->hide();

    // Special axis for time
    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setTickCount(5);
    axisX->setFormat("HH:mm:ss");
    axisX->setTitleText("Time");

    QValueAxis* axisY = new QValueAxis();
    axisY->setLabelFormat("%.2f");

	//attach the axis to the series
    m_priceChart->addAxis(axisX, Qt::AlignBottom);
    m_priceChart->addAxis(axisY, Qt::AlignLeft);
    m_priceSeries->attachAxis(axisX);
    m_priceSeries->attachAxis(axisY);

    m_priceChart->addAxis(axisX, Qt::AlignBottom);
    m_priceChart->addAxis(axisY, Qt::AlignLeft);
    m_alertSeries->attachAxis(axisX);
    m_alertSeries->attachAxis(axisY);


    m_priceChart->setTitle("Price Evolution");
    m_chartView->setRenderHint(QPainter::Antialiasing);
}

// Create the order book display
void MultipleViewsWindow::createOrderBookDisplay()
{
    m_orderBookTable->setHorizontalHeaderLabels({
        "Bid Size", "Bid Price", "Ask Price", "Ask Size"
    });
}

void MultipleViewsWindow::updatePriceChart(const time_t& time, const double& price)
{
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(time);
    qint64 msTimestamp = dateTime.toMSecsSinceEpoch();

	// Add point to the price series
    m_priceSeries->append(msTimestamp, price);

    // Adjust axes
    m_priceChart->axes(Qt::Horizontal).first()->setRange(
        QDateTime::fromMSecsSinceEpoch(m_priceSeries->at(0).x()),
        QDateTime::fromMSecsSinceEpoch(m_priceSeries->at(m_priceSeries->count() - 1).x())
    );

    QValueAxis* axisY = qobject_cast<QValueAxis*>(m_priceChart->axes(Qt::Vertical).first());
    if (axisY) {
        double minPrice = price;
        double maxPrice = price;
        for (int i = 0; i < m_priceSeries->count(); ++i) {
            double y = m_priceSeries->at(i).y();
            if (y < minPrice) minPrice = y;
            if (y > maxPrice) maxPrice = y;
        }

        double margin = (maxPrice - minPrice) * 0.1;
		if (margin < 0.1) margin = 0.1; // minial margin of 0.1

        axisY->setRange(minPrice - margin, maxPrice + margin);
    }

    // Refresh
    m_chartView->update();
}

// Add an alert to the chart
void MultipleViewsWindow::addAlert(const Alert& alert)
{
    // Convertir time_t en QDateTime
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(alert.getTimestamp());
    qint64 msTimestamp = dateTime.toMSecsSinceEpoch();

    //Color picking
    QColor alertColor;
    switch (alert.getSeverity()) {
    case AlertSeverity::LOW:
        alertColor = QColor(255, 165, 0); // Orange
        break;
    case AlertSeverity::MEDIUM:
        alertColor = QColor(255, 0, 0);   // Rouge
        break;
    case AlertSeverity::HIGH:
        alertColor = QColor(128, 0, 128); // Violet
        break;
    default:
        alertColor = QColor(255, 0, 0);   // Rouge par défaut
    }
	// Add the alert to the alert series
	m_alertSeries->append(msTimestamp, 0.5);  // 0.5 but we could also retrieve the orderId and put the alert at the price level of the order

    // 
    QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem(m_priceChart);
    textItem->setText(QString::fromStdString(alert.getDescription()));
    textItem->setBrush(alertColor);
    textItem->setFont(QFont("Arial", 9, QFont::Bold));

    QPointF pointPos = m_priceChart->mapToPosition(QPointF(msTimestamp, 0.5), m_alertSeries);
    textItem->setPos(pointPos.x() - textItem->boundingRect().width() / 2,
        pointPos.y() - 25);  // 25 pixels au-dessus du point

    // Stocker la référence pour pouvoir le mettre à jour ou le supprimer plus tard
    m_alertLabels.append(textItem);
}

// To change the label position when the chart receive a new point
void MultipleViewsWindow::updateAlertLabelsPositions()
{
    // Security to avoid memory violation if the alert series or labels are empty
    if (m_alertSeries == nullptr || m_alertLabels.isEmpty()) {
        return;
    }
    for (int i = 0; i < m_alertSeries->count() && i < m_alertLabels.size(); ++i) {
        QPointF point = m_alertSeries->at(i);
        QPointF pointPos = m_priceChart->mapToPosition(point, m_alertSeries);
        m_alertLabels[i]->setPos(pointPos.x() - m_alertLabels[i]->boundingRect().width() / 2, pointPos.y() - 25);
    }
}

void MultipleViewsWindow::updateOrderBook(const Order& order)
{
	if (order.getSide() == side::BUY) {
		m_orderBookTable->setItem(0, 0, new QTableWidgetItem(QString::number(order.getQuantity())));
		m_orderBookTable->setItem(0, 1, new QTableWidgetItem(QString::number(order.getPrice())));
	}
	else {
		m_orderBookTable->setItem(0, 2, new QTableWidgetItem(QString::number(order.getPrice())));
		m_orderBookTable->setItem(0, 3, new QTableWidgetItem(QString::number(order.getQuantity())));
	}
}


void MultipleViewsWindow::updateRawDataOrder(const Order& order)
{
    MultipleViewsWindow::searchTable->addOrder(
        QString::number(order.getId()), 
        QDateTime::fromSecsSinceEpoch(order.getTimestampCreated()).toString("yyyy-MM-dd HH:mm:ss"), 
        QString::fromStdString(order.getMarketId()), 
        QString::fromStdString(order.getOrderType() == orderType::LIMIT ? "LIMIT" : "MARKET"), 
        order.getQuantity(), 
        order.getPrice()
    );
}

