#include "user_bookings_window.hpp"
#include "ui_user_bookings_window.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QJsonObject>
#include <QListWidgetItem>

namespace roomsched::bookings {

user_bookings_window::user_bookings_window(
    roomsched::client::ApiClient *existingApi,
    int userId,
    QWidget *parent
) : QWidget(parent), ui(new Ui::user_bookings_window), api(existingApi), currentUserId(userId) {
    ui->setupUi(this);
    loadBookings();
    connect(ui->refreshButton, &QPushButton::clicked, this, &user_bookings_window::loadBookings);
    connect(api, &roomsched::client::ApiClient::bookingsLoaded, this, &user_bookings_window::onBookingsLoaded);
    connect(api, &roomsched::client::ApiClient::bookingCancelled, this, &user_bookings_window::onCancelResponse);
    loadBookings();
}

user_bookings_window::~user_bookings_window() {
    delete ui;
}

void user_bookings_window::loadBookings() {
    api->getUserBookings(currentUserId);
}

void user_bookings_window::onBookingsLoaded(const QJsonArray &bookingsArray) {
    ui->bookingsList->clear();

    for (int i = 0; i < bookingsArray.size(); ++i) {
        QJsonObject booking = bookingsArray[i].toObject();
        if (booking["status"].toString() == "cancelled") {
            continue; // Пропускаем отмененные, не добавляем их в UI
        }
        int bookingId = booking["id"].toInt();
        int roomNum = booking["room_id"].toInt();
        QString date = booking["booking_date"].toString();
        QString start = booking["start_time"].toString();
        QString end = booking["end_time"].toString();
        QString infoText = QString("Аудитория %1, %2 с %3 до %4")
                            .arg(roomNum)
                            .arg(date)
                            .arg(start)
                            .arg(end);

        QWidget *rowWidget = new QWidget(this);
        rowWidget->setStyleSheet(
            "QWidget {"
            "   background-color: #dbd3e3;" 
            "   border-radius: 8px;"
            "}"
        );

        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(15, 10, 15, 10);

        QLabel *textLabel = new QLabel(infoText, rowWidget);
        textLabel->setStyleSheet("color: #2d213d; font-weight: 500; background: transparent;");
        rowLayout->addWidget(textLabel);
        
        rowLayout->addStretch();

        QPushButton *closeBtn = new QPushButton("✕", rowWidget);
        closeBtn->setFixedSize(24, 24);
        closeBtn->setCursor(Qt::PointingHandCursor);
        closeBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: transparent;"
            "   color: #ff6b6b;"
            "   border: none;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "   border-radius: 4px;"
            "}"
            "QPushButton:hover {"
            "   background-color: rgba(255, 107, 107, 0.15);"
            "   color: #e55353;"
            "}"
        );
        
        closeBtn->setProperty("bookingId", bookingId);
        connect(closeBtn, &QPushButton::clicked, this, &user_bookings_window::onCancelButtonClicked);
        rowLayout->addWidget(closeBtn);
        rowWidget->setLayout(rowLayout);
        QListWidgetItem *item = new QListWidgetItem(ui->bookingsList);
        QSize hint = rowWidget->sizeHint();
        hint.setHeight(hint.height() + 5); 
        item->setSizeHint(hint);
        ui->bookingsList->addItem(item);
        ui->bookingsList->setItemWidget(item, rowWidget);
    }
}

void user_bookings_window::onCancelButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int bookingId = btn->property("bookingId").toInt();

    auto reply = QMessageBox::question(
        this, "Отмена бронирования",
        QString("Вы действительно хотите отменить это бронирование?"),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        api->cancelBooking(bookingId);
    }
}

void user_bookings_window::onCancelResponse(bool success, const QString &message) {
    if (success) {
        QMessageBox::information(this, "Успех", "Бронирование успешно отменено.");
        loadBookings();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось отменить бронирование: " + message);
    }
}

} // namespace roomsched::bookings