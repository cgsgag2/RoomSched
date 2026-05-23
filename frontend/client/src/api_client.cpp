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
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(raw);
            onSuccess(doc.object());
        } else {
            QJsonDocument doc = QJsonDocument::fromJson(raw);
            if (doc.isObject() && doc.object().contains("message")) {
                onError(doc.object()["message"].toString());
            } 
            else if (!raw.isEmpty()) {
                onError(QString::fromUtf8(raw)); 
            } 
            else {
                onError(QString("Ошибка %1: %2").arg(statusCode).arg(reply->errorString()));
            }
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

    connect(reply, &QNetworkReply::finished, [this, reply, onSuccess, onError]() {
        QByteArray raw = reply->readAll();
        qDebug() << "GET" << reply->url().toString() << "Response:" << raw;

        QJsonDocument doc = QJsonDocument::fromJson(raw);
        if (doc.isNull()) {
            onError("Сервер прислал пустой ответ или не JSON");
            reply->deleteLater();
            return;
        }

        QJsonObject finalObj;
        if (doc.isArray()) {
            finalObj["rooms"] = doc.array();
            finalObj["status"] = "success"; 
        } 
        else if (doc.isObject()) {
            finalObj = doc.object();
        }
        if (!finalObj.contains("status") || finalObj["status"] == "success") {
            onSuccess(finalObj);
        } else {
            onError(finalObj["message"].toString());
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
            // Сохраняем ID, который пришел от сервера
            if (response.contains("user")) {
                m_currentUserId = response["user"].toObject()["id"].toInt();
            } else if (response.contains("id")) {
                m_currentUserId = response["id"].toInt();
            }
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
        qDebug() << "Rooms loading error:" << err;
    });
}

void ApiClient::bookRoom(int roomId, const QString &date, const QString &start, const QString &end) {
    QJsonObject body;
    body["room_id"] = roomId;
    body["user_id"] = m_currentUserId;
    body["booking_date"] = date; 
    body["start_time"] = start;
    body["end_time"] = end;
    qDebug() << "Sending JSON:" << QJsonDocument(body).toJson(QJsonDocument::Compact);

    sendPost("/book-room", body, [this](QJsonObject obj) {
        emit bookingFinished(true, "Успешно забронировано!");
    }, [this](QString err) {
        if (err.contains("409")) {
            emit bookingFinished(false, "Это время уже занято. Выберите другое.");
        } else {
            emit bookingFinished(false, "Ошибка: " + err);
        }
    });
}

}  // namespace roomsched::client