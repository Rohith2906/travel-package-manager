#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QString>
#include <vector>

using namespace std;

struct TravelPackage {
    QString destination;
    QString state;
    QString weatherCondition;
    double temperature;
    QString requiredClothing;
    double distance;
    double travelCost;
    double hotelCostPerDay;
    double foodCostPerDay;
    int availableDays;
};

class TravelPackageManager : public QWidget {
    Q_OBJECT

public:
    TravelPackageManager(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Travel Package Manager");
        resize(1000, 650);

        createPackages();
        setupUI();

        table->setRowCount(packages.size());

        for (int i = 0; i < packages.size(); ++i) {
            updateTableRow(i);
        }
    }

private slots:
    void addPackage() {
        if (destinationEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter destination name.");
            return;
        }

        TravelPackage package;
        package.destination = destinationEdit->text();
        package.state = stateEdit->text();
        package.weatherCondition = weatherBox->currentText();
        package.temperature = temperatureSpin->value();
        package.requiredClothing = clothingEdit->text();
        package.distance = distanceSpin->value();
        package.travelCost = travelCostSpin->value();
        package.hotelCostPerDay = hotelCostSpin->value();
        package.foodCostPerDay = foodCostSpin->value();
        package.availableDays = daysSpin->value();

        packages.append(package);

        int row = table->rowCount();
        table->insertRow(row);
        updateTableRow(row);

        QMessageBox::information(this, "Success", "Travel package added successfully.");

        destinationEdit->clear();
        stateEdit->clear();
        clothingEdit->clear();
    }

    void showWeatherInfo() {
        int row = table->currentRow();

        if (row < 0) {
            QMessageBox::warning(this, "Select Package", "Please select a travel package.");
            return;
        }

        const TravelPackage &package = packages[row];

        QString message;
        message += "Destination: " + package.destination + "\n";
        message += "Weather Condition: " + package.weatherCondition + "\n";
        message += "Temperature: " + QString::number(package.temperature) + " °C\n";
        message += "Required Clothing: " + package.requiredClothing;

        QMessageBox::information(this, "Weather and Clothing", message);
    }

    void calculateCost() {
        int row = table->currentRow();

        if (row < 0) {
            QMessageBox::warning(this, "Select Package", "Please select a travel package.");
            return;
        }

        int days = tripDaysSpin->value();
        int persons = personsSpin->value();
        const TravelPackage &package = packages[row];

        if (days > package.availableDays) {
            QMessageBox::warning(
                this,
                "Invalid Days",
                "This package is available only for " +
                QString::number(package.availableDays) + " days."
            );
            return;
        }

        double transportCost = package.travelCost * persons;
        double hotelCost = package.hotelCostPerDay * days * persons;
        double foodCost = package.foodCostPerDay * days * persons;
        double totalCost = transportCost + hotelCost + foodCost;

        QString result;
        result += "Destination: " + package.destination + "\n";
        result += "Days: " + QString::number(days) + "\n";
        result += "Persons: " + QString::number(persons) + "\n\n";
        result += "Transport Cost: Rs. " + QString::number(transportCost, 'f', 2) + "\n";
        result += "Hotel Cost: Rs. " + QString::number(hotelCost, 'f', 2) + "\n";
        result += "Food Cost: Rs. " + QString::number(foodCost, 'f', 2) + "\n\n";
        result += "Total Cost: Rs. " + QString::number(totalCost, 'f', 2);

        costOutput->setPlainText(result);
    }

    void searchPackage() {
        QString searchText = searchEdit->text().trimmed().toLower();

        if (searchText.isEmpty()) {
            for (int i = 0; i < table->rowCount(); ++i) {
                table->setRowHidden(i, false);
            }
            return;
        }

        for (int i = 0; i < packages.size(); ++i) {
            bool match = packages[i].destination.toLower().contains(searchText);
            table->setRowHidden(i, !match);
        }
    }

