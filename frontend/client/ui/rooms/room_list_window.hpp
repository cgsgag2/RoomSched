#ifndef ROOM_LIST_WINDOW_HPP_
#define ROOM_LIST_WINDOW_HPP_

#include <QCheckBox>
#include <QJsonArray>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class room_list_window;
}

QT_END_NAMESPACE

namespace roomsched::client::roomlistwindow {

class room_list_window : public QWidget {
    Q_OBJECT

public:
    explicit room_list_window(QWidget *parent = nullptr);
    room_list_window(
        ApiClient *existingApi,
        QString userName,
        QString userEmail,
        QString userPhone,
        QWidget *parent = nullptr,
        const QString &initialBuilding = {}
    );
    ~room_list_window();
    void updateViewForBuilding(const QString &buildingName);

private slots:
    void onRoomsLoaded(const QJsonArray &roomsArray);
    void onBuildingsLoaded(const QJsonArray &buildingsArray);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::room_list_window *ui;
    void updateGrid();
    void showRoomDetails(const QJsonObject &room);
    void applyFilters();
    void scheduleApplyFilters();
    void renderRooms(const QJsonArray &roomsArray);
    ApiClient *api;
    RoomFilters currentFilters;
    QJsonArray rooms;
    QList<QPushButton *> buttons;
    QTimer *resizeTimer;
    QTimer *filterTimer;
    QString initialBuildingName;
};

}  // namespace roomsched::client::roomlistwindow

#endif  // ROOM_LIST_WINDOW_HPP_
