#include "room_list_window.hpp"
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateEdit>
#include <QTimeEdit>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QComboBox>
#include <QDebug>
#include "ui_room_list_window.h"

namespace roomsched::roomlistwindow {

room_list_window::room_list_window(QWidget *parent)
    : QWidget(parent), ui(new Ui::room_list_window) {
    ui->setupUi(this);
}

room_list_window::room_list_window(
    roomsched::client::ApiClient *existingApi,
    QString userName,
    QString userEmail,
    QString userPhone,
    QWidget *parent,
    const QString &initialBuilding
)
    : QWidget(parent), ui(new Ui::room_list_window), 
    api(existingApi), initialBuildingName(initialBuilding) {
    ui->setupUi(this);
    resizeTimer = new QTimer(this);
    resizeTimer->setSingleShot(true);
    connect(resizeTimer, &QTimer::timeout, this, &room_list_window::updateGrid);
    connect(api, &roomsched::client::ApiClient::roomsLoaded, this, &room_list_window::onRoomsLoaded);
    connect(api, &roomsched::client::ApiClient::buildingsLoaded, this, &room_list_window::onBuildingsLoaded);
    connect(api, &roomsched::client::ApiClient::bookingFinished, this, [this](bool success, QString message) {
        if (success) {
            QMessageBox::information(this, "Успех", "Комната успешно забронирована!");
            api->getRooms(); 
        } else {
            QMessageBox::warning(this, "Ошибка бронирования", message);
        }
    });
    connect(ui->buildingCombo, &QComboBox::currentIndexChanged, this, [this](int) {
        applyBuildingFilter();
    });
    api->getBuildings();
    api->getRooms();
}

room_list_window::~room_list_window() {
    delete ui;
}

void room_list_window::showRoomDetails(const QJsonObject &room) {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Бронирование аудитории");
    dialog->setMinimumSize(360, 420);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel(QString("Аудитория №%1").arg(room["room_number"].toString()), dialog);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #423358;");
    mainLayout->addWidget(titleLabel);

    QLabel *infoLabel = new QLabel(QString("Корпус: %1\nВместимость: %2 человек")
                                   .arg(room["building"].toString())
                                   .arg(room["capacity"].toInt()), dialog);
    mainLayout->addWidget(infoLabel);

    mainLayout->addWidget(new QLabel("Выберите дату:", dialog));
    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), dialog);
    dateEdit->setCalendarPopup(true);
    mainLayout->addWidget(dateEdit);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    
    QVBoxLayout *startLayout = new QVBoxLayout();
    startLayout->addWidget(new QLabel("Начало:", dialog));
    QTimeEdit *startTime = new QTimeEdit(QTime::currentTime(), dialog);
    startLayout->addWidget(startTime);
    
    QVBoxLayout *endLayout = new QVBoxLayout();
    endLayout->addWidget(new QLabel("Окончание:", dialog));
    QTimeEdit *endTime = new QTimeEdit(QTime::currentTime().addSecs(3600), dialog);
    endLayout->addWidget(endTime);

    timeLayout->addLayout(startLayout);
    timeLayout->addLayout(endLayout);
    mainLayout->addLayout(timeLayout);

    QPushButton *confirmBtn = new QPushButton("Забронировать", dialog);
    confirmBtn->setObjectName("loginButton"); 
    mainLayout->addWidget(confirmBtn);

    connect(confirmBtn, &QPushButton::clicked, [this, room, dateEdit, startTime, endTime, dialog]() {
        QString date = dateEdit->date().toString("yyyy-MM-dd");
        QString start = startTime->time().toString("HH:mm:ss");
        QString end = endTime->time().toString("HH:mm:ss");

        if (startTime->time() >= endTime->time()) {
            QMessageBox::warning(dialog, "Ошибка", "Время начала должно быть меньше времени окончания.");
            return;
        }

        api->bookRoom(room["id"].toInt(), date, start, end);
        dialog->accept();
    });

    dialog->exec();
}

void room_list_window::onRoomsLoaded(const QJsonArray &roomsArray) {
    allRooms = roomsArray;
    if (!initialBuildingName.isEmpty()) {
        int idx = ui->buildingCombo->findText(initialBuildingName);
        if (idx >= 0) {
            ui->buildingCombo->setCurrentIndex(idx);
        }
        initialBuildingName = "";
    }
    applyBuildingFilter();
}

void room_list_window::onBuildingsLoaded(const QJsonArray &buildingsArray) {
    ui->buildingCombo->clear();
    ui->buildingCombo->addItem("Все корпуса", -1);

    for (const auto &value : buildingsArray) {
        QJsonObject building = value.toObject();
        ui->buildingCombo->addItem(
            building["name"].toString(),
            building["id"].toInt()
        );
    }

    if (!initialBuildingName.isEmpty()) {
        int idx = ui->buildingCombo->findText(initialBuildingName);
        if (idx >= 0) {
            ui->buildingCombo->setCurrentIndex(idx);
        }
    }
}

void room_list_window::applyBuildingFilter() {
    int buildingId = ui->buildingCombo->currentData().toInt();
    QJsonArray filtered;
    if (allRooms.isEmpty()) {
        return; 
    }
    if (buildingId <= 0) {
        filtered = allRooms;
    } else {
        for (const auto &value : allRooms) {
            QJsonObject room = value.toObject();
            if (room["building"].toString() == ui->buildingCombo->currentText()) {
                filtered.append(room);
            }
        }
    }
    this->rooms = filtered;
    renderRooms(rooms);
}

void room_list_window::updateViewForBuilding(const QString &buildingName) {
    initialBuildingName = buildingName;
    if (!allRooms.isEmpty()) {
        int idx = ui->buildingCombo->findText(buildingName);
        if (idx >= 0) ui->buildingCombo->setCurrentIndex(idx);
        applyBuildingFilter();
    }
}

void room_list_window::renderRooms(const QJsonArray &roomsArray) {
    qDeleteAll(buttons);
    buttons.clear();
    for (const QJsonValue &value : roomsArray) {
        QJsonObject room = value.toObject();
        QString label = QString("Аудитория %1\n%2")
                            .arg(room["room_number"].toString())
                            .arg(room["building"].toString());
                        
        QPushButton *btn = new QPushButton(label, this);
        btn->setProperty("class", "RoomButton"); 
        btn->setMinimumSize(220, 140);  
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);    
        
        connect(btn, &QPushButton::clicked, [this, room]() {
            showRoomDetails(room);
        });
        buttons.append(btn);
    }
    updateGrid();
}

void room_list_window::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (!isVisible() || buttons.isEmpty()) {
        return;
    }
    if (!rooms.isEmpty()) {
        resizeTimer->start(50);
    }
}

void room_list_window::updateGrid() {
    if (buttons.isEmpty()) return;
    int windowWidth = ui->scrollArea->width();
    if (windowWidth < 100) return;
    QLayoutItem *item;
    while ((item = ui->gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->hide(); 
            item->widget()->setParent(nullptr);
        }
        delete item;
    }

    int buttonWidth = 220;
    int spacing = 15;
    int max_columns = qMax(1, windowWidth / (buttonWidth + spacing));
    int row = 0, col = 0;
    for (QPushButton *btn : buttons) {
        if (!btn) continue;
        btn->show();
        ui->gridLayout->addWidget(btn, row, col, Qt::AlignLeft | Qt::AlignTop);
        col++;
        if (col >= max_columns) {
            col = 0;
            row++;
        }
    }
    ui->gridLayout->setColumnStretch(max_columns, 1);
}

void room_list_window::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    updateGrid();
}

}  // namespace roomsched::roomlistwindow