    void deletePackage() {
        int row = table->currentRow();

        if (row < 0) {
            QMessageBox::warning(this, "Select Package", "Please select a travel package.");
            return;
        }

        int reply = QMessageBox::question(
            this,
            "Delete Package",
            "Are you sure you want to delete this package?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            packages.removeAt(row);
            table->removeRow(row);
        }
    }

private:
    QVector<TravelPackage> packages;

    QTableWidget *table;
    QLineEdit *destinationEdit;
    QLineEdit *stateEdit;
    QLineEdit *clothingEdit;
    QLineEdit *searchEdit;

    QComboBox *weatherBox;

    QDoubleSpinBox *temperatureSpin;
    QDoubleSpinBox *distanceSpin;
    QDoubleSpinBox *travelCostSpin;
    QDoubleSpinBox *hotelCostSpin;
    QDoubleSpinBox *foodCostSpin;

    QSpinBox *daysSpin;
    QSpinBox *personsSpin;
    QSpinBox *tripDaysSpin;

    QTextEdit *costOutput;

    void createPackages() {
        packages.append({
            "Goa", "India", "Hot and Humid", 31.0,
            "Cotton clothes, sunglasses, hat and slippers",
            650.0, 2500.0, 1800.0, 800.0, 10
        });

        packages.append({
            "Manali", "Himachal Pradesh", "Cold and Snowy", 5.0,
            "Jacket, sweater, gloves, woollen cap and boots",
            2200.0, 7000.0, 2500.0, 1000.0, 12
        });

        packages.append({
            "Mysuru", "Karnataka", "Pleasant", 24.0,
            "Comfortable cotton clothes and walking shoes",
            180.0, 1200.0, 1200.0, 600.0, 7
        });

        packages.append({
            "Ooty", "Tamil Nadu", "Cool and Cloudy", 15.0,
            "Light jacket, jeans and comfortable shoes",
            270.0, 1800.0, 1500.0, 700.0, 8
        });

        packages.append({
            "Rajasthan", "India", "Hot and Dry", 35.0,
            "Loose cotton clothes, scarf, sunglasses and sunscreen",
            1800.0, 6000.0, 2000.0, 900.0, 10
        });
    }

