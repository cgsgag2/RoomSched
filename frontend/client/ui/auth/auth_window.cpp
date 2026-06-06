#include "auth_window.hpp"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include "room_list_window.hpp"
#include "ui_auth_window.h"
#include "register_window.hpp"
#include "main_menu_window.hpp"
#include "main_parent_window.hpp"

namespace roomsched::authwindow {

auth_window::auth_window(roomsched::client::ApiClient *existingApi, QWidget *parent)
    : QWidget(parent), ui(new Ui::auth_window), api(existingApi) {
    ui->setupUi(this);
    ui->errorLabel->hide();
    ui->registerButton->setFlat(true);
    connect(
        ui->mailInput, &QLineEdit::textChanged, ui->errorLabel, &QLabel::hide
    );
    connect(
        ui->passwordInput, &QLineEdit::textChanged, ui->errorLabel, &QLabel::hide
    );
    connect(
        ui->loginButton, &QPushButton::clicked, this,
        &auth_window::on_login_clicked
    );
    connect(
        ui->registerButton, &QPushButton::clicked, this,
        [this]() {
            this->hide();
            QCoreApplication::processEvents();
            auto *regWindow = new roomsched::registerwindow::register_window();
            connect(regWindow, &roomsched::registerwindow::register_window::backToLogin, this, &auth_window::show);
            regWindow->show();
        }
    );
    connect(
        api, &roomsched::client::ApiClient::loginFailed, this,
        [this](QString err) { 
            ui->errorLabel->setText(err);
            ui->errorLabel->show(); 
        }
    );
    connect(
        api, &roomsched::client::ApiClient::loginSuccess, this,
        [this](QJsonObject data) {
            int userId = -1;
        if (data.contains("user") && data["user"].isObject()) {
            userId = data["user"].toObject().value("id").toInt(-1);
        }
        api->setParent(nullptr);
        auto *mainWindow = new roomsched::menu::main_parent_window(
            api, 
            ui->mailInput->text(), 
            userId,
            nullptr 
        );
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(mainWindow, &roomsched::menu::main_parent_window::logoutSuccessful, this, [this](){
            this->show(); 
        });
        mainWindow->show();
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
    ui->errorLabel->setText("");
    ui->errorLabel->hide();
    const QString email = ui->mailInput->text();
    const QString password = ui->passwordInput->text();
    if (!check_email(email)) {
        ui->errorLabel->setText("Введите корректный email.");
        ui->errorLabel->show();
        return;
    }
    if (password.isEmpty()) {
        ui->errorLabel->setText("Введите пароль.");
        ui->errorLabel->show();
        return;
    }
    if (password.length() < 8) {
        ui->errorLabel->setText("Пароль должен быть не менее 8 символов.");
        ui->errorLabel->show();
        return;
    }
    api->login(email, password);
}

}  // namespace roomsched::authwindow