#include "auth_window.hpp"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include "room_list_window.hpp"
#include "ui_auth_window.h"
#include "register_window.hpp"

namespace roomsched::authwindow {

auth_window::auth_window(QWidget *parent)
    : QWidget(parent), ui(new Ui::auth_window) {
    ui->setupUi(this);
    api = new roomsched::client::ApiClient(this);
    connect(
        ui->loginButton, &QPushButton::clicked, this,
        &auth_window::on_login_clicked
    );
    connect(
        ui->registerButton, &QPushButton::clicked, this,
        [this]() {
        auto *regWindow = new roomsched::registerwindow::register_window();
        connect(regWindow, &roomsched::registerwindow::register_window::backToLogin, this, &auth_window::show);
        regWindow->show();
        this->hide();
    }
);
    connect(
        api, &roomsched::client::ApiClient::loginFailed, this,
        [](QString err) { QMessageBox::warning(nullptr, "Ошибка", err); }
    );
    connect(
        api, &roomsched::client::ApiClient::loginSuccess, this,
        [this](QJsonObject) {
            auto *rooms = new roomsched::roomlistwindow::room_list_window(
                api, 
                ui->mailInput->text(), 
                ui->mailInput->text(),
                "",
                nullptr 
            );
            rooms->show();
            this->close();
        }
    );
}

auth_window::~auth_window() {
    delete ui;
}

bool auth_window::check_name(QString enterName) {
    const QStringList parts =
        enterName.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    return parts.size() >= 2;
}

bool auth_window::check_email(QString enterEmail) {
    QString doneEmail = enterEmail.trimmed();
    const QRegularExpression emailRegex(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
    return emailRegex.match(doneEmail).hasMatch();
}

void auth_window::on_login_clicked() {
    const QString email = ui->mailInput->text();
    const QString password = ui->passwordInput->text();
    if (!check_email(email)) {
        QMessageBox::warning(nullptr, "Ошибка", "Введите корректный email.");
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите пароль.");
        return;
    }
    if (password.length() < 8) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен быть не менее 8 символов.");
        return;
    }
    api->login(email, password);
}

}  // namespace roomsched::authwindow