#include "authwindow.hpp"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include "roomlistwindow.hpp"
#include "ui_authwindow.h"

namespace roomsched::authwindow {

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

bool AuthWindow::CheckName(QString EnterName) {
    QStringList parts =
        EnterName.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    return parts.size() >= 2;
}

bool AuthWindow::CheckEmail(QString EnterEmail) {
    QString DoneEmail = EnterEmail.trimmed();
    QRegularExpression emailRegex(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
    return emailRegex.match(DoneEmail).hasMatch();
}

bool AuthWindow::CheckPhone() {
    return ui->phoneInput->hasAcceptableInput();
}

void AuthWindow::onLoginClicked() {
    QString name = ui->nameInput->text();
    QString email = ui->mailInput->text();
    QString phone = ui->phoneInput->text();
    if (!CheckName(name)) {
        QMessageBox::warning(
            nullptr, "Ошибка", "Введите хотя бы имя и фамилию."
        );
        return;
    }
    if (!CheckEmail(email)) {
        QMessageBox::warning(nullptr, "Ошибка", "Введите корректный email.");
        return;
    }
    if (!CheckPhone()) {
        QMessageBox::warning(
            nullptr, "Ошибка", "Номер телефона заполнен не полностью."
        );
        return;
    }
    // часть с отправкой данных на сервак
    // отправить данные на сервак, дождаться от него ответ
    // если сервак ответил ок, то идем в открытие окно дальше
    // если нет - точно так же кинуть плашку об ошибке как выше
    auto *rooms =
        new roomsched::roomlistwindow::RoomListWindow(name, email, phone);
    rooms->show();
    this->close();
}

}  // namespace roomsched::authwindow
