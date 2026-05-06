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
    QWidget *parent
)
    : QWidget(parent), ui(new Ui::room_list_window), api(existingApi) {
    ui->setupUi(this);
    connect(api, &roomsched::client::ApiClient::roomsLoaded, this, &room_list_window::onRoomsLoaded);
    connect(api, &roomsched::client::ApiClient::bookingFinished, this, [this](bool success, QString message) {
        if (success) {
            QMessageBox::information(this, "Успех", "Комната успешно забронирована!");
            api->getRooms(); 
        } else {
            QMessageBox::warning(this, "Ошибка бронирования", message);
        }
    });
    qDebug() << "DEBUG: Calling getRooms now...";
    api->getRooms();
}

room_list_window::~room_list_window() {
    delete ui;
}

void room_list_window::setupRooms() {
    QGridLayout *layout =
        qobject_cast<QGridLayout *>(ui->roomsGridContainer->layout());
    if (!layout) {
        return;
    }
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    layout->setSpacing(15);
    int columns = 4;

    for (int i = 0; i < rooms.size(); ++i) {
        QJsonObject room = rooms[i].toObject();
        bool isAvailable = room["is_available"].toBool();
        QString roomName = room["room_number"].toString();
        QPushButton *btn = new QPushButton(roomName);
        btn->setMinimumSize(100, 100);
        if (!isAvailable) {
            btn->setStyleSheet("background-color: #f44336; color: white;"); 
            btn->setText(roomName + " (Занято)");
        } else {
            btn->setStyleSheet("background-color: #4CAF50; color: white;"); 
        }
        layout->addWidget(btn, i / columns, i % columns);
        connect(btn, &QPushButton::clicked, [this, room]() {
            showRoomDetails(room);
        });
    }
}

void room_list_window::showRoomDetails(const QJsonObject &room) {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Бронирование: " + room["room_number"].toString());
    dialog->setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *titleLabel = new QLabel("Информация о помещении", dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 10px;");
    layout->addWidget(titleLabel);

    QString infoText = QString(
        "<b>Здание:</b> %1<br>"
        "<b>Этаж:</b> %2<br>"
        "<b>Площадь:</b> %3 м²<br>"
        "<b>Описание:</b> %4"
    ).arg(room["building"].toString())
     .arg(room["floor"].toInt())
     .arg(room["total_area"].toDouble())
     .arg(room["description"].toString());

    QLabel *infoLabel = new QLabel(infoText, dialog);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    layout->addSpacing(20);

    layout->addWidget(new QLabel("Выберите дату:", dialog));
    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), dialog);
    dateEdit->setCalendarPopup(true);
    layout->addWidget(dateEdit);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    QTimeEdit *startTime = new QTimeEdit(QTime(10, 0), dialog);
    QTimeEdit *endTime = new QTimeEdit(QTime(11, 0), dialog);
    
    timeLayout->addWidget(new QLabel("С:"));
    timeLayout->addWidget(startTime);
    timeLayout->addWidget(new QLabel("До:"));
    timeLayout->addWidget(endTime);
    layout->addLayout(timeLayout);

    layout->addSpacing(20);

    QPushButton *confirmBtn = new QPushButton("Подтвердить бронирование", dialog);
    confirmBtn->setCursor(Qt::PointingHandCursor);
    confirmBtn->setStyleSheet(
        "QPushButton { background-color: #3A6161; color: white; padding: 10px; border-radius: 5px; font-weight: bold; }"
        "QPushButton:hover { background-color: #447575; }"
    );

    connect(confirmBtn, &QPushButton::clicked, [this, dialog, room, dateEdit, startTime, endTime]() {
        QString date = dateEdit->date().toString("yyyy-MM-dd");
        QString start = startTime->time().toString("HH:mm:ss");
        QString end = endTime->time().toString("HH:mm:ss");

        if (startTime->time() >= endTime->time()) {
            QMessageBox::warning(dialog, "Ошибка", "Время начала должно быть меньше времени окончания.");
            return;
        }

        api->bookRoom(
            room["id"].toInt(), 
            date, 
            start, 
            end
        );

        dialog->accept();
    });

    layout->addWidget(confirmBtn);
    dialog->exec();
}

void room_list_window::onRoomsLoaded(const QJsonArray &roomsArray) {
    this->rooms = roomsArray;
    QLayoutItem *item;
    while ((item = ui->gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    int row = 0, col = 0;
    for (const QJsonValue &value : roomsArray) {
        QJsonObject room = value.toObject();
                QString label = QString("Комната %1\n%2")
                        .arg(room["room_number"].toString())
                        .arg(room["building"].toString());
                        
        QPushButton *btn = new QPushButton(label, this);
        btn->setMinimumSize(120, 100);
        
        connect(btn, &QPushButton::clicked, [this, room]() {
            showRoomDetails(room);
        });
        ui->gridLayout->addWidget(btn, row, col);
        col++;
        if (col > 3) {
            col = 0;
            row++;
        }
    }
}

}  // namespace roomsched::roomlistwindow
