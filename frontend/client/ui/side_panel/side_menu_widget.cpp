#include "side_menu_widget.hpp"
#include "ui_side_menu_widget.h"

namespace roomsched::menu {

side_menu_widget::side_menu_widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::side_menu_widget) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    connect(ui->btnMenuClose, &QPushButton::clicked, this, &side_menu_widget::hide);
    connect(ui->btnHome, &QPushButton::clicked, this, &side_menu_widget::navigateToHome);
    connect(ui->btnRooms, &QPushButton::clicked, this, &side_menu_widget::navigateToRooms);
    connect(ui->btnBookings, &QPushButton::clicked, this, &side_menu_widget::navigateToBookings);
    connect(ui->btnLogout, &QPushButton::clicked, this, &side_menu_widget::logoutRequested);
}

side_menu_widget::~side_menu_widget() {
    delete ui;
}

} // namespace roomsched::menu