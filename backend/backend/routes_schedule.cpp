#include "routes_schedule.h"
#include "database.h"
#include "schedule.h"

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

void Routes_Schedule::setupRoutes(QHttpServer& httpServer)
{
httpServer.route(
    "/v2/schedule", QHttpServerRequest::Method::Get,
    [](const QHttpServerRequest &)
    {
        QSqlQuery query("SELECT * FROM schedule");
        QSqlError error = query.lastError();
        if (error.type() != QSqlError::NoError) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }

        QList<Schedule> scheduleList;
        while (query.next()) {
            Schedule schedule;
            schedule.id = query.value("id").toInt();
            schedule.line_id = query.value("line_id").toInt();
            schedule.time = query.value("time").toTime();
            scheduleList.append(schedule);
        }

        // Construye la respuesta JSON
        QJsonArray jsonArray;
        for (const Schedule &schedule : scheduleList) {
            QJsonObject jsonObject;
            jsonObject["id"] = static_cast<qint64>(schedule.id);
            jsonObject["line_id"] = static_cast<qint64>(schedule.line_id);
            jsonObject["time"] = schedule.time.toString();
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
    "/v2/schedule/<arg>", QHttpServerRequest::Method::Get,
    [](qint64 scheduleId, const QHttpServerRequest &)
    {
        QString sqlQuery = QString("SELECT * FROM schedule WHERE id = %1").arg(scheduleId);
        QSqlQuery query(sqlQuery);
        QSqlError error = query.lastError();
        if (error.type() != QSqlError::NoError) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }

        if (query.next()) {
            QJsonObject jsonObject;
            jsonObject["id"] = static_cast<qint64>(query.value("id").toInt());
            jsonObject["line_id"] = static_cast<qint64>(query.value("line_id").toInt());
            jsonObject["time"] = query.value("time").toTime().toString();

            QJsonDocument jsonDocument(jsonObject);
            QByteArray responseData = jsonDocument.toJson();
            QHttpServerResponse response(responseData);
            setCorsHeaders(response);
            return response;
        } else {
            return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
        }
    });

httpServer.route("/v2/schedule", QHttpServerRequest::Method::Options,
                 [](const QHttpServerRequest &)
                 {
                     QHttpServerResponse response("");
                     setCorsHeaders(response);
                     return response;
                 });

httpServer.route(
    "/v2/schedule", QHttpServerRequest::Method::Post,
    [](const QHttpServerRequest &request)
    {
        const auto json = byteArrayToJsonObject(request.body());
        if (!json || !json->contains("line_id") || !json->contains("time"))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        QSharedPointer<Schedule> newSchedule(new Schedule());
        newSchedule->line_id = json->value("line_id").toInt();
        newSchedule->time = QTime::fromString(json->value("time").toString(), "hh:mm:ss");
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO schedule (line_id, time) "
                            "VALUES (:line_id, :time)");
        insertQuery.bindValue(":line_id", static_cast<int>(newSchedule->line_id));
        insertQuery.bindValue(":time", newSchedule->time.toString("hh:mm:ss"));
        if (!insertQuery.exec()) {
            qDebug() << "Error: Failed to insert schedule:" << insertQuery.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }
        QHttpServerResponse response("");
        setCorsHeaders(response);
        return response;
    });

httpServer.route("/v2/schedule/<arg>", QHttpServerRequest::Method::Options,
                 [](qint64 scheduleId, const QHttpServerRequest &)
                 {
                     QHttpServerResponse response("");
                     setCorsHeaders(response);
                     return response;
                 });

httpServer.route(
    "/v2/schedule/<arg>", QHttpServerRequest::Method::Put,
    [](qint64 scheduleId, const QHttpServerRequest &request)
    {
        const auto json = byteArrayToJsonObject(request.body());
        if (!json || !json->contains("line_id") || !json->contains("time"))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE schedule SET line_id = :line_id, time = :time WHERE id = :id");
        updateQuery.bindValue(":id", scheduleId);
        updateQuery.bindValue(":line_id", json->value("line_id").toInt());
        updateQuery.bindValue(":time", QTime::fromString(json->value("time").toString(), "hh:mm:ss"));

        if (!updateQuery.exec()) {
            qDebug() << "Error: Failed to update schedule:" << updateQuery.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }
        QHttpServerResponse response("");
        setCorsHeaders(response);
        return response;
    });

httpServer.route(
    "/v2/schedule/<arg>", QHttpServerRequest::Method::Delete,
    [](qint64 scheduleId, const QHttpServerRequest &request)
    {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM schedule WHERE id = :id");
        deleteQuery.bindValue(":id", scheduleId);
        if (!deleteQuery.exec()) {
            qDebug() << "Error: Failed to delete schedule:" << deleteQuery.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }
        QHttpServerResponse response("");
        setCorsHeaders(response);
        return response;
    });

