#ifndef AUTH_WINDOW_HPP_
#define AUTH_WINDOW_HPP_

#include <QObject>
#include <QString>
#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class auth_window;
}

QT_END_NAMESPACE

namespace roomsched::client::authwindow {

class auth_window : public QWidget {
    Q_OBJECT

public:
    auth_window(
        roomsched::client::ApiClient *existingApi,
        QWidget *parent = nullptr
    );
    ~auth_window();
    auth_window(const auth_window &) = delete;
    auth_window &operator=(const auth_window &) = delete;

private:
    static bool check_name(QString enterName);
    static bool check_email(QString enterEmail);
    void on_login_clicked();
    Ui::auth_window *ui;
    roomsched::client::ApiClient *api;
};

}  // namespace roomsched::client::authwindow

#endif  // AUTH_WINDOW_HPP_
