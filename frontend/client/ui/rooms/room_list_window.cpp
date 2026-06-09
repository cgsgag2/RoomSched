#include "room_list_window.hpp"
#include <QComboBox>
#include <QDateEdit>
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QTimeEdit>
#include <QUrl>
#include <QVBoxLayout>
#include "ui_room_list_window.h"

namespace {

const QTime kDayStart(8, 0);
const QTime kDayEnd(23, 59);

void clampTimeRange(QTimeEdit *startEdit, QTimeEdit *endEdit) {
    if (!startEdit || !endEdit) {
        return;
    }
    if (startEdit->time() < kDayStart) {
        startEdit->setTime(kDayStart);
    }
    if (endEdit->time() > kDayEnd) {
        endEdit->setTime(kDayEnd);
    }
    if (endEdit->time() <= startEdit->time()) {
        QTime next = startEdit->time().addSecs(3600);
        if (next > kDayEnd) {
            next = kDayEnd;
        }
        endEdit->setTime(next);
    }
}

void disablePastDates(QDateEdit *dateEdit) {
    if (!dateEdit) {
        return;
    }
    dateEdit->setMinimumDate(QDate::currentDate());
}

}  // namespace

namespace roomsched::client::roomlistwindow {

QString roomTypeLabel(const QString &type) {
    if (type == "lecture") {
        return "Лекционная";
    }
    if (type == "seminar") {
        return "Семинарная";
    }
    if (type == "coworking") {
        return "Коворкинг";
    }
    if (type == "private_office") {
        return "Офис";
    }
    return "Комната";
}

room_list_window::room_list_window(QWidget *parent)
    : QWidget(parent), ui(new Ui::room_list_window) {
    ui->setupUi(this);
}

room_list_window::room_list_window(
    ApiClient *existingApi,
    QString userName,
    QString userEmail,
    QString userPhone,
    QWidget *parent,
    const QString &initialBuilding
)
    : QWidget(parent),
      ui(new Ui::room_list_window),
      api(existingApi),
      initialBuildingName(initialBuilding) {
    ui->setupUi(this);
    resizeTimer = new QTimer(this);
    resizeTimer->setSingleShot(true);
    filterTimer = new QTimer(this);
    filterTimer->setSingleShot(true);
    filterTimer->setInterval(250);
    connect(
        filterTimer, &QTimer::timeout, this, &room_list_window::applyFilters
    );
    connect(
        api, &ApiClient::roomsLoaded, this, &room_list_window::onRoomsLoaded
    );
    connect(
        api, &ApiClient::buildingsLoaded, this,
        &room_list_window::onBuildingsLoaded
    );
    connect(
        api, &ApiClient::bookingFinished, this,
        [this](bool success, QString message) {
            if (success) {
                QMessageBox::information(
                    this, "Успех", "Комната успешно забронирована!"
                );
                applyFilters();
            } else {
                if (message == "Некорректный интервал времени.") {
                    message =
                        "Комната уже забронирована на это время! Выберите "
                        "другое.";
                }
                QMessageBox::warning(this, "Ошибка бронирования", message);
            }
        }
    );
    connect(
        ui->buildingCombo, &QComboBox::currentIndexChanged, this,
        [this](int) { scheduleApplyFilters(); }
    );
    connect(ui->typeCombo, &QComboBox::currentIndexChanged, this, [this](int) {
        scheduleApplyFilters();
    });
    connect(
        ui->capacityMinSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
        [this](int) { scheduleApplyFilters(); }
    );
    connect(
        ui->capacityMaxSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
        [this](int) { scheduleApplyFilters(); }
    );
    connect(ui->projectorCheck, &QCheckBox::stateChanged, this, [this](int) {
        scheduleApplyFilters();
    });
    connect(ui->whiteboardCheck, &QCheckBox::stateChanged, this, [this](int) {
        scheduleApplyFilters();
    });
    connect(ui->wifiCheck, &QCheckBox::stateChanged, this, [this](int) {
        scheduleApplyFilters();
    });
    connect(ui->printersCheck, &QCheckBox::stateChanged, this, [this](int) {
        scheduleApplyFilters();
    });
    if (ui->typeCombo->count() == 0) {
        ui->typeCombo->addItem("Все типы", "");
        ui->typeCombo->addItem("Лекционная", "lecture");
        ui->typeCombo->addItem("Семинар", "seminar");
        ui->typeCombo->addItem("Коворкинг", "coworking");
        ui->typeCombo->addItem("Офис", "private_office");
    }
    api->getBuildings();
    applyFilters();
}

room_list_window::~room_list_window() {
    delete ui;
}

void room_list_window::showRoomDetails(const QJsonObject &room) {
    QDialog *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle("Бронирование аудитории");
    dialog->setMinimumSize(360, 420);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel(
        QString("Аудитория №%1").arg(room["room_number"].toString()), dialog
    );
    titleLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #423358;"
    );
    mainLayout->addWidget(titleLabel);

