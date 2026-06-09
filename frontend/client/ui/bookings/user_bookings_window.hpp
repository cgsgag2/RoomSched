#ifndef USER_BOOKINGS_WINDOW_HPP_
#define USER_BOOKINGS_WINDOW_HPP_

#include <QJsonArray>
#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class user_bookings_window;
}

QT_END_NAMESPACE

namespace roomsched::client::bookings {

class user_bookings_window : public QWidget {
    Q_OBJECT

public:
    explicit user_bookings_window(
        ApiClient *existingApi,
        int userId,
        QWidget *parent = nullptr
    );
    ~user_bookings_window();

    void loadBookings();

private slots:
    void onBookingsLoaded(const QJsonArray &bookingsArray);
    void onCancelResponse(bool success, const QString &message);
    void onCancelButtonClicked();

private:
    Ui::user_bookings_window *ui;
    ApiClient *api;
    int currentUserId;
};

}  // namespace roomsched::client::bookings

#endif  // USER_BOOKINGS_WINDOW_HPP_
