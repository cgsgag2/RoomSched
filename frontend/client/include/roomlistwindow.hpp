#ifndef ROOMLISTWINDOW_HPP
#define ROOMLISTWINDOW_HPP

#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class RoomListWindow;
}

QT_END_NAMESPACE

namespace roomsched::roomlistwindow {

class RoomListWindow : public QWidget {
    Q_OBJECT

public:
    explicit RoomListWindow(QWidget *parent = nullptr);
    RoomListWindow(
        QString userName,
        QString userEmail,
        QString userPhone,
        QWidget *parent = nullptr
    );
    ~RoomListWindow();

private:
    Ui::RoomListWindow *ui;
    void setupRooms();
    void showRoomDetails(const QJsonObject &room);
    roomsched::client::ApiClient *api;
    QJsonArray rooms;
};

}  // namespace roomsched::roomlistwindow

#endif  // ROOMLISTWINDOW_HPP
