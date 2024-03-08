#include "routes_bus_stop.h"
#include "bus_stop.h"

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

void Routes_Bus_Stop::setupRoutes(QHttpServer& httpServer)
{
    httpServer.route(
        "/v2/bus_stop", QHttpServerRequest::Method::Get,
        [](const QHttpServerRequest &)
        {
            QSqlQuery query("SELECT * FROM bus_stop");
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                // En caso de error, podrías devolver una respuesta de error apropiada
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            QList<Bus_Stop> busStopList;
            while (query.next()) {
                Bus_Stop busStop;
                busStop.id = query.value("id").toLongLong();
                busStop.line_id = query.value("line_id").toLongLong();
                busStop.location = query.value("location").toString();
                busStopList.append(busStop);
            }

            // Construye la respuesta JSON
            QJsonArray jsonArray;
            for (const Bus_Stop &busStop : busStopList) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(busStop.id);
                jsonObject["line_id"] = static_cast<qint64>(busStop.line_id);
                jsonObject["location"] = busStop.location;
                jsonArray.append(jsonObject);
            }
            QJsonDocument jsonDocument(jsonArray);
            QByteArray responseData = jsonDocument.toJson();

            // Construye la respuesta HTTP
            QHttpServerResponse response(responseData);
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/bus_stop/<arg>", QHttpServerRequest::Method::Get,
        [](qint64 busStopId, const QHttpServerRequest &)
        {
            QString sqlQuery = QString("SELECT * FROM bus_stop WHERE id = %1").arg(busStopId);
            QSqlQuery query(sqlQuery);
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                // En caso de error, podrías devolver una respuesta de error apropiada
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            if (query.next()) {
                // Construir el objeto JSON con los datos de la parada de autobús
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(query.value("id").toLongLong());
                jsonObject["line_id"] = static_cast<qint64>(query.value("line_id").toLongLong());
                jsonObject["location"] = query.value("location").toString();
                jsonObject["imagenFilePath"] = query.value("imagenFilePath").toString();

                // Construir la respuesta JSON
                QJsonDocument jsonDocument(jsonObject);
                QByteArray responseData = jsonDocument.toJson();

                // Construir la respuesta HTTP
                QHttpServerResponse response(responseData);
                setCorsHeaders(response);
                return response;
            } else {
                // Si no hay resultados, devolver una respuesta indicando que no se encontraron datos
                return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
            }
        });

    httpServer.route("/v2/bus_stop", QHttpServerRequest::Method::Options,
                     [](const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");
                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/bus_stop", QHttpServerRequest::Method::Post,
        [](const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("line_id") || !json->contains("location"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
            QSharedPointer<Bus_Stop> newBusStop(new Bus_Stop());
            newBusStop->line_id = json->value("line_id").toInt();
            newBusStop->location = json->value("location").toString();
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO bus_stop (line_id, location) "
                                "VALUES (:line_id, :location)");
            insertQuery.bindValue(":line_id", static_cast<int>(newBusStop->line_id));
            insertQuery.bindValue(":location", newBusStop->location);
            if (!insertQuery.exec()) {
                qDebug() << "Error: Failed to insert bus stop:" << insertQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/bus_stop/<arg>", QHttpServerRequest::Method::Options,
        [](qint64 busStopId, const QHttpServerRequest &)
        {
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/bus_stop/<arg>", QHttpServerRequest::Method::Put,
        [](qint64 busStopId, const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("line_id") || !json->contains("location"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE bus_stop SET line_id = :line_id, location = :location WHERE id = :id");
            updateQuery.bindValue(":id", busStopId);
            updateQuery.bindValue(":line_id", json->value("line_id").toInt());
            updateQuery.bindValue(":location", json->value("location").toString());
            if (!updateQuery.exec()) {
                qDebug() << "Error: Failed to update bus stop:" << updateQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/bus_stop/<arg>", QHttpServerRequest::Method::Delete,
        [](qint64 busStopId, const QHttpServerRequest &request)
        {
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM bus_stop WHERE id = :id");
            deleteQuery.bindValue(":id", busStopId);
            if (!deleteQuery.exec()) {
                qDebug() << "Error: Failed to delete bus stop:" << deleteQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });


    httpServer.route(
        "/v2/line/<arg>/bus_stop", QHttpServerRequest::Method::Get,
        [](qint64 lineId, const QHttpServerRequest &)
        {
            QString sqlQuery = QString("SELECT * FROM bus_stop WHERE line_id = %1").arg(lineId);
            QSqlQuery query(sqlQuery);
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            QList<Bus_Stop> busStopList;
            while (query.next()) {
                Bus_Stop busStop;
                busStop.id = query.value("id").toLongLong();
                busStop.line_id = query.value("line_id").toLongLong();
                busStop.location = query.value("location").toString();
                busStopList.append(busStop);
            }

            QJsonArray jsonArray;
            for (const Bus_Stop &busStop : busStopList) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(busStop.id);
                jsonObject["line_id"] = static_cast<qint64>(busStop.line_id);
                jsonObject["location"] = busStop.location;
                jsonArray.append(jsonObject);
            }
            QJsonDocument jsonDocument(jsonArray);
            QByteArray responseData = jsonDocument.toJson();

            // Construye la respuesta HTTP
            QHttpServerResponse response(responseData);
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/line/<arg>/bus_stop/<arg>", QHttpServerRequest::Method::Get,
        [](qint64 lineId, qint64 busStopId, const QHttpServerRequest &)
        {
            QString sqlQuery = QString("SELECT * FROM bus_stop WHERE line_id = %1 AND id = %2").arg(lineId).arg(busStopId);
            QSqlQuery query(sqlQuery);
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            if (query.next()) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(query.value("id").toLongLong());
                jsonObject["line_id"] = static_cast<qint64>(query.value("line_id").toLongLong());
                jsonObject["location"] = query.value("location").toString();

                QJsonDocument jsonDocument(jsonObject);
                QByteArray responseData = jsonDocument.toJson();
                QHttpServerResponse response(responseData);
                setCorsHeaders(response);
                return response;
            } else {
                return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
            }
        });

    httpServer.route("/v2/line/<arg>/bus_stop", QHttpServerRequest::Method::Options,
                     [](qint64 lineId, const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");
                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/line/<arg>/bus_stop", QHttpServerRequest::Method::Post,
        [](qint64 lineId, const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("location"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSharedPointer<Bus_Stop> newBusStop(new Bus_Stop());
            newBusStop->line_id = lineId;
            newBusStop->location = json->value("location").toString();
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO bus_stop (line_id, location) "
                                "VALUES (:line_id, :location)");
            insertQuery.bindValue(":line_id", static_cast<int>(newBusStop->line_id));
            insertQuery.bindValue(":location", newBusStop->location);
            if (!insertQuery.exec()) {
                qDebug() << "Error: Failed to insert schedule:" << insertQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route("/v2/line/<arg>/bus_stop/<arg>", QHttpServerRequest::Method::Options,
                     [](qint64 lineId, qint64 busStopId, const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");
                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/line/<arg>/bus_stop/<arg>", QHttpServerRequest::Method::Put,
        [](qint64 lineId, qint64 busStopId, const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("location"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE bus_stop SET line_id = :line_id, location = :location WHERE id = :id");
            updateQuery.bindValue(":id", busStopId);
            updateQuery.bindValue(":line_id", lineId);
            updateQuery.bindValue(":location", json->value("location").toString());
            if (!updateQuery.exec()) {
                qDebug() << "Error executing query:" << updateQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    // Ruta para eliminar un horario existente
    httpServer.route(
        "/v2/line/<arg>/bus_stop/<arg>", QHttpServerRequest::Method::Delete,
        [](qint64 lineId, qint64 busStopId, const QHttpServerRequest &request)
        {
            QString sqlQuery = QString("DELETE FROM bus_stop WHERE id = %1 AND line_id = %2").arg(busStopId).arg(lineId);
            QSqlQuery deleteQuery(sqlQuery);
            QSqlError error = deleteQuery.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << deleteQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });
}

