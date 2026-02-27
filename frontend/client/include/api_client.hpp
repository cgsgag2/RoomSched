#ifndef API_CLIENT_HPP
#define API_CLIENT_HPP

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

namespace roomsched::client {

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);
    void login(const QString &username, const QString &email);
    void getRooms(int buildingId = 1);

signals:
    void loginSuccess(QJsonObject data);
    void loginFailed(QString error);
    void roomsLoaded(QJsonArray rooms);

private:
    QNetworkAccessManager manager;
    void sendPost(
        const QString &url,
        const QJsonObject &body,
        std::function<void(QJsonObject)> onSuccess,
        std::function<void(QString)> onError
    );
    void sendGet(
        const QString &url,
        std::function<void(QJsonObject)> onSuccess,
        std::function<void(QString)> onError
    );
};

}  // namespace roomsched::client

#endif  // API_CLIENT_HPP
