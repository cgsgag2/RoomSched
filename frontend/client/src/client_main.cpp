#include "client_main.hpp"
#include <QApplication>
#include "authwindow.h"
#include "roomlistwindow.h"

namespace roomsched {

int run_client(int argc, char *argv[]) {
    QApplication app(argc, argv);

    int test_case = 1;
    if (test_case == 0) {
        AuthWindow auth;
        auth.show();
        return app.exec();
    } else if (test_case == 1) {
        RoomListWindow roomList;
        roomList.show();
        return app.exec();
    }
}

}  // namespace roomsched
