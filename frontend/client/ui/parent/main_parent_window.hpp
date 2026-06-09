#ifndef MAIN_PARENT_WINDOW_HPP_
#define MAIN_PARENT_WINDOW_HPP_

#include <QStackedWidget>
#include <QWidget>
#include "api_client.hpp"
#include "main_menu_window.hpp"
#include "room_list_window.hpp"
#include "side_menu_widget.hpp"
#include "telegram_binding_window.hpp"
#include "user_bookings_window.hpp"

namespace roomsched::client::menu {

class main_parent_window : public QWidget {
    Q_OBJECT

public:
    explicit main_parent_window(
        ApiClient *existingApi,
        const QString &userEmail,
        int userId,
        QWidget *parent = nullptr
    );
    ~main_parent_window();

signals:
    void logoutSuccessful();

private slots:
    void toggleMenu();
    void showMenu();
    void hideMenu();
    void handleLogout();

private:
    QStackedWidget *stackedWidget;
    side_menu_widget *sideMenu;

    ApiClient *api;
    mainmenu::main_menu_window *homeWindow;
    roomlistwindow::room_list_window *roomsWindow;
    bookings::user_bookings_window *bookingsWindow;
    telegram::telegram_binding_window *telegramWindow;

    void setupWindowHeader(QWidget *window, const QString &titleText);
    void resizeEvent(QResizeEvent *event) override;
};

}  // namespace roomsched::client::menu

#endif  // MAIN_PARENT_WINDOW_HPP_
