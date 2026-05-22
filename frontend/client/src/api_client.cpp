#include "api_client.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>

namespace roomsched::client {

static const QString BASE_URL = "http://localhost:8080";

static QString mapStatusCode(int statusCode) {
    if (statusCode == 400) {
        return "Некорректный запрос. Проверьте введенные данные.";
    }
    if (statusCode == 401) {
        return "Неверный email или пароль.";
    }
    if (statusCode == 403) {
        return "Недостаточно прав для операции.";
    }
    if (statusCode == 404) {
        return "Ресурс не найден.";
    }
    if (statusCode == 409) {
        return "Конфликт данных. Попробуйте изменить параметры.";
    }
    if (statusCode == 422) {
        return "Некорректные данные. Проверьте поля.";
    }
    if (statusCode >= 500 && statusCode < 600) {
        return "Ошибка сервера. Попробуйте позже.";
    }
    return {};
}

QString ApiClient::mapErrorCode(const QString &code) {
    if (code == "INVALID_JSON") {
        return "Некорректный запрос к серверу.";
    }
    if (code == "MISSING_FIELDS") {
        return "Заполните все обязательные поля.";
    }
    if (code == "INVALID_EMAIL") {
        return "Неверный формат email.";
    }
    if (code == "WEAK_PASSWORD") {
        return "Пароль слишком слабый (минимум 8 символов, буква и цифра).";
    }
    if (code == "EMAIL_EXISTS") {
        return "Пользователь с таким email уже зарегистрирован.";
    }
    if (code == "PHONE_EXISTS") {
        return "Пользователь с таким телефоном уже зарегистрирован.";
    }
    if (code == "FULLNAME_EXISTS") {
        return "Пользователь с таким именем уже зарегистрирован.";
    }
    if (code == "INVALID_CREDENTIALS") {
        return "Неверный email или пароль.";
    }
    if (code == "BOOKING_CONFLICT") {
        return "Это время уже занято. Выберите другое.";
    }
    if (code == "INVALID_TIME_RANGE") {
        return "Некорректный интервал времени.";
    }
    if (code == "ROOM_NOT_FOUND") {
        return "Комната не найдена.";
    }
    if (code == "BOOKING_NOT_FOUND") {
        return "Бронирование не найдено.";
    }
    if (code == "REGISTRATION_FAILED") {
        return "Не удалось зарегистрировать пользователя. Попробуйте позже.";
    }
    if (code == "BAD_REQUEST") {
        return "Некорректный запрос. Проверьте введенные данные.";
    }
    if (code == "DATABASE_ERROR") {
        return "Ошибка базы данных. Попробуйте позже.";
    }
    if (code == "INTERNAL_ERROR") {
        return "Внутренняя ошибка сервера. Попробуйте позже.";
    }
    return {};
}

QString ApiClient::formatError(
    const QJsonObject &obj,
    int statusCode,
    const QString &fallback
) {
    const QString code = obj.value("code").toString();
    const QString mapped = mapErrorCode(code);
    if (!mapped.isEmpty()) {
        return mapped;
    }

    const QString message = obj.value("message").toString();
    if (!message.isEmpty()) {
        return message;
    }

    const QString statusMapped = mapStatusCode(statusCode);
    if (!statusMapped.isEmpty()) {
        return statusMapped;
    }

    if (!fallback.isEmpty()) {
        return fallback;
    }

    return QString("Ошибка %1").arg(statusCode);
}

QString ApiClient::formatError(
    QNetworkReply *reply,
    int statusCode,
    const QByteArray &raw
) {
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (doc.isObject()) {
        return formatError(doc.object(), statusCode, reply->errorString());
    }

    if (!raw.isEmpty()) {
        return QString::fromUtf8(raw);
    }

    return QString("Ошибка %1: %2").arg(statusCode).arg(reply->errorString());
}

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
        QJsonDocument doc = QJsonDocument::fromJson(raw);

        if (reply->error() != QNetworkReply::NoError) {
            onError(formatError(reply, statusCode, raw));
            reply->deleteLater();
            return;
        }

        if (!doc.isObject()) {
            onError("Сервер прислал пустой ответ или не JSON");
            reply->deleteLater();
            return;
        }

        QJsonObject obj = doc.object();
        if (obj.value("status").toString() == "error") {
            onError(formatError(obj, statusCode, reply->errorString()));
        } else {
            onSuccess(obj);
        }
        reply->deleteLater();
    });
}

void ApiClient::sendGet(
    const QString &url,
    std::function<void(QJsonObject)> onSuccess,
    std::function<void(QString)> onError,
    const QString &arrayKey
) {
    QNetworkRequest req(BASE_URL + url);
    auto reply = manager.get(req);

    connect(reply, &QNetworkReply::finished, [this, reply, onSuccess, onError, arrayKey]() {
        QByteArray raw = reply->readAll();
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "GET" << reply->url().toString() << "Response:" << raw;

        if (reply->error() != QNetworkReply::NoError) {
            onError(formatError(reply, statusCode, raw));
            reply->deleteLater();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(raw);
        if (doc.isNull()) {
            onError("Сервер прислал пустой ответ или не JSON");
            reply->deleteLater();
            return;
        }

        QJsonObject finalObj;
        if (doc.isArray()) {
            finalObj[arrayKey] = doc.array();
            finalObj["status"] = "success";
        } else if (doc.isObject()) {
            finalObj = doc.object();
        }

        if (finalObj.value("status").toString() == "error") {
            onError(formatError(finalObj, statusCode, reply->errorString()));
        } else {
            onSuccess(finalObj);
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
    Q_UNUSED(buildingId);
    sendGet("/rooms", [this](QJsonObject obj) {
        if (obj.contains("rooms") && obj["rooms"].isArray()) {
            emit roomsLoaded(obj["rooms"].toArray());
        }
    }, [this](QString err) {
        qDebug() << "Rooms loading error:" << err;
    }, "rooms");
}

void ApiClient::getBuildings() {
    sendGet("/buildings", [this](QJsonObject obj) {
        if (obj.contains("buildings") && obj["buildings"].isArray()) {
            emit buildingsLoaded(obj["buildings"].toArray());
        }
    }, [this](QString err) {
        qDebug() << "Buildings loading error:" << err;
    }, "buildings");
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
        emit bookingFinished(false, err);
    });
}

}  // namespace roomsched::client
