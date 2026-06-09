#ifndef TELEGRAM_BINDING_WINDOW_HPP_
#define TELEGRAM_BINDING_WINDOW_HPP_

#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class telegram_binding_window;
}

QT_END_NAMESPACE

namespace roomsched::client::telegram {

class telegram_binding_window : public QWidget {
    Q_OBJECT

public:
    explicit telegram_binding_window(
        ApiClient *existingApi,
        int userId,
        QWidget *parent = nullptr
    );

    ~telegram_binding_window();

private slots:
    void onGetCodeClicked();

private:
    Ui::telegram_binding_window *ui;
    ApiClient *api;
    int currentUserId;
};

}  // namespace roomsched::client::telegram

#endif  // TELEGRAM_BINDING_WINDOW_HPP_
