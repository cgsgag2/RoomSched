#include "telegram_binding_window.hpp"
#include <QMessageBox>
#include "ui_telegram_binding_window.h"

namespace roomsched::client::telegram {

telegram_binding_window::telegram_binding_window(
    ApiClient *existingApi,
    int userId,
    QWidget *parent
)
    : QWidget(parent),
      ui(new Ui::telegram_binding_window),
      api(existingApi),
      currentUserId(userId) {
    ui->setupUi(this);

    connect(
        ui->getCodeButton, &QPushButton::clicked, this,
        &telegram_binding_window::onGetCodeClicked
    );

    connect(
        api, &ApiClient::telegramCodeReceived, this,
        [this](const QString &code) {
            ui->getCodeButton->setEnabled(false);
            ui->getCodeButton->setText("Код получен, telegram привязан");

            QMessageBox::information(
                this, "Подключение Telegram",
                QString("📨 Код успешно получен.\n\n"
                        "Откройте Telegram-бота:\n"
                        "@roomsched_notifications_bot\n\n"
                        "И отправьте команду:\n\n"
                        "/link %1\n\n"
                        "После этого уведомления будут подключены.")
                    .arg(code)
            );
        }
    );

    connect(
        api, &ApiClient::telegramLinkFailed, this,
        [this](const QString &message) {
            QMessageBox::critical(this, "Ошибка", message);
        }
    );
}

telegram_binding_window::~telegram_binding_window() {
    delete ui;
}

void telegram_binding_window::onGetCodeClicked() {
    api->requestTelegramLinkCode(currentUserId);
}

}  // namespace roomsched::client::telegram
