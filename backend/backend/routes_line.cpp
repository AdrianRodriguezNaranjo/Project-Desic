#include "routes_line.h"
#include "line.h"

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

void Routes_Line::setupRoutes(QHttpServer& httpServer)
{
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
            const auto number = json->value("number").toString();
            updateQuery.bindValue(":number", number.toInt());
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
}
