#ifndef CLIENT_MAIN_HPP
#define CLIENT_MAIN_HPP

#include <QMainWindow>

namespace roomsched {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
};

}  // namespace roomsched

#endif
