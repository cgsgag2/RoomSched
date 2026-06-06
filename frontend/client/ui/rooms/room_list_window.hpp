#ifndef ROOMLISTWINDOW_HPP_
#define ROOMLISTWINDOW_HPP_

#include <QWidget>
#include "api_client.hpp"
#include <QJsonArray>
#include <QPushButton>
#include <QTimer>
#include <QSpinBox>
#include <QCheckBox>

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
    roomsched::client::ApiClient *api;
    roomsched::client::RoomFilters currentFilters;
    QJsonArray rooms;
    QList<QPushButton*> buttons;
    QTimer *resizeTimer;
    QTimer *filterTimer;
    QString initialBuildingName;
};

}  // namespace roomsched::roomlistwindow

#endif  // ROOMLISTWINDOW_HPP_
