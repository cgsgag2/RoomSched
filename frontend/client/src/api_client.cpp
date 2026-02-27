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

void ApiClient::login(const QString &username, const QString &email) {
    QJsonObject body{
        {"username", username}, {"password", ""}, {"email", email}
    };
    sendPost(
        "/register", body, [this](QJsonObject obj) { emit loginSuccess(obj); },
        [this](QString err) { emit loginFailed(err); }
    );
}

void ApiClient::getRooms(int buildingId) {
    sendGet(
        QString("/buildings/%1/rooms").arg(buildingId),
        [this](QJsonObject obj) { emit roomsLoaded(obj["rooms"].toArray()); },
        [](QString) {}
    );
}

}  // namespace roomsched::client
