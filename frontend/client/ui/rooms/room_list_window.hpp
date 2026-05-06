#ifndef ROOMLISTWINDOW_HPP_
#define ROOMLISTWINDOW_HPP_

#include <QWidget>
#include "api_client.hpp"
#include <QJsonArray>

QT_BEGIN_NAMESPACE

namespace Ui {
class room_list_window;
}

QT_END_NAMESPACE

namespace roomsched::roomlistwindow {

class room_list_window : public QWidget {
    Q_OBJECT

public:
    explicit room_list_window(QWidget *parent = nullptr);
    room_list_window(
        roomsched::client::ApiClient *existingApi,
        QString userName,
        QString userEmail,
        QString userPhone,
        QWidget *parent = nullptr
    );
    ~room_list_window();

private slots:
    void onRoomsLoaded(const QJsonArray &roomsArray);

private:
    Ui::room_list_window *ui;
    void setupRooms();
    void showRoomDetails(const QJsonObject &room);
    roomsched::client::ApiClient *api;
    QJsonArray rooms;
};

}  // namespace roomsched::roomlistwindow

#endif  // ROOMLISTWINDOW_HPP_
