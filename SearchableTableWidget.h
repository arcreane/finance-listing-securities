#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QVector>
#include <QString>

enum class DataType {
    TRADES,
    ORDERS,
    FIRMS
};

class SearchableTableWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchableTableWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        setupConnections();
    }

	// Add trade to the table
    void addTrade(const QString& id, const QString& date, const QString& symbol,
        int quantity, double price) {
        if (static_cast<DataType>(dataTypeSelector->currentData().toInt()) != DataType::TRADES) {
            dataTypeSelector->setCurrentIndex(dataTypeSelector->findData(static_cast<int>(DataType::TRADES)));
        }

        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(id));
        table->setItem(row, 1, new QTableWidgetItem(date));
        table->setItem(row, 2, new QTableWidgetItem(symbol));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(quantity)));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(price, 'f', 2)));
    }

	//Add order to the table
    void addOrder(const QString& id, const QString& date, const QString& symbol,
        const QString& type, int quantity, double price) {
        if (static_cast<DataType>(dataTypeSelector->currentData().toInt()) != DataType::ORDERS) {
            dataTypeSelector->setCurrentIndex(dataTypeSelector->findData(static_cast<int>(DataType::ORDERS)));
        }

        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(id));
        table->setItem(row, 1, new QTableWidgetItem(date));
        table->setItem(row, 2, new QTableWidgetItem(symbol));
        table->setItem(row, 3, new QTableWidgetItem(type));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(quantity)));
        table->setItem(row, 5, new QTableWidgetItem(QString::number(price, 'f', 2)));
    }

	//Add firm to the table
    void addFirm(const QString& id, const QString& name, const QString& status,
        const QString& authDate) {
        if (static_cast<DataType>(dataTypeSelector->currentData().toInt()) != DataType::FIRMS) {
            dataTypeSelector->setCurrentIndex(dataTypeSelector->findData(static_cast<int>(DataType::FIRMS)));
        }

        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(id));
        table->setItem(row, 1, new QTableWidgetItem(name));
        table->setItem(row, 2, new QTableWidgetItem(status));
        table->setItem(row, 3, new QTableWidgetItem(authDate));
    }

	//clear all the datas in the table
    void clearAllData() {
        table->setRowCount(0);
    }

private:
    QTableWidget* table;
    QComboBox* dataTypeSelector;
    QVector<QLineEdit*> searchFields;
    QVBoxLayout* mainLayout;

    void setupUI() {
        mainLayout = new QVBoxLayout(this);

        dataTypeSelector = new QComboBox(this);
        dataTypeSelector->addItem("Trades passés", static_cast<int>(DataType::TRADES));
        dataTypeSelector->addItem("Ordres passés", static_cast<int>(DataType::ORDERS));
        dataTypeSelector->addItem("Firmes autorisées", static_cast<int>(DataType::FIRMS));
        mainLayout->addWidget(dataTypeSelector);

        table = new QTableWidget(this);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSortingEnabled(true);
        mainLayout->addWidget(table);

        setLayout(mainLayout);
    }

    void setupConnections() {
        connect(dataTypeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SearchableTableWidget::onDataTypeChanged);
    }

    void loadTradesData() {
        table->clear();
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({ "ID", "Date", "Symbol", "Quantité", "Prix" });
        setupSearchFields({ "ID", "Date", "Ticker", "Quantity", "Price" });
        table->setRowCount(0);
    }

    void loadOrdersData() {
        table->clear();
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels({ "ID", "Date", "Symbol", "Type", "Quantité", "Prix" });
        setupSearchFields({ "ID", "Date", "Ticker", "Type", "Quantity", "Price" });
        table->setRowCount(0);
    }

    void loadFirmsData() {
        table->clear();
        table->setColumnCount(4);
        table->setHorizontalHeaderLabels({ "ID", "Nom", "Statut", "Date d'autorisation" });
        setupSearchFields({ "ID", "Nom", "Statut", "Date d'autorisation" });
        table->setRowCount(0);
    }

    void setupSearchFields(const QStringList& headers) {
        for (auto* field : searchFields) {
            delete field;
        }
        searchFields.clear();

        QWidget* searchWidget = new QWidget(this);
        QHBoxLayout* searchLayout = new QHBoxLayout(searchWidget);

        for (const QString& header : headers) {
            QLineEdit* searchField = new QLineEdit(this);
            searchField->setPlaceholderText("Rechercher " + header);
            searchFields.append(searchField);
            searchLayout->addWidget(searchField);

            connect(searchField, &QLineEdit::textChanged,
                this, &SearchableTableWidget::filterTable);
        }

        mainLayout->insertWidget(1, searchWidget);
    }

private slots:
    void onDataTypeChanged(int index) {
        DataType type = static_cast<DataType>(dataTypeSelector->itemData(index).toInt());
        switch (type) {
        case DataType::TRADES:
            loadTradesData();
            break;
        case DataType::ORDERS:
            loadOrdersData();
            break;
        case DataType::FIRMS:
            loadFirmsData();
            break;
        }
    }

    void filterTable() {
        for (int row = 0; row < table->rowCount(); ++row) {
            bool rowMatch = true;

            for (int col = 0; col < table->columnCount(); ++col) {
                QString searchText = searchFields[col]->text().toLower();
                if (!searchText.isEmpty()) {
                    QString cellText = table->item(row, col)->text().toLower();
                    if (!cellText.contains(searchText)) {
                        rowMatch = false;
                        break;
                    }
                }
            }
            table->setRowHidden(row, !rowMatch);
        }
    }
};