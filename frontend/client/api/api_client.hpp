#ifndef API_CLIENT_HPP
#define API_CLIENT_HPP

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>

namespace roomsched::client {

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);
    void registerUser(const QString &fullname, const QString &email, const QString &phone, const QString &password);
    void login(const QString &email, const QString &password);
    void getRooms(int buildingId = 1);
    void bookRoom(int roomId, const QString &date, const QString &start, const QString &end);
    bool isAuthenticated() const { 
        return !m_token.isEmpty(); 
    }
    void logout() { 
        m_token.clear(); 
        m_currentUserId = -1; 
    }
    QString getToken() const { 
        return m_token; 
    }

signals:
    void loginSuccess(QJsonObject data);
    void loginFailed(QString error);
    void registrationFinished(bool success, QString message);
    void roomsLoaded(QJsonArray rooms);
    void bookingFinished(bool success, QString message);

private:
    int m_currentUserId = -1;
    QString m_token;
    QNetworkAccessManager manager;
    void sendPost(
        const QString &url,
        const QJsonObject &body,
        std::function<void(QJsonObject)> onSuccess,
        std::function<void(QString)> onError,
        bool attachToken = true
    );
    void sendGet(
        const QString &url,
        std::function<void(QJsonObject)> onSuccess,
        std::function<void(QString)> onError,
        bool attachToken = true
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