    const QString typeLabel = roomTypeLabel(room["type"].toString());
    const int capacity = room["capacity"].toInt();

    QLabel *infoLabel = new QLabel(
        QString("Тип: %1\nКорпус: %2\nВместимость: %3 человек")
            .arg(typeLabel)
            .arg(room["building"].toString())
            .arg(capacity),
        dialog
    );
    mainLayout->addWidget(infoLabel);

    QStringList equipment;
    if (room["has_projector"].toBool()) {
        equipment << "Проектор";
    }
    if (room["has_whiteboard"].toBool()) {
        equipment << "Доска";
    }
    if (room["has_wifi"].toBool()) {
        equipment << "Wi-Fi";
    }
    if (room["has_printers"].toBool()) {
        equipment << "Принтер";
    }
    if (room["has_phone"].toBool()) {
        equipment << "Телефон";
    }

    const QString equipmentText =
        equipment.isEmpty()
            ? "Оборудование: нет"
            : QString("Оборудование: %1").arg(equipment.join(", "));
    mainLayout->addWidget(new QLabel(equipmentText, dialog));

    mainLayout->addWidget(new QLabel("Выберите дату:", dialog));
    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), dialog);
    dateEdit->setCalendarPopup(true);
    disablePastDates(dateEdit);
    mainLayout->addWidget(dateEdit);

    QHBoxLayout *timeLayout = new QHBoxLayout();

    QVBoxLayout *startLayout = new QVBoxLayout();
    startLayout->addWidget(new QLabel("Начало:", dialog));
    QTimeEdit *startTime = new QTimeEdit(QTime::currentTime(), dialog);
    startTime->setMinimumTime(kDayStart);
    startTime->setMaximumTime(kDayEnd);
    startLayout->addWidget(startTime);

    QVBoxLayout *endLayout = new QVBoxLayout();
    endLayout->addWidget(new QLabel("Окончание:", dialog));
    QTimeEdit *endTime =
        new QTimeEdit(QTime::currentTime().addSecs(3600), dialog);
    endTime->setMinimumTime(kDayStart);
    endTime->setMaximumTime(kDayEnd);
    endLayout->addWidget(endTime);
    clampTimeRange(startTime, endTime);

    clampTimeRange(startTime, endTime);

    QObject::connect(
        startTime, &QTimeEdit::timeChanged, dialog,
        [startTime, endTime]() { clampTimeRange(startTime, endTime); }
    );

    QObject::connect(
        endTime, &QTimeEdit::timeChanged, dialog,
        [startTime, endTime]() { clampTimeRange(startTime, endTime); }
    );

    timeLayout->addLayout(startLayout);
    timeLayout->addLayout(endLayout);
    mainLayout->addLayout(timeLayout);

    QPushButton *confirmBtn = new QPushButton("Забронировать", dialog);
    confirmBtn->setObjectName("loginButton");
    mainLayout->addWidget(confirmBtn);

    connect(
        confirmBtn, &QPushButton::clicked,
        [this, room, dateEdit, startTime, endTime, dialog]() {
            QString date = dateEdit->date().toString("yyyy-MM-dd");
            QString start = startTime->time().toString("HH:mm:ss");
            QString end = endTime->time().toString("HH:mm:ss");

            if (startTime->time() < kDayStart || endTime->time() > kDayEnd) {
                QMessageBox::warning(
                    dialog, "Ошибка", "Бронирование доступно с 08:00 до 23:59."
                );
                return;
            }
            if (startTime->time() >= endTime->time()) {
                QMessageBox::warning(
                    dialog, "Ошибка",
                    "Время начала должно быть меньше времени окончания."
                );
                return;
            }

            api->bookRoom(room["id"].toInt(), date, start, end);
            dialog->accept();
        }
    );

    dialog->exec();
}

