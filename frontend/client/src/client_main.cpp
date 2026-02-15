#include "client_main.hpp"
#include <QApplication>
#include "authwindow.h"

namespace roomsched {

int run_client(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создаем наше окно авторизации
    AuthWindow auth;
    auth.show();

    // Запускаем бесконечный цикл обработки событий Qt
    return app.exec();
}

}  // namespace roomsched
