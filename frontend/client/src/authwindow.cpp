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
    api = new roomsched::client::ApiClient(this);
    connect(
        ui->loginButton, &QPushButton::clicked, this,
        &AuthWindow::onLoginClicked
    );
    connect(
        api, &roomsched::client::ApiClient::loginFailed, this,
        [](QString err) { QMessageBox::warning(nullptr, "Ошибка", err); }
    );
    connect(
        api, &roomsched::client::ApiClient::loginSuccess, this,
        [this](QJsonObject) {
            auto *rooms = new roomsched::roomlistwindow::RoomListWindow(
                ui->nameInput->text(), ui->mailInput->text(),
                ui->phoneInput->text()
            );
            rooms->show();
            this->close();
        }
    );
}

AuthWindow::~AuthWindow() {
    delete ui;
}

bool AuthWindow::checkName(QString enterName) {
    const QStringList parts =
        enterName.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    return parts.size() >= 2;
}

bool AuthWindow::checkEmail(QString enterEmail) {
    QString doneEmail = enterEmail.trimmed();
    const QRegularExpression emailRegex(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
    return emailRegex.match(doneEmail).hasMatch();
}

bool AuthWindow::checkPhone() {
    return ui->phoneInput->hasAcceptableInput();
}

void AuthWindow::onLoginClicked() {
    const QString name = ui->nameInput->text();
    const QString email = ui->mailInput->text();
    const QString phone = ui->phoneInput->text();
    if (!checkName(name)) {
        QMessageBox::warning(
            nullptr, "Ошибка", "Введите хотя бы имя и фамилию."
        );
        return;
    }
    if (!checkEmail(email)) {
        QMessageBox::warning(nullptr, "Ошибка", "Введите корректный email.");
        return;
    }
    if (!checkPhone()) {
        QMessageBox::warning(
            nullptr, "Ошибка", "Номер телефона заполнен не полностью."
        );
        return;
    }
    api->login(name, email);
}

}  // namespace roomsched::authwindow
