#ifndef SIDE_MENU_WIDGET_HPP_
#define SIDE_MENU_WIDGET_HPP_

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
class side_menu_widget;
}

QT_END_NAMESPACE

namespace roomsched::client::menu {

class side_menu_widget : public QWidget {
    Q_OBJECT

public:
    explicit side_menu_widget(QWidget *parent = nullptr);
    ~side_menu_widget();

signals:
    void navigateToHome();
    void navigateToRooms();
    void navigateToBookings();
    void navigateToTelegram();
    void logoutRequested();

private:
    Ui::side_menu_widget *ui;
};

}  // namespace roomsched::client::menu

#endif  // SIDE_MENU_WIDGET_HPP_
