#ifndef AUTHWINDOW_HPP
#define AUTHWINDOW_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
class AuthWindow;
}

QT_END_NAMESPACE

namespace roomsched {

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

private slots:
    void onLoginClicked();

private:
    Ui::AuthWindow *ui;
};

}  // namespace roomsched

#endif
