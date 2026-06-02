#include "main_parent_window.hpp"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>

namespace roomsched::menu {

main_parent_window::main_parent_window(
    roomsched::client::ApiClient *existingApi,
    const QString &userEmail,
    int userId,
    QWidget *parent
) : QWidget(parent) {
    
    resize(800, 550);
    setWindowTitle("RoomSched");
    QVBoxLayout *mainLayout = new QVBoxLayout(this); 
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("background-color: #c9bfd4;");
    mainLayout->addWidget(stackedWidget); // Добавьте stackedWidget в лейаут
    setLayout(mainLayout);
    homeWindow = new roomsched::mainmenu::main_menu_window(existingApi, userEmail, this);    
    roomsWindow = new roomsched::roomlistwindow::room_list_window(existingApi, "", userEmail, "", this);
    bookingsWindow = new roomsched::bookings::user_bookings_window(existingApi, userId, this);

    setupWindowHeader(homeWindow, "Добро пожаловать в RoomSched!");
    setupWindowHeader(roomsWindow, "Выбор аудитории для бронирования");
    setupWindowHeader(bookingsWindow, "Мои активные бронирования");

    stackedWidget->addWidget(homeWindow); 
    stackedWidget->addWidget(roomsWindow);  
    stackedWidget->addWidget(bookingsWindow); 

    sideMenu = new side_menu_widget(this);
    sideMenu->hide(); 

    connect(sideMenu, &side_menu_widget::navigateToHome, this, [this]() {
        stackedWidget->setCurrentIndex(0);
        hideMenu();
    });
    connect(sideMenu, &side_menu_widget::navigateToRooms, this, [this]() {
        roomsWindow->updateViewForBuilding("Все корпуса");
        stackedWidget->setCurrentIndex(1);
        hideMenu();
    });
    connect(sideMenu, &side_menu_widget::navigateToBookings, this, [this]() {
        stackedWidget->setCurrentIndex(2);
        hideMenu();
    });
    
    connect(homeWindow, &roomsched::mainmenu::main_menu_window::buildingSelected, this, [this](const QString &buildingName) {
        roomsWindow->updateViewForBuilding(buildingName);
        stackedWidget->setCurrentIndex(1); 
        roomsWindow->update(); 
        roomsWindow->show();
    });
}

main_parent_window::~main_parent_window() {}

void main_parent_window::setupWindowHeader(QWidget *window, const QString &titleText) {
    QLabel *titleLabel = window->findChild<QLabel*>("titleLabel");
    if (!titleLabel) return;
    if (titleLabel->layout()) delete titleLabel->layout();
    titleLabel->setText(""); 
    
    titleLabel->setStyleSheet(
        "background-color: #423358;"
        "border-radius: 8px;"
    );

    QHBoxLayout *headerLayout = new QHBoxLayout(titleLabel);
    headerLayout->setContentsMargins(20, 5, 15, 5);

    QLabel *textLabel = new QLabel(titleText, titleLabel);
    textLabel->setStyleSheet("color: #ffffff; font-size: 18px; font-weight: 600; background: transparent;");
    headerLayout->addWidget(textLabel);

    headerLayout->addStretch();

    QPushButton *menuBtn = new QPushButton("Меню", titleLabel);
    menuBtn->setCursor(Qt::PointingHandCursor);
    menuBtn->setFixedSize(80, 32);
    menuBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #6c528d;" 
        "   color: #ffffff;"
        "   font-weight: 600;"
        "   border: none;"
        "   border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #553e72;"
        "}"
    );
    stackedWidget->setContentsMargins(0, 0, 0, 0);
    connect(menuBtn, &QPushButton::clicked, this, &main_parent_window::toggleMenu);
    headerLayout->addWidget(menuBtn);
    titleLabel->setLayout(headerLayout);
}

void main_parent_window::toggleMenu() {
    if (sideMenu->isVisible()) {
        hideMenu();
    } else {
        showMenu();
    }
}

void main_parent_window::showMenu() {
    sideMenu->show();
    int x = this->width() - sideMenu->width();
    sideMenu->move(x, 0);
    sideMenu->raise(); 
}

void main_parent_window::hideMenu() {
    sideMenu->hide();
}

void main_parent_window::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (sideMenu != nullptr) {
        int menuWidth = sideMenu->width();
        int windowHeight = this->height();
        
        // Позиционируем меню
        sideMenu->setGeometry(this->width() - menuWidth, 0, menuWidth, windowHeight);
        
        // Дополнительно: если меню скрыто (isVisible() == false), 
        // его лучше не трогать, или убедиться, что raise() не вызывает проблем
        if (sideMenu->isVisible()) {
            sideMenu->raise(); 
        }
    }
}

} // namespace roomsched::menu