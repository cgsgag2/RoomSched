#ifndef REGISTERWINDOW_HPP
#define REGISTERWINDOW_HPP

#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterWindow; }
QT_END_NAMESPACE

namespace roomsched::registerwindow {

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();
    RegisterWindow(const RegisterWindow &) = delete;
    RegisterWindow &operator=(const RegisterWindow &) = delete;

private:
    Ui::RegisterWindow *ui;
    roomsched::client::ApiClient *api;
    static bool checkName(QString enterName);
    static bool checkEmail(QString enterEmail);
    bool checkPhone();
    void onRegisterButtonClicked();
};

} // namespace roomsched::registerwindow

#endif // REGISTERWINDOW_HPP