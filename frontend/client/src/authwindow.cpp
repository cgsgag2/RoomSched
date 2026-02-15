#include "authwindow.hpp"
#include <QDebug>
#include <QRegularExpressionValidator>
#include "ui_authwindow.h"

namespace roomsched {

AuthWindow::AuthWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AuthWindow) {
    ui->setupUi(this);

    connect(
        ui->loginButton, &QPushButton::clicked, this,
        &AuthWindow::onLoginClicked
    );
}

AuthWindow::~AuthWindow() {
    delete ui;
}

void AuthWindow::onLoginClicked() {
    QString name = ui->nameInput->text();
    QString email = ui->mailInput->text();
    QString phone = ui->phoneInput->text();
    if (name.isEmpty() || email.isEmpty() || phone.isEmpty()) {
        qDebug() << "Ошибка: Пожалуйста, заполните все поля!";
    } else {
        qDebug() << "Данные получены:" << name << email << phone;

        // coming soon открытие странички с аудиториями
    }
}

}  // namespace roomsched
