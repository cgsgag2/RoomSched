#include "api_client.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>

namespace roomsched::client {

static const QString BASE_URL = "http://localhost:8080";

ApiClient::ApiClient(QObject *parent) : QObject(parent) {
}

void ApiClient::sendPost(
    const QString &url,
    const QJsonObject &body,
    std::function<void(QJsonObject)> onSuccess,
    std::function<void(QString)> onError
) {
    QNetworkRequest req(BASE_URL + url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto reply = manager.post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply, onSuccess, onError]() {
        QByteArray raw = reply->readAll();
        qDebug() << "------------------------------------";
        qDebug() << "URL:" << reply->url().toString();
        qDebug() << "Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Raw Server Response:" << raw; 
        qDebug() << "------------------------------------";

        QJsonDocument doc = QJsonDocument::fromJson(raw);

        if (!doc.isObject()) {
            onError("Некорректный ответ сервера или ошибка валидации");
            reply->deleteLater();
            return;
        }
        
        QJsonObject obj = doc.object();
        if (obj["status"] == "success") {
            onSuccess(obj);
        } else {
            onError(obj["message"].toString());
        }
        
        reply->deleteLater();
    });
}

void ApiClient::sendGet(
    const QString &url,
    std::function<void(QJsonObject)> onSuccess,
    std::function<void(QString)> onError
) {
    QNetworkRequest req(BASE_URL + url);
    auto reply = manager.get(req);
    connect(reply, &QNetworkReply::finished, [reply, onSuccess, onError]() {
        QByteArray raw = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(raw);
        if (!doc.isObject()) {
            onError("Некорректный ответ сервера");
            reply->deleteLater();
            return;
        }
        QJsonObject obj = doc.object();
        if (obj["status"] == "success") {
            onSuccess(obj);
        } else {
            onError(obj["message"].toString());
        }
        reply->deleteLater();
    });
}

void ApiClient::registerUser(const QString &fullname, const QString &email, const QString &phone, const QString &password) {
    QJsonObject body{
        {"fullname", fullname}, 
        {"email", email},
        {"phone", phone},
        {"password", password}
    };

    sendPost("/register", body, [this](QJsonObject obj) {
        emit registrationFinished(true, "Регистрация успешна"); 
    }, [this](QString err) {
        emit registrationFinished(false, err);
    });
}

void ApiClient::login(const QString &email, const QString &password) {
    QJsonObject body;
    body["email"] = email.trimmed();
    body["password"] = password.trimmed();

    sendPost("/login", body, 
        [this](QJsonObject response) {
            emit loginSuccess(response); 
        }, 
        [this](QString error) {
            emit loginFailed(error);
        }
    );
}

void ApiClient::getRooms(int buildingId) {
    sendGet("/rooms", [this](QJsonObject obj) {
        if (obj.contains("rooms") && obj["rooms"].isArray()) {
            emit roomsLoaded(obj["rooms"].toArray());
        }
    }, [this](QString err) {
        emit loginFailed("Не удалось загрузить комнаты: " + err);
    });
}

void ApiClient::bookRoom(int roomId, const QString &date, const QString &start, const QString &end) {
    QJsonObject body{
        {"room_id", roomId},
        {"user_id", 1}, // Пока оставляем 1 для MVP, как в серверных тестах
        {"date", date},
        {"start_time", start},
        {"end_time", end}
    };

    sendPost("/bookings", body, [this](QJsonObject obj) {
        emit bookingFinished(true, "Успешно забронировано!");
    }, [this](QString err) {
        emit bookingFinished(false, "Ошибка бронирования: " + err);
    });
}

}  // namespace roomsched::client