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
    void registerUser(const QString &fullname, const QString &email, const QString &phone, const QString &password);
    void login(const QString &email, const QString &password);
    void getRooms(int buildingId = 1);
    void getBuildings();
    void bookRoom(int roomId, const QString &date, const QString &start, const QString &end);
    void getUserBookings(int userId);
    void cancelBooking(int bookingId);
    void logout();
    QJsonObject getRoomInfo(int roomId) const;
    void clearSession() {
        m_currentUserId = -1;
        m_token.clear();
        m_roomsCache.clear();
        emit roomsCacheUpdated();
    }

signals:
    void loginSuccess(QJsonObject data);
    void loginFailed(QString error);
    void registrationFinished(bool success, QString message);
    void roomsLoaded(QJsonArray rooms);
    void buildingsLoaded(QJsonArray buildings);
    void bookingFinished(bool success, QString message);
    void bookingsLoaded(const QJsonArray &bookings);
    void bookingCancelled(bool success, const QString &message);
    void roomsCacheUpdated();

private:
    int m_currentUserId = -1;
    QString m_token;
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
    QMap<int, QJsonObject> m_roomsCache;

};

}  // namespace roomsched::client

#endif  // API_CLIENT_HPP
