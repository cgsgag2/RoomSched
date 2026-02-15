#ifndef ROOMLISTWINDOW_HPP
#define ROOMLISTWINDOW_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
class RoomListWindow;
}

QT_END_NAMESPACE

namespace roomsched {

class RoomListWindow : public QWidget {
    Q_OBJECT

public:
    explicit RoomListWindow(QWidget *parent = nullptr);
    ~RoomListWindow();

private:
    Ui::RoomListWindow *ui;
    void setupRooms();
    void showRoomDetails(const QString &roomName);
};

}  // namespace roomsched

#endif  // ROOMLISTWINDOW_HPP
