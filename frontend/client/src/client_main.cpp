#include "client_main.hpp"
#include <QApplication>
#include "auth_window.hpp"
#include "room_list_window.hpp"

namespace roomsched {

int run_client(int argc, char *argv[]) {
    QApplication app(argc, argv);

    int test_case = 0;
    if (test_case == 0) {
        roomsched::authwindow::auth_window auth;
        auth.show();
        return app.exec();
    } else if (test_case == 1) {
        roomsched::roomlistwindow::room_list_window room_list;
        room_list.show();
        return app.exec();
    }
    return app.exec();
}

}  // namespace roomsched
