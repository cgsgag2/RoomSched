#ifndef REGISTERWINDOW_HPP_
#define REGISTERWINDOW_HPP_

#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class register_window; }
QT_END_NAMESPACE

namespace roomsched::registerwindow {

class register_window : public QWidget {
    Q_OBJECT

public:
    explicit register_window(QWidget *parent = nullptr);
    ~register_window();
    register_window(const register_window &) = delete;
    register_window &operator=(const register_window &) = delete;

signals:
    void backToLogin();

private:
    Ui::register_window *ui;
    roomsched::client::ApiClient *api;
    static bool check_name(QString enterName);
    static bool check_email(QString enterEmail);
    bool check_phone();
    void onRegisterButtonClicked();
};

} // namespace roomsched::registerwindow

#endif // REGISTERWINDOW_HPP_