    void setupUI() {
        table = new QTableWidget(this);
        table->setColumnCount(8);
        table->setHorizontalHeaderLabels({
            "Destination",
            "Weather",
            "Temperature",
            "Clothing",
            "Distance (km)",
            "Travel Cost",
            "Hotel Cost/Day",
            "Food Cost/Day"
        });

        table->horizontalHeader()->setStretchLastSection(true);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        destinationEdit = new QLineEdit;
        stateEdit = new QLineEdit;
        clothingEdit = new QLineEdit;
        searchEdit = new QLineEdit;
        searchEdit->setPlaceholderText("Search destination...");

        weatherBox = new QComboBox;
        weatherBox->addItems({
            "Hot and Humid",
            "Hot and Dry",
            "Cold and Snowy",
            "Cool and Cloudy",
            "Rainy",
            "Pleasant"
        });

        temperatureSpin = new QDoubleSpinBox;
        temperatureSpin->setRange(-50.0, 60.0);
        temperatureSpin->setValue(25.0);

        distanceSpin = new QDoubleSpinBox;
        distanceSpin->setRange(0.0, 100000.0);
        distanceSpin->setValue(100.0);

        travelCostSpin = new QDoubleSpinBox;
        travelCostSpin->setRange(0.0, 1000000.0);
        travelCostSpin->setValue(1000.0);

        hotelCostSpin = new QDoubleSpinBox;
        hotelCostSpin->setRange(0.0, 1000000.0);
        hotelCostSpin->setValue(1000.0);

        foodCostSpin = new QDoubleSpinBox;
        foodCostSpin->setRange(0.0, 1000000.0);
        foodCostSpin->setValue(500.0);

        daysSpin = new QSpinBox;
        daysSpin->setRange(1, 365);
        daysSpin->setValue(5);

        tripDaysSpin = new QSpinBox;
        tripDaysSpin->setRange(1, 365);
        tripDaysSpin->setValue(3);

        personsSpin = new QSpinBox;
        personsSpin->setRange(1, 100);
        personsSpin->setValue(2);

        costOutput = new QTextEdit;
        costOutput->setReadOnly(true);

        QPushButton *addButton = new QPushButton("Add Package");
        QPushButton *weatherButton = new QPushButton("Show Weather & Clothing");
        QPushButton *costButton = new QPushButton("Calculate Total Cost");
        QPushButton *deleteButton = new QPushButton("Delete Selected Package");
        QPushButton *searchButton = new QPushButton("Search");

        connect(addButton, &QPushButton::clicked, this, &TravelPackageManager::addPackage);
        connect(weatherButton, &QPushButton::clicked, this, &TravelPackageManager::showWeatherInfo);
        connect(costButton, &QPushButton::clicked, this, &TravelPackageManager::calculateCost);
        connect(deleteButton, &QPushButton::clicked, this, &TravelPackageManager::deletePackage);
        connect(searchButton, &QPushButton::clicked, this, &TravelPackageManager::searchPackage);
        connect(searchEdit, &QLineEdit::textChanged, this, &TravelPackageManager::searchPackage);

        QFormLayout *formLayout = new QFormLayout;
        formLayout->addRow("Destination:", destinationEdit);
        formLayout->addRow("State/Country:", stateEdit);
        formLayout->addRow("Weather:", weatherBox);
        formLayout->addRow("Temperature (°C):", temperatureSpin);
        formLayout->addRow("Required Clothing:", clothingEdit);
        formLayout->addRow("Distance (km):", distanceSpin);
        formLayout->addRow("Travel Cost per Person:", travelCostSpin);
        formLayout->addRow("Hotel Cost per Day:", hotelCostSpin);
        formLayout->addRow("Food Cost per Day:", foodCostSpin);
        formLayout->addRow("Available Days:", daysSpin);

        QGroupBox *packageGroupBox = new QGroupBox("Add New Travel Package");
        packageGroupBox->setLayout(formLayout);

        QHBoxLayout *searchLayout = new QHBoxLayout;
        searchLayout->addWidget(searchEdit);
        searchLayout->addWidget(searchButton);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(weatherButton);
        buttonLayout->addWidget(costButton);
        buttonLayout->addWidget(deleteButton);

        QFormLayout *costLayout = new QFormLayout;
        costLayout->addRow("Trip Days:", tripDaysSpin);
        costLayout->addRow("Number of Persons:", personsSpin);

        QGroupBox *costGroupBox = new QGroupBox("Cost Calculator");
        costGroupBox->setLayout(costLayout);

        QVBoxLayout *rightLayout = new QVBoxLayout;
        rightLayout->addWidget(packageGroupBox);
        rightLayout->addLayout(searchLayout);
        rightLayout->addLayout(buttonLayout);
        rightLayout->addWidget(costGroupBox);
        rightLayout->addWidget(new QLabel("Cost Estimate:"));
        rightLayout->addWidget(costOutput);

        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->addWidget(table, 3);
        mainLayout->addLayout(rightLayout, 2);

        setLayout(mainLayout);
    }

    void updateTableRow(int row) {
        const TravelPackage &package = packages[row];

        table->setItem(row, 0, new QTableWidgetItem(package.destination));
        table->setItem(row, 1, new QTableWidgetItem(package.weatherCondition));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(package.temperature) + " °C"));
        table->setItem(row, 3, new QTableWidgetItem(package.requiredClothing));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(package.distance)));
        table->setItem(row, 5, new QTableWidgetItem("Rs. " + QString::number(package.travelCost)));
        table->setItem(row, 6, new QTableWidgetItem("Rs. " + QString::number(package.hotelCostPerDay)));
        table->setItem(row, 7, new QTableWidgetItem("Rs. " + QString::number(package.foodCostPerDay)));
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TravelPackageManager window;
    window.show();

    return app.exec();
}
