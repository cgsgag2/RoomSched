#include "roomlistwindow.hpp"
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "ui_roomlistwindow.h"

namespace roomsched {

RoomListWindow::RoomListWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::RoomListWindow) {
    ui->setupUi(this);
    setupRooms();
}

RoomListWindow::~RoomListWindow() {
    delete ui;
}

void RoomListWindow::setupRooms() {
    QGridLayout *layout =
        qobject_cast<QGridLayout *>(ui->roomsGridContainer->layout());
    if (!layout) {
        return;
    }
    layout->setSpacing(15);
    layout->setContentsMargins(10, 10, 10, 10);

    for (int i = 0; i < 16; ++i) {
        QString roomName = QString("Аудитория %1").arg(101 + i);
        QPushButton *btn = new QPushButton(roomName);
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
        connect(btn, &QPushButton::clicked, this, [this, roomName]() {
            showRoomDetails(roomName);
        });
        layout->addWidget(btn, i / 4, i % 4);
    }
}

void RoomListWindow::showRoomDetails(const QString &roomName) {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Информация об аудитории");
    dialog->setMinimumWidth(300);
    dialog->setStyleSheet("background-color: white;");
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);
    QLabel *title = new QLabel(roomName, dialog);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #3A6161;");
    layout->addWidget(title);
    QLabel *info =
        new QLabel("Доступное время:\n10:00 - 12:00\n14:00 - 18:00", dialog);
    info->setStyleSheet("font-size: 14px; color: #333;");
    layout->addWidget(info);
    QPushButton *bookBtn = new QPushButton("Забронировать", dialog);
    bookBtn->setCursor(Qt::PointingHandCursor);
    bookBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #3A6161; color: white; border-radius: 10px; "
        "padding: 10px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #447575; }"
    );
    connect(bookBtn, &QPushButton::clicked, [dialog, roomName]() {
        QMessageBox msgBox(dialog);
        msgBox.setWindowTitle("Успех");
        msgBox.setText("Вы успешно забронировали " + roomName);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QLabel{ color: black; } QPushButton{ background-color: lightgray; "
            "color: black; }"
        );
        msgBox.exec();
        dialog->accept();
    });
    layout->addWidget(bookBtn);
    dialog->exec();
}

}  // namespace roomsched
