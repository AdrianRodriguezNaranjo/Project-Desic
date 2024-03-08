#include "routes_user.h"
#include "bus_user.h"

static std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &arr)
{
    QJsonParseError err;
    const auto json = QJsonDocument::fromJson(arr, &err);
    if (err.error || !json.isObject())
        return std::nullopt;
    return json.object();
}

static void setCorsHeaders(QHttpServerResponse &response)
{
    response.setHeader("Access-Control-Allow-Origin", "http://localhost:3000");
    response.setHeader("Access-Control-Allow-Methods", "GET,HEAD,OPTIONS,POST,PUT,DELETE");
    response.setHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Authorization");
}

void Routes_User::setupRoutes(QHttpServer& httpServer)
{
    httpServer.route(
        "/v2/bus_user", QHttpServerRequest::Method::Get,
        [](const QHttpServerRequest &)
        {
            QSqlQuery query("SELECT * FROM bus_user");
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            QList<Bus_User> busUserList;
            while (query.next()) {
                Bus_User busUser;
                busUser.id = query.value("id").toLongLong();
                busUser.email = query.value("email").toString();
                busUser.password = query.value("password").toString();
                busUser.name = query.value("name").toString();
                busUserList.append(busUser);
            }
            QJsonArray jsonArray;
            for (const Bus_User &busUser : busUserList) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(busUser.id);
                jsonObject["email"] = busUser.email;
                jsonObject["password"] = busUser.password;
                jsonObject["name"] = busUser.name;
                jsonArray.append(jsonObject);
            }
            QJsonDocument jsonDocument(jsonArray);
            QByteArray responseData = jsonDocument.toJson();

            QHttpServerResponse response(responseData);
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/bus_user/<arg>", QHttpServerRequest::Method::Get,
        [](qint64 busUserId, const QHttpServerRequest &)
        {
            QString sqlQuery = QString("SELECT * FROM bus_user WHERE id = %1").arg(busUserId);
            QSqlQuery query(sqlQuery);
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            if (query.next()) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(query.value("id").toLongLong());
                jsonObject["email"] = query.value("email").toString();
                jsonObject["password"] = query.value("password").toString();
                jsonObject["name"] = query.value("name").toString();

                QJsonDocument jsonDocument(jsonObject);
                QByteArray responseData = jsonDocument.toJson();

                QHttpServerResponse response(responseData);
                setCorsHeaders(response);
                return response;
            } else {
                return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
            }
        });

    httpServer.route("/v2/bus_user", QHttpServerRequest::Method::Options,
                     [](const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");
                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/bus_user", QHttpServerRequest::Method::Post,
        [](const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("credentials") || !json->contains("name"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QString credentials = json->value("credentials").toString();
            QStringList credentialParts = credentials.split(':');
            if (credentialParts.size() != 2) // Verificar que hay dos partes (correo electrónico y contraseña)
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QString email = credentialParts[0];
            QString password = credentialParts[1];

            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO bus_user (email, password, name) "
                                "VALUES (:email, :password, :name)");
            insertQuery.bindValue(":email", email);
            insertQuery.bindValue(":password", password);
            insertQuery.bindValue(":name", json->value("name").toString());
            if (!insertQuery.exec()) {
                qDebug() << "Error: Failed to insert bus user:" << insertQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route("/v2/bus_user/<arg>", QHttpServerRequest::Method::Options,
                     [](const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");
                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/bus_user/<arg>", QHttpServerRequest::Method::Put,
        [](qint64 busUserId, const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("email") || !json->contains("password") || !json->contains("name"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE bus_user SET email = :email, password = :password, name = :name WHERE id = :id");
            updateQuery.bindValue(":id", busUserId);
            updateQuery.bindValue(":email", json->value("email").toString());
            updateQuery.bindValue(":password", json->value("password").toString());
            updateQuery.bindValue(":name", json->value("name").toString());

            if (!updateQuery.exec()) {
                qDebug() << "Error: Failed to update bus user:" << updateQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/bus_user/<arg>", QHttpServerRequest::Method::Delete,
        [](qint64 busUserId, const QHttpServerRequest &)
        {
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM bus_user WHERE id = :id");
            deleteQuery.bindValue(":id", busUserId);
            if (!deleteQuery.exec()) {
                qDebug() << "Error: Failed to delete bus user:" << deleteQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });
}
