#include <QCoreApplication>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QDebug>
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QSqlQuery>
#include "line.h"
#include "schedule.h"
#include "bus_stop.h"
#include "bus_user.h"
#include "database.h"
#include "chatserver.h"
#include "user_lines.h"
// #include "http_server.h"

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

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Parameters to connect to the PostgreSQL database
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("bus");
    db.setUserName("postgres");
    db.setPassword("alumno");

    // Open the connection to the database
    if (!db.open()) {
        qDebug() << "Error: Failed to open the database connection:" << db.lastError().text();
        return 1;
    }

    // Create the tables in the PostgreSQL database
    if (!Database::createLineTable()) {
        return 1;
    }

    // HttpServer::startServer();
    QHttpServer httpServer;

    httpServer.route(
        "/", []()
        { return "Bus API using QxOrm & QtHttpServer"; });

    httpServer.route(
        "/v2/line", QHttpServerRequest::Method::Get,
        [](const QHttpServerRequest &)
        {
            QSqlQuery query("SELECT * FROM line");
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                // En caso de error, podrías devolver una respuesta de error apropiada
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            QList<Line> lineList;
            while (query.next()) {
                Line line;
                line.id = query.value("id").toInt();
                line.number = query.value("number").toInt();
                line.firstbusstop = query.value("firstbusstop").toString();
                line.lastbusstop = query.value("lastbusstop").toString();
                lineList.append(line);
            }

            // Construye la respuesta JSON
            QJsonArray jsonArray;
            for (const Line &line : lineList) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(line.id);
                jsonObject["number"] = static_cast<qint64>(line.number);
                jsonObject["firstbusstop"] = line.firstbusstop;
                jsonObject["lastbusstop"] = line.lastbusstop;
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
        "/v2/line/<arg>", QHttpServerRequest::Method::Get,
        [](qint64 lineId, const QHttpServerRequest &)
        {
            QString sqlQuery = QString("SELECT * FROM line WHERE id = %1").arg(lineId);
            QSqlQuery query(sqlQuery);
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                // En caso de error, podrías devolver una respuesta de error apropiada
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            if (query.next()) {
                // Construir el objeto JSON con los datos de la bicicleta
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(query.value("id").toInt());
                jsonObject["number"] = static_cast<qint64>(query.value("number").toInt());
                jsonObject["firstbusstop"] = query.value("firstbusstop").toString();
                jsonObject["lastbusstop"] = query.value("lastbusstop").toString();

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

    httpServer.route("/v2/line",QHttpServerRequest::Method::Options,
                     [](const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");

                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/line", QHttpServerRequest::Method::Post,
        [](const QHttpServerRequest &request)
        {
            qDebug() << "Valor request: " << request.body();
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("number") || !json->contains("firstbusstop")|| !json->contains("lastbusstop"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
            QSharedPointer<Line> new_line(new Line());
            const auto number = json->value("number").toString();
            new_line->number = number.toInt();
            // new_line->number = json->value("number").toInt();
            new_line->firstbusstop = json->value("firstbusstop").toString();
            new_line->lastbusstop = json->value("lastbusstop").toString();
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO line (number, firstbusstop, lastbusstop) "
                                "VALUES (:number, :firstbusstop, :lastbusstop)");
            insertQuery.bindValue(":number", static_cast<int>(new_line->number));
            insertQuery.bindValue(":firstbusstop", new_line->firstbusstop);
            insertQuery.bindValue(":lastbusstop", new_line->lastbusstop);
            if (!insertQuery.exec()) {
                qDebug() << "Error: Failed to insert line:" << insertQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            // QHttpServerResponse(QHttpServerResponder::StatusCode::Created);
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route("/v2/line/<arg>",QHttpServerRequest::Method::Options,
                     [](qint64 lineId, const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");

                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/line/<arg>", QHttpServerRequest::Method::Put,
        [](qint64 lineId, const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("number") || !json->contains("firstbusstop")|| !json->contains("lastbusstop"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE line SET number = :number, firstbusstop = :firstbusstop, lastbusstop = :lastbusstop WHERE id = :id");
            updateQuery.bindValue(":id", lineId);
            updateQuery.bindValue(":number", json->value("number").toString());
            updateQuery.bindValue(":firstbusstop", json->value("firstbusstop").toString());
            updateQuery.bindValue(":lastbusstop", json->value("lastbusstop").toString());
            if (!updateQuery.exec()) {
                qDebug() << "Error: Failed to update line:" << updateQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            // QHttpServerResponse response(QHttpServerResponder::StatusCode::Ok);
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/line/<arg>", QHttpServerRequest::Method::Delete,
        [](qint64 lineId, const QHttpServerRequest &request)
        {
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM line WHERE id = :id");
            deleteQuery.bindValue(":id", lineId);
            if (!deleteQuery.exec()) {
                qDebug() << "Error: Failed to delete line:" << deleteQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            // QHttpServerResponse response(QHttpServerResponder::StatusCode::Ok);
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

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
            updateQuery.bindValue(":time", QTime::fromString(json->value("time").toString(), "hh:mm:ss"));

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
                busStop.imagenFilePath = query.value("imagenFilePath").toString();
                busStopList.append(busStop);
            }

            // Construye la respuesta JSON
            QJsonArray jsonArray;
            for (const Bus_Stop &busStop : busStopList) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(busStop.id);
                jsonObject["line_id"] = static_cast<qint64>(busStop.line_id);
                jsonObject["location"] = busStop.location;
                jsonObject["imagenFilePath"] = busStop.imagenFilePath;
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
            if (!json || !json->contains("line_id") || !json->contains("location") || !json->contains("imagenFilePath"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
            QSharedPointer<Bus_Stop> newBusStop(new Bus_Stop());
            newBusStop->line_id = json->value("line_id").toInt();
            newBusStop->location = json->value("location").toString();
            newBusStop->imagenFilePath = json->value("imagenFilePath").toString();
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO bus_stop (line_id, location, imagenFilePath) "
                                "VALUES (:line_id, :location, :imagenFilePath)");
            insertQuery.bindValue(":line_id", static_cast<int>(newBusStop->line_id));
            insertQuery.bindValue(":location", newBusStop->location);
            insertQuery.bindValue(":imagenFilePath", newBusStop->imagenFilePath);
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
            if (!json || !json->contains("line_id") || !json->contains("location") || !json->contains("imagenFilePath"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE bus_stop SET line_id = :line_id, location = :location, imagenFilePath = :imagenFilePath WHERE id = :id");
            updateQuery.bindValue(":id", busStopId);
            updateQuery.bindValue(":line_id", json->value("line_id").toInt());
            updateQuery.bindValue(":location", json->value("location").toString());
            updateQuery.bindValue(":imagenFilePath", json->value("imagenFilePath").toString());
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
                busStop.imagenFilePath = query.value("imagenFilePath").toString();
                busStopList.append(busStop);
            }

            QJsonArray jsonArray;
            for (const Bus_Stop &busStop : busStopList) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(busStop.id);
                jsonObject["line_id"] = static_cast<qint64>(busStop.line_id);
                jsonObject["location"] = busStop.location;
                jsonObject["imagenFilePath"] = busStop.imagenFilePath;
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
                jsonObject["imagenFilePath"] = query.value("imagenFilePath").toString();

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
            if (!json || !json->contains("location") || !json->contains("imagenFilePath"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSharedPointer<Bus_Stop> newBusStop(new Bus_Stop());
            newBusStop->line_id = lineId;
            newBusStop->location = json->value("location").toString();
            newBusStop->imagenFilePath = json->value("imagenFilePath").toString();
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO bus_stop (line_id, location, imagenFilePath) "
                                "VALUES (:line_id, :location, :imagenFilePath)");
            insertQuery.bindValue(":line_id", static_cast<int>(newBusStop->line_id));
            insertQuery.bindValue(":location", newBusStop->location);
            insertQuery.bindValue(":imagenFilePath", newBusStop->imagenFilePath);
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
            if (!json || !json->contains("location") || !json->contains("imagenFilePath"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE bus_stop SET line_id = :line_id, location = :location, imagenFilePath = :imagenFilePath WHERE id = :id");
            updateQuery.bindValue(":id", busStopId);
            updateQuery.bindValue(":line_id", lineId);
            updateQuery.bindValue(":location", json->value("location").toString());
            updateQuery.bindValue(":imagenFilePath", json->value("imagenFilePath").toString());
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
            busUser.username = query.value("username").toString();
            busUser.password = query.value("password").toString();
            busUser.name = query.value("name").toString();
            busUserList.append(busUser);
        }
        QJsonArray jsonArray;
        for (const Bus_User &busUser : busUserList) {
            QJsonObject jsonObject;
            jsonObject["id"] = static_cast<qint64>(busUser.id);
            jsonObject["username"] = busUser.username;
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
            jsonObject["username"] = query.value("username").toString();
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

httpServer.route(
    "/v2/bus_user", QHttpServerRequest::Method::Post,
    [](const QHttpServerRequest &request)
    {
        const auto json = byteArrayToJsonObject(request.body());
        if (!json || !json->contains("username") || !json->contains("password") || !json->contains("name"))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO bus_user (username, password, name) "
                            "VALUES (:username, :password, :name)");
        insertQuery.bindValue(":username", json->value("username").toString());
        insertQuery.bindValue(":password", json->value("password").toString());
        insertQuery.bindValue(":name", json->value("name").toString());
        if (!insertQuery.exec()) {
            qDebug() << "Error: Failed to insert bus user:" << insertQuery.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }
        QHttpServerResponse response("");
        setCorsHeaders(response);
        return response;
    });

httpServer.route(
    "/v2/bus_user/<arg>", QHttpServerRequest::Method::Put,
    [](qint64 busUserId, const QHttpServerRequest &request)
    {
        const auto json = byteArrayToJsonObject(request.body());
        if (!json || !json->contains("username") || !json->contains("password") || !json->contains("name"))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE bus_user SET username = :username, password = :password, name = :name WHERE id = :id");
        updateQuery.bindValue(":id", busUserId);
        updateQuery.bindValue(":username", json->value("username").toString());
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

httpServer.route(
    "/v2/user_lines", QHttpServerRequest::Method::Post,
    [](const QHttpServerRequest &request)
    {
        const auto json = byteArrayToJsonObject(request.body());
        if (!json || !json->contains("user_id") || !json->contains("line_id"))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO user_lines (user_id, line_id) "
                            "VALUES (:user_id, :line_id)");
        insertQuery.bindValue(":user_id", json->value("user_id").toInt());
        insertQuery.bindValue(":line_id", json->value("line_id").toInt());
        if (!insertQuery.exec()) {
            qDebug() << "Error: Failed to insert user-line relationship:" << insertQuery.lastError().text();
            return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
        }
        QHttpServerResponse response("");
        setCorsHeaders(response);
        return response;
    });

    const auto port = httpServer.listen(QHostAddress::Any, 49080);
    if (!port)
    {
        qDebug() << QCoreApplication::translate("QHttpServerExample",
                                                "Server failed to listen on a port.");
    }

    qDebug() << QCoreApplication::translate(
                    "QHttpServerExample",
                    "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)")
                    .arg(port);

    ChatServer server(1234);
    return app.exec();
}
