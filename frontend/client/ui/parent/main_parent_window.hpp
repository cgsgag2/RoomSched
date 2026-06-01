#ifndef MAIN_PARENT_WINDOW_HPP_
#define MAIN_PARENT_WINDOW_HPP_

#include <QWidget>
#include <QStackedWidget>
#include "api_client.hpp"
#include "side_menu_widget.hpp"
#include "main_menu_window.hpp"
#include "user_bookings_window.hpp"
#include "room_list_window.hpp"

namespace roomsched::menu {

class main_parent_window : public QWidget {
    Q_OBJECT

public:
    explicit main_parent_window(
        roomsched::client::ApiClient *existingApi,
        const QString &userEmail,
        int userId,
        QWidget *parent = nullptr
    );
    ~main_parent_window();

private slots:
    void toggleMenu();
    void showMenu();
    void hideMenu();

private:
    QStackedWidget *stackedWidget;
    side_menu_widget *sideMenu;
    
    roomsched::mainmenu::main_menu_window *homeWindow;
    roomsched::roomlistwindow::room_list_window *roomsWindow; 
    roomsched::bookings::user_bookings_window *bookingsWindow; 

    void setupWindowHeader(QWidget *window, const QString &titleText);
    void resizeEvent(QResizeEvent *event) override;
};

} // namespace roomsched::menu

#endif // MAIN_PARENT_WINDOW_HPP_