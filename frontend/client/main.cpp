#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>

#include "auth_window.hpp"
#include "room_list_window.hpp"

namespace roomsched {

int run_client(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    auto *api = new roomsched::client::ApiClient();
    auto *auth = new roomsched::authwindow::auth_window(api);
    auth->show();    
    return app.exec();
}

}  // namespace roomsched

int main(int argc, char *argv[]) {
    return roomsched::run_client(argc, argv);
}