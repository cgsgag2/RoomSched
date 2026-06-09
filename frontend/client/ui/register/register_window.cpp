#include "register_window.hpp"
#include <QMessageBox>
#include <QRegularExpression>
#include "ui_register_window.h"

namespace roomsched::client::registerwindow {

register_window::register_window(QWidget *parent)
    : QWidget(parent), ui(new Ui::register_window) {
    ui->setupUi(this);
    api = new ApiClient(this);
    ui->errorLabel->hide();
    connect(
        ui->nameInput, &QLineEdit::textChanged, ui->errorLabel, &QLabel::hide
    );
    connect(
        ui->mailInput, &QLineEdit::textChanged, ui->errorLabel, &QLabel::hide
    );
    connect(
        ui->phoneInput, &QLineEdit::textChanged, ui->errorLabel, &QLabel::hide
    );
    connect(
        ui->passwordInput, &QLineEdit::textChanged, ui->errorLabel,
        &QLabel::hide
    );

    connect(
        api, &ApiClient::registrationFinished, this,
        [this](bool success, QString msg) {
            if (success) {
                QMessageBox::information(
                    this, "Успех", "Вы успешно зарегистрированы!"
                );
                emit backToLogin();
                this->close();
            } else {
                ui->errorLabel->setText(msg);
                ui->errorLabel->show();
            }
        }
    );
    connect(
        ui->registerSubmitButton, &QPushButton::clicked, this,
        &register_window::onRegisterButtonClicked
    );
    connect(ui->backButton, &QPushButton::clicked, this, [this]() {
        emit backToLogin();
        this->close();
    });
}

register_window::~register_window() {
    delete ui;
}

bool register_window::check_name(QString enterName) {
    const QStringList parts =
        enterName.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    return parts.size() >= 2;
}

bool register_window::check_email(QString enterEmail) {
    QString doneEmail = enterEmail.trimmed();
    const QRegularExpression emailRegex(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
    return emailRegex.match(doneEmail).hasMatch();
}

bool register_window::check_phone() {
    return ui->phoneInput->hasAcceptableInput();
}

void register_window::onRegisterButtonClicked() {
    QString username = ui->nameInput->text();
    QString email = ui->mailInput->text();
    QString phone = ui->phoneInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || email.isEmpty() || phone.isEmpty() ||
        password.isEmpty()) {
        ui->errorLabel->setText("Заполните все поля.");
        ui->errorLabel->show();
        return;
    }
    if (!check_name(username)) {
        ui->errorLabel->setText("Введите имя и фамилию через пробел.");
        ui->errorLabel->show();
        return;
    }
    if (!check_email(email)) {
        ui->errorLabel->setText("Введите корректный адрес email.");
        ui->errorLabel->show();
        return;
    }
    if (!check_phone()) {
        ui->errorLabel->setText("Введите номер телефона.");
        ui->errorLabel->show();
        return;
    }
    if (password.length() < 8) {
        ui->errorLabel->setText("Пароль должен быть не короче 8 символов.");
        ui->errorLabel->show();
        return;
    }
    api->registerUser(username, email, phone, password);
}

}  // namespace roomsched::client::registerwindow
