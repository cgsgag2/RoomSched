#include "register_window.hpp"
#include "ui_register_window.h"
#include <QMessageBox>
#include <QRegularExpression>

namespace roomsched::registerwindow {

register_window::register_window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::register_window)
{
    ui->setupUi(this);
    api = new roomsched::client::ApiClient(this);

    connect(ui->registerButton, &QPushButton::clicked,
            this, &register_window::onRegisterButtonClicked);
}

register_window::~register_window()
{
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
bool register_window::check_phone()
{
    return ui->phoneInput->hasAcceptableInput();
}

void register_window::onRegisterButtonClicked()
{
    QString username = ui->nameInput->text();
    QString email = ui->mailInput->text();
    QString phone = ui->phoneInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || email.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }
    if (!check_name(username)) {
        QMessageBox::warning(this, "Ошибка", "Введите имя и фамилию.");
        return;
    }
    if (!check_email(email)) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный email.");
        return;
    }
    if (!check_phone()) {
        QMessageBox::warning(this, "Ошибка", "Введите полный номер телефона.");
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите пароль.");
        return;
    }
    if (password.length() < 6) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен быть не менее 6 символов.");
        return;
    }

    // TODO: подключить api->registerUser
    QMessageBox::information(this, "Успех", "Регистрация (заглушка)");
}

} // namespace roomsched::registerwindow