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

    // Логика подписок на сигналы API и кнопки
    connect(api, &roomsched::client::ApiClient::buildingsLoaded, this, &main_menu_window::onBuildingsLoaded);
    connect(ui->continueButton, &QPushButton::clicked, this, &main_menu_window::onContinueClicked);

    // Запрашиваем список зданий
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
        QMessageBox msgBox(QMessageBox::Warning, "Ошибка", "Пожалуйста, выберите здание из списка.", QMessageBox::Ok, this);
    
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: #c9bfd4;" 
        "   min-width: 450px;"         
        "}"
        "QLabel {"
        "   color: #2d213d;"
        "   font-size: 14px;"
        "}"
        "QPushButton {"
        "   background-color: #6c528d;"
        "   color: white;"
        "   border-radius: 4px;"
        "   min-width: 80px;"
        "   min-height: 25px;"
        "}"
        "QPushButton:hover { background-color: #553e72; }"
    );
    
    msgBox.exec();
    return;
    }

    auto *rooms = new roomsched::roomlistwindow::room_list_window(
        api,
        "", // Передаем пустое имя, если его нет в этом окне
        email,
        "", // Передаем пустой телефон
        nullptr, // Открываем как самостоятельное окно
        buildingName
    );
    rooms->show();
    this->close();
}

}  // namespace roomsched::mainmenu