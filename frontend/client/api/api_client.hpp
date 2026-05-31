#ifndef API_CLIENT_HPP
#define API_CLIENT_HPP

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QDate>
#include <QTime>
#include <optional>

namespace roomsched::client {

struct RoomFilters {
    std::optional<QString> building;
    std::optional<QString> type;
    std::optional<int> capacityMin;
    std::optional<int> capacityMax;
    std::optional<bool> hasProjector;
    std::optional<bool> hasWhiteboard;
    std::optional<bool> hasWifi;
    std::optional<bool> hasPrinters;
    std::optional<bool> hasPhone;
    std::optional<QDate> date;
    std::optional<QTime> startTime;
    std::optional<QTime> endTime;
};

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);
    void registerUser(const QString &fullname, const QString &email, const QString &phone, const QString &password);
    void login(const QString &email, const QString &password);
    void getRooms(const RoomFilters &filters = {});
    void getBuildings();
    void bookRoom(int roomId, const QString &date, const QString &start, const QString &end);

signals:
    void loginSuccess(QJsonObject data);
    void loginFailed(QString error);
    void registrationFinished(bool success, QString message);
    void roomsLoaded(QJsonArray rooms);
    void buildingsLoaded(QJsonArray buildings);
    void bookingFinished(bool success, QString message);

private:
    int m_currentUserId = -1;
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
    static QString mapErrorCode(const QString &code);
    static QString formatError(
        const QJsonObject &obj,
        int statusCode,
        const QString &fallback
    );
    static QString formatError(
        QNetworkReply *reply,
        int statusCode,
        const QByteArray &raw
    );
};

}  // namespace roomsched::client

#endif  // API_CLIENT_HPP
