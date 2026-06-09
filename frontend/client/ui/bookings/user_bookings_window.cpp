#include "user_bookings_window.hpp"
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include "ui_user_bookings_window.h"

namespace roomsched::client::bookings {

user_bookings_window::user_bookings_window(
    ApiClient *existingApi,
    int userId,
    QWidget *parent
)
    : QWidget(parent),
      ui(new Ui::user_bookings_window),
      api(existingApi),
      currentUserId(userId) {
    ui->setupUi(this);
    loadBookings();
    connect(
        ui->refreshButton, &QPushButton::clicked, this,
        &user_bookings_window::loadBookings
    );
    connect(
        api, &ApiClient::bookingsLoaded, this,
        &user_bookings_window::onBookingsLoaded
    );
    connect(
        api, &ApiClient::bookingCancelled, this,
        &user_bookings_window::onCancelResponse
    );
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
            continue;
        }
        int bookingId = booking["id"].toInt();
        int roomId = booking["room_id"].toInt();
        QString roomName =
            booking["room_number"].toString("Ауд. " + QString::number(roomId));
        QString buildingName =
            booking["building_name"].toString("Неизвестное здание");
        QString date =
            QDate::fromString(booking["booking_date"].toString(), "yyyy-MM-dd")
                .toString("dd.MM.yy");
        QString start = booking["start_time"].toString().left(5);
        QString end = booking["end_time"].toString().left(5);
        QString infoText = QString("%1 | ауд. %2 | %3 | с %4 до %5")
                               .arg(buildingName)
                               .arg(roomName)
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
        rowWidget->setMinimumHeight(50);
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(15, 10, 15, 10);

        QLabel *textLabel = new QLabel(infoText, rowWidget);
        textLabel->setStyleSheet(
            "color: #2d213d; font-weight: 500; background: transparent;"
        );
        rowLayout->addWidget(textLabel);

        rowLayout->addStretch();

        QPushButton *closeBtn = new QPushButton("✕", rowWidget);
        closeBtn->setFixedSize(32, 32);
        closeBtn->setCursor(Qt::PointingHandCursor);
        closeBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: transparent;"
            "   color: #ff6b6b;"
            "   border: 1px solid #b34343;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "   border-radius: 4px;"
            "}"
            "QPushButton:hover {"
            "   background-color: rgba(255, 107, 107, 0.15);"
            "   border: 1px solid #7a2e2e;"
            "   color: #e55353;"
            "}"
        );

        closeBtn->setProperty("bookingId", bookingId);
        connect(
            closeBtn, &QPushButton::clicked, this,
            &user_bookings_window::onCancelButtonClicked
        );
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
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) {
        return;
    }

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

void user_bookings_window::onCancelResponse(
    bool success,
    const QString &message
) {
    if (success) {
        QMessageBox::information(
            this, "Успех", "Бронирование успешно отменено."
        );
        loadBookings();
    } else {
        QMessageBox::critical(
            this, "Ошибка", "Не удалось отменить бронирование: " + message
        );
    }
}

}  // namespace roomsched::client::bookings