httpServer.route(
    "/v2/line/<arg>/schedule", QHttpServerRequest::Method::Get,
    [](qint64 lineId, const QHttpServerRequest &)
    {
        QString sqlQuery = QString("SELECT * FROM schedule WHERE line_id = %1").arg(lineId);
        QSqlQuery query(sqlQuery);
        QSqlError error = query.lastError();
        if (error.type() != QSqlError::NoError) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }

        QList<Schedule> scheduleList;
        while (query.next()) {
            Schedule schedule;
            schedule.id = query.value("id").toInt();
            schedule.line_id = query.value("line_id").toInt();
            schedule.time = query.value("time").toTime();
            scheduleList.append(schedule);
        }

        // Construye la respuesta JSON
        QJsonArray jsonArray;
        for (const Schedule &schedule : scheduleList) {
            QJsonObject jsonObject;
            jsonObject["id"] = static_cast<qint64>(schedule.id);
            jsonObject["line_id"] = static_cast<qint64>(schedule.line_id);
            jsonObject["time"] = schedule.time.toString();
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
    "/v2/line/<arg>/schedule/<arg>", QHttpServerRequest::Method::Get,
    [](qint64 lineId, qint64 scheduleId, const QHttpServerRequest &)
    {
        QString sqlQuery = QString("SELECT * FROM schedule WHERE line_id = %1 AND id = %2").arg(lineId).arg(scheduleId);
        QSqlQuery query(sqlQuery);
        QSqlError error = query.lastError();
        if (error.type() != QSqlError::NoError) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }

        if (query.next()) {
            QJsonObject jsonObject;
            jsonObject["id"] = static_cast<qint64>(query.value("id").toInt());
            jsonObject["line_id"] = static_cast<qint64>(query.value("line_id").toInt());
            jsonObject["time"] = query.value("time").toTime().toString();

            QJsonDocument jsonDocument(jsonObject);
            QByteArray responseData = jsonDocument.toJson();
            QHttpServerResponse response(responseData);
            setCorsHeaders(response);
            return response;
        } else {
            return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
        }
    });

httpServer.route("/v2/line/<arg>/schedule", QHttpServerRequest::Method::Options,
                 [](qint64 lineId, const QHttpServerRequest &)
                 {
                     QHttpServerResponse response("");
                     setCorsHeaders(response);
                     return response;
                 });

httpServer.route(
    "/v2/line/<arg>/schedule", QHttpServerRequest::Method::Post,
    [](qint64 lineId, const QHttpServerRequest &request)
    {
        const auto json = byteArrayToJsonObject(request.body());
        if (!json || !json->contains("time"))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        QSharedPointer<Schedule> newSchedule(new Schedule());
        newSchedule->line_id = lineId;
        newSchedule->time = QTime::fromString(json->value("time").toString(), "hh:mm");
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO schedule (line_id, time) "
                            "VALUES (:line_id, :time)");
        insertQuery.bindValue(":line_id", static_cast<int>(newSchedule->line_id));
        insertQuery.bindValue(":time", newSchedule->time.toString("hh:mm"));
        if (!insertQuery.exec()) {
            qDebug() << "Error: Failed to insert schedule:" << insertQuery.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }
        QHttpServerResponse response("");
        setCorsHeaders(response);
        return response;
    });

httpServer.route("/v2/line/<arg>/schedule/<arg>", QHttpServerRequest::Method::Options,
                 [](qint64 lineId, qint64 scheduleId, const QHttpServerRequest &)
                 {
                     QHttpServerResponse response("");
                     setCorsHeaders(response);
                     return response;
                 });

httpServer.route(
    "/v2/line/<arg>/schedule/<arg>", QHttpServerRequest::Method::Put,
    [](qint64 lineId, qint64 scheduleId, const QHttpServerRequest &request)
    {
        const auto json = byteArrayToJsonObject(request.body());
        if (!json || !json->contains("time"))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE schedule SET time = :time WHERE id = :id AND line_id = :line_id");
        updateQuery.bindValue(":id", scheduleId);
        updateQuery.bindValue(":line_id", lineId);
        updateQuery.bindValue(":time", QTime::fromString(json->value("time").toString(), "hh:mm"));

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
    "/v2/line/<arg>/schedule/<arg>", QHttpServerRequest::Method::Delete,
    [](qint64 lineId, qint64 scheduleId, const QHttpServerRequest &request)
    {
        QString sqlQuery = QString("DELETE FROM schedule WHERE id = %1 AND line_id = %2").arg(scheduleId).arg(lineId);
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
