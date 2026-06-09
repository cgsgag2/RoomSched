#ifndef MAIN_MENU_WINDOW_HPP_
#define MAIN_MENU_WINDOW_HPP_

#include <QJsonArray>
#include <QWidget>
#include "api_client.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class main_menu_window;
}

QT_END_NAMESPACE

namespace roomsched::client::mainmenu {

class main_menu_window : public QWidget {
    Q_OBJECT

public:
    main_menu_window(
        ApiClient *existingApi,
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
    ApiClient *api;
    QString email;
};

}  // namespace roomsched::client::mainmenu

#endif  // MAIN_MENU_WINDOW_HPP_
