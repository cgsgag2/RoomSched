#ifndef AUTHWINDOW_HPP_
#define AUTHWINDOW_HPP_

#include <QObject>
#include <QString>
#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class auth_window;
}

QT_END_NAMESPACE

namespace roomsched::authwindow {

class auth_window : public QWidget {
    Q_OBJECT

public:
    explicit auth_window(QWidget *parent = nullptr);
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

}  // namespace roomsched::authwindow

#endif
