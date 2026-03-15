#include "authwindow.hpp"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include "roomlistwindow.hpp"
#include "ui_authwindow.h"
#include "registerwindow.hpp"

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
        ui->registerButton, &QPushButton::clicked, this,
        [this]() {
        auto *regWindow = new roomsched::registerwindow::RegisterWindow();
        regWindow->show();
        this->close();
    }
);
    connect(
        api, &roomsched::client::ApiClient::loginFailed, this,
        [](QString err) { QMessageBox::warning(nullptr, "Ошибка", err); }
    );
    connect(
        api, &roomsched::client::ApiClient::loginSuccess, this,
        [this](QJsonObject) {
            auto *rooms = new roomsched::roomlistwindow::RoomListWindow(
                ui->mailInput->text(), ui->mailInput->text(),
                ""
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

void AuthWindow::onLoginClicked() {
    const QString email = ui->mailInput->text();
    const QString password = ui->passwordInput->text();
    if (!checkEmail(email)) {
        QMessageBox::warning(nullptr, "Ошибка", "Введите корректный email.");
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите пароль.");
        return;
    }
    //TODO add check of password
    if (password.length() < 6) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен быть не менее 6 символов.");
        return;
    }
    api->login(email, password);
}

}  // namespace roomsched::authwindow