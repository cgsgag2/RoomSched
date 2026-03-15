#include "registerwindow.hpp"
#include "ui_registerwindow.h"
#include <QMessageBox>
#include <QRegularExpression>

namespace roomsched::registerwindow {

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    api = new roomsched::client::ApiClient(this);

    connect(ui->registerButton, &QPushButton::clicked,
            this, &RegisterWindow::onRegisterButtonClicked);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}
bool RegisterWindow::checkName(QString enterName) {
    const QStringList parts =
        enterName.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    return parts.size() >= 2;
}

bool RegisterWindow::checkEmail(QString enterEmail) {
    QString doneEmail = enterEmail.trimmed();
    const QRegularExpression emailRegex(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
    return emailRegex.match(doneEmail).hasMatch();
}
bool RegisterWindow::checkPhone()
{
    return ui->phoneInput->hasAcceptableInput();
}

void RegisterWindow::onRegisterButtonClicked()
{
    QString username = ui->nameInput->text();
    QString email = ui->mailInput->text();
    QString phone = ui->phoneInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || email.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }
    if (!checkName(username)) {
        QMessageBox::warning(this, "Ошибка", "Введите имя и фамилию.");
        return;
    }
    if (!checkEmail(email)) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный email.");
        return;
    }
    if (!checkPhone()) {
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