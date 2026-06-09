#include "main_parent_window.hpp"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QVBoxLayout>

namespace roomsched::client::menu {

main_parent_window::main_parent_window(
    ApiClient *existingApi,
    const QString &userEmail,
    int userId,
    QWidget *parent
)
    : QWidget(parent), api(existingApi) {
    resize(800, 550);
    setWindowTitle("RoomSched");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("background-color: #c9bfd4;");
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
    homeWindow = new mainmenu::main_menu_window(existingApi, userEmail, this);
    roomsWindow = new roomlistwindow::room_list_window(
        existingApi, "", userEmail, "", this
    );
    existingApi->getRooms();
    bookingsWindow =
        new bookings::user_bookings_window(existingApi, userId, this);
    telegramWindow =
        new telegram::telegram_binding_window(existingApi, userId, this);

    setupWindowHeader(homeWindow, "Добро пожаловать!");
    setupWindowHeader(roomsWindow, "Выбор аудитории");
    setupWindowHeader(bookingsWindow, "Мои бронирования");
    setupWindowHeader(telegramWindow, "Привязка Telegram");

    stackedWidget->addWidget(homeWindow);
    stackedWidget->addWidget(roomsWindow);
    stackedWidget->addWidget(bookingsWindow);
    stackedWidget->addWidget(telegramWindow);

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

    connect(sideMenu, &side_menu_widget::navigateToTelegram, this, [this]() {
        stackedWidget->setCurrentWidget(telegramWindow);
        hideMenu();
    });

    connect(
        homeWindow, &mainmenu::main_menu_window::buildingSelected, this,
        [this](const QString &buildingName) {
            roomsWindow->updateViewForBuilding(buildingName);
            stackedWidget->setCurrentIndex(1);
            roomsWindow->update();
            roomsWindow->show();
        }
    );

    connect(
        stackedWidget, &QStackedWidget::currentChanged, this,
        [this](int index) {
            if (index == 2) {
                bookingsWindow->loadBookings();
            }
        }
    );

    connect(
        sideMenu, &side_menu_widget::logoutRequested, this,
        &main_parent_window::handleLogout
    );
}

main_parent_window::~main_parent_window() {
}

void main_parent_window::setupWindowHeader(
    QWidget *window,
    const QString &titleText
) {
    QLabel *titleLabel = window->findChild<QLabel *>("titleLabel");
    if (!titleLabel) {
        return;
    }
    if (titleLabel->layout()) {
        qDeleteAll(titleLabel->children());
        delete titleLabel->layout();
    }
    titleLabel->setText("");
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLabel->setFixedHeight(80);
    titleLabel->setStyleSheet(
        "background-color: #423358;"
        "border-radius: 8px;"
    );

    QHBoxLayout *headerLayout = new QHBoxLayout(titleLabel);
    headerLayout->setContentsMargins(150, 5, 15, 5);

    QLabel *textLabel = new QLabel(titleText, titleLabel);
    textLabel->setStyleSheet(
        "color: #ffffff; font-size: 22px; font-weight: 600; background: "
        "transparent;"
    );
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
    connect(
        menuBtn, &QPushButton::clicked, this, &main_parent_window::toggleMenu
    );
    headerLayout->addWidget(menuBtn, 0, Qt::AlignVCenter);
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
        sideMenu->setGeometry(
            this->width() - menuWidth, 0, menuWidth, windowHeight
        );
        if (sideMenu->isVisible()) {
            sideMenu->raise();
        }
    }
}

void main_parent_window::handleLogout() {
    if (api) {
        api->clearSession();
    }
    emit logoutSuccessful();
    this->close();
}

}  // namespace roomsched::client::menu
