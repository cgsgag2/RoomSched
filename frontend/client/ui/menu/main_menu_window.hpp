#ifndef MAIN_MENU_WINDOW_HPP_
#define MAIN_MENU_WINDOW_HPP_

#include <QWidget>
#include <QJsonArray>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class main_menu_window;
}

QT_END_NAMESPACE

namespace roomsched::mainmenu {

class main_menu_window : public QWidget {
    Q_OBJECT

public:
    main_menu_window(
        roomsched::client::ApiClient *existingApi,
        const QString &userEmail,
        QWidget *parent = nullptr
    );
    ~main_menu_window();

signals:
    void buildingSelected(const QString &buildingName);

private slots:
    void onBuildingsLoaded(const QJsonArray &buildingsArray);
    void onContinueClicked();

private:
    Ui::main_menu_window *ui;
    roomsched::client::ApiClient *api;
    QString email;
};

}  // namespace roomsched::mainmenu

#endif  // MAIN_MENU_WINDOW_HPP_