void room_list_window::onRoomsLoaded(const QJsonArray &roomsArray) {
    rooms = roomsArray;
    renderRooms(rooms);
}

void room_list_window::onBuildingsLoaded(const QJsonArray &buildingsArray) {
    ui->buildingCombo->clear();
    ui->buildingCombo->addItem("Все корпуса", "");

    for (const auto &value : buildingsArray) {
        QJsonObject building = value.toObject();
        ui->buildingCombo->addItem(
            building["name"].toString(), building["name"].toString()
        );
    }

    if (!initialBuildingName.isEmpty()) {
        int idx = ui->buildingCombo->findText(initialBuildingName);
        if (idx >= 0) {
            ui->buildingCombo->setCurrentIndex(idx);
        }
    }
    applyFilters();
}

void room_list_window::applyFilters() {
    RoomFilters filters;
    const QString building = ui->buildingCombo->currentData().toString();
    if (!building.isEmpty()) {
        filters.building = building;
    }

    const QString type = ui->typeCombo->currentData().toString();
    if (!type.isEmpty()) {
        filters.type = type;
    }

    if (ui->capacityMinSpin->value() > 0) {
        filters.capacityMin = ui->capacityMinSpin->value();
    }
    if (ui->capacityMaxSpin->value() > 0) {
        filters.capacityMax = ui->capacityMaxSpin->value();
    }

    if (ui->projectorCheck->isChecked()) {
        filters.hasProjector = true;
    }
    if (ui->whiteboardCheck->isChecked()) {
        filters.hasWhiteboard = true;
    }
    if (ui->wifiCheck->isChecked()) {
        filters.hasWifi = true;
    }
    if (ui->printersCheck->isChecked()) {
        filters.hasPrinters = true;
    }

    currentFilters = filters;
    api->getRooms(currentFilters);
}

void room_list_window::updateViewForBuilding(const QString &buildingName) {
    initialBuildingName = buildingName;
    int idx = ui->buildingCombo->findText(buildingName);
    if (idx >= 0) {
        ui->buildingCombo->setCurrentIndex(idx);
    }
    applyFilters();
}

void room_list_window::renderRooms(const QJsonArray &roomsArray) {
    qDeleteAll(buttons);
    buttons.clear();
    for (const QJsonValue &value : roomsArray) {
        QJsonObject room = value.toObject();
        const QString typeLabel = roomTypeLabel(room["type"].toString());
        const QString roomNumber = room["room_number"].toString();
        QString label =
            QString("Аудитория №%1\n%2").arg(roomNumber).arg(typeLabel);

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
    if (buttons.isEmpty()) {
        return;
    }
    int windowWidth = ui->scrollArea->width();
    if (windowWidth < 100) {
        return;
    }
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
        if (!btn) {
            continue;
        }
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

void room_list_window::scheduleApplyFilters() {
    filterTimer->start();
}

}  // namespace roomsched::client::roomlistwindow
