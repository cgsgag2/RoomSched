#ifndef AUTHWINDOW_HPP
#define AUTHWINDOW_HPP

#include <QObject>
#include <QString>
#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class AuthWindow;
}

QT_END_NAMESPACE

namespace roomsched::authwindow {

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();
    AuthWindow(const AuthWindow &) = delete;
    AuthWindow &operator=(const AuthWindow &) = delete;

private:
    static bool checkName(QString enterName);
    static bool checkEmail(QString enterEmail);
    bool checkPhone();
    void onLoginClicked();
    Ui::AuthWindow *ui;
    roomsched::client::ApiClient *api;
};

}  // namespace roomsched::authwindow

#endif
