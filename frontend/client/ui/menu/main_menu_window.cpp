#include "main_menu_window.hpp"
#include <QMessageBox>
#include "room_list_window.hpp"
#include "ui_main_menu_window.h"

namespace roomsched::mainmenu {

main_menu_window::main_menu_window(
    roomsched::client::ApiClient *existingApi,
    const QString &userEmail,
    QWidget *parent
)
    : QWidget(parent),
      ui(new Ui::main_menu_window),
      api(existingApi),
      email(userEmail) {
    ui->setupUi(this);
    ui->welcomeLabel->setText(
        "Здравствуйте, добро пожаловать в RoomSched!\n"
        "Выберите, пожалуйста, здание для бронирования."
    );

    connect(api, &roomsched::client::ApiClient::buildingsLoaded, this, &main_menu_window::onBuildingsLoaded);
    connect(ui->continueButton, &QPushButton::clicked, this, &main_menu_window::onContinueClicked);

    api->getBuildings();
}

main_menu_window::~main_menu_window() {
    delete ui;
}

void main_menu_window::onBuildingsLoaded(const QJsonArray &buildingsArray) {
    ui->buildingCombo->clear();
    for (const auto &value : buildingsArray) {
        QJsonObject building = value.toObject();
        ui->buildingCombo->addItem(building["name"].toString());
    }
}

void main_menu_window::onContinueClicked() {
    const QString buildingName = ui->buildingCombo->currentText();
    if (buildingName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите здание.");
        return;
    }

    auto *rooms = new roomsched::roomlistwindow::room_list_window(
        api,
        email,
        email,
        "",
        nullptr,
        buildingName
    );
    rooms->show();
    close();
}

}  // namespace roomsched::mainmenu

