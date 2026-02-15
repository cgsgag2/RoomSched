#include "client_main.hpp"
#include <QApplication>
#include <QLabel>

namespace roomsched {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle("RoomSched - Client");

    this->resize(400, 300);
    QLabel *label = new QLabel("Добро пожаловать в RoomSched!", this);
    label->setGeometry(0, 0, 400, 300);
    label->setAlignment(Qt::AlignCenter);
}

}  // namespace roomsched

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    roomsched::MainWindow window;
    window.show();
    return app.exec();
}
