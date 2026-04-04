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

// по факту осталось только для тестов
// это открытие без данных пользователя
room_list_window::room_list_window(QWidget *parent)
    : QWidget(parent), ui(new Ui::room_list_window) {
    ui->setupUi(this);
    setupRooms();
}

room_list_window::room_list_window(
    QString userName,
    QString userEmail,
    QString userPhone,
    QWidget *parent
)
    : QWidget(parent), ui(new Ui::room_list_window) {
    ui->setupUi(this);
    api = new roomsched::client::ApiClient(this);
    connect(
        api, &roomsched::client::ApiClient::roomsLoaded, this,
        [this](QJsonArray arr) {
            rooms = arr;
            setupRooms();
        }
    );
    api->getRooms(1);
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
    layout->setSpacing(15);
    layout->setContentsMargins(10, 10, 10, 10);

    for (int i = 0; i < rooms.size(); ++i) {
        QJsonObject room = rooms[i].toObject();
        QString name = room["name"].toString();
        int capacity = room["capacity"].toInt();
        QPushButton *btn = new QPushButton(
            QString("%1\nВместимость: %2").arg(name).arg(capacity)
        );
        btn->setMinimumSize(200, 120);
        btn->setStyleSheet(
            "QPushButton {"
            "  background-color: white; "
            "  color: black; "
            "  border-radius: 20px; "
            "  border: 1px solid #EEEEEE; "
            "  font-weight: bold; "
            "  font-size: 14px; "
            "}"
            "QPushButton:hover {"
            "  background-color: #3A6161; "
            "  color: white; "
            "  border: none; "
            "}"
        );
        connect(btn, &QPushButton::clicked, this, [this, room]() {
            showRoomDetails(room);
        });
        layout->addWidget(btn, i / 4, i % 4);
    }
}

void room_list_window::showRoomDetails(const QJsonObject &room) {
    QString name = room["name"].toString();
    int capacity = room["capacity"].toInt();
    int room_id = room["id"].toInt();

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(name);
    dialog->setMinimumWidth(350);
    dialog->setStyleSheet("background-color: white;");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *title = new QLabel(name, dialog);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #3A6161;");
    layout->addWidget(title);

    QLabel *capacityLabel = new QLabel(QString("Вместимость: %1").arg(capacity), dialog);
    capacityLabel->setStyleSheet("font-size: 14px; color: #333;");
    layout->addWidget(capacityLabel);

    QHBoxLayout *dateLayout = new QHBoxLayout();
    QLabel *dateLabel = new QLabel("Дата:", dialog);
    dateLabel->setStyleSheet("font-size: 14px; color: #333;");
    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), dialog);
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumDate(QDate::currentDate());
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateEdit);
    layout->addLayout(dateLayout);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    QLabel *timeLabel = new QLabel("Время:", dialog);
    timeLabel->setStyleSheet("font-size: 14px; color: #333;");
    QTimeEdit *startTime = new QTimeEdit(QTime(10, 0), dialog);
    QTimeEdit *endTime = new QTimeEdit(QTime(12, 0), dialog);
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(startTime);
    timeLayout->addWidget(new QLabel("-", dialog));
    timeLayout->addWidget(endTime);
    layout->addLayout(timeLayout);

    QPushButton *bookBtn = new QPushButton("Забронировать", dialog);
    bookBtn->setCursor(Qt::PointingHandCursor);
    bookBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #3A6161; color: white; border-radius: 10px; "
        "  padding: 10px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #447575; }"
    );

    connect(bookBtn, &QPushButton::clicked, [this, dialog, name, room_id, dateEdit, startTime, endTime]() {
        QString date = dateEdit->date().toString("yyyy-MM-dd");
        QString start = startTime->time().toString("hh:mm");
        QString end = endTime->time().toString("hh:mm");

        if (start >= end) {
            QMessageBox::warning(dialog, "Ошибка", "Время начала должно быть меньше времени окончания");
            return;
        }

        QJsonObject body{
            {"room_id", room_id},
            {"user_id", 1},
            {"date", date},
            {"start_time", start},
            {"end_time", end}
        };

        QNetworkRequest req(QUrl("http://localhost:8080/bookings"));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, [dialog, name, manager](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QMessageBox::information(dialog, "Успех", "Вы успешно забронировали " + name);
                dialog->accept();
            } else {
                QMessageBox::warning(dialog, "Ошибка", "Не удалось забронировать комнату");
            }
            reply->deleteLater();
            manager->deleteLater();
        });

        manager->post(req, QJsonDocument(body).toJson());
    });

    layout->addWidget(bookBtn);
    dialog->exec();
}

}  // namespace roomsched::roomlistwindow
