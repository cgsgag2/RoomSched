#include "client_main.hpp"
#include <QApplication>
#include "authwindow.hpp"
#include "roomlistwindow.hpp"

namespace roomsched {

int run_client(int argc, char *argv[]) {
    QApplication app(argc, argv);

    int test_case = 0;
    if (test_case == 0) {
        AuthWindow auth;
        auth.show();
        return app.exec();
    } else if (test_case == 1) {
        RoomListWindow roomList;
        roomList.show();
        return app.exec();
    }
    return app.exec();
}

}  // namespace roomsched
