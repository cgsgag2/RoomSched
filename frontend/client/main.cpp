#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "LabelSelectorFilter.hpp"
#include "auth_window.hpp"
#include "room_list_window.hpp"

namespace roomsched::client {

int run_client(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.installEventFilter(new LabelSelectorFilter());
    app.setQuitOnLastWindowClosed(false);
    auto *api = new ApiClient();
    auto *auth = new authwindow::auth_window(api);
    auth->show();
    return app.exec();
}

}  // namespace roomsched::client

int main(int argc, char *argv[]) {
    return roomsched::client::run_client(argc, argv);
}
