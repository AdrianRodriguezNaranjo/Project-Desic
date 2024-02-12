#include "http_server.h"
#include "database.h"
#include "line.h"

void setCorsHeaders(QHttpServerResponse &response)
{
    response.setHeader("Access-Control-Allow-Origin", "http://localhost:3000");
    response.setHeader("Access-Control-Allow-Methods", "GET,HEAD,OPTIONS,POST,PUT,DELETE");
    response.setHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Authorization");
}

void HttpServer::startServer()
{
QHttpServer httpServer;

httpServer.route(
    "/", []()
    { return "Bicycles API using QxOrm & QtHttpServer"; });

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

// httpServer.route(
//     "/v2/bicycle/<arg>", QHttpServerRequest::Method::Get,
//     [](qint64 bicycleId, const QHttpServerRequest &)
//     {
//         QString sqlQuery = QString("SELECT * FROM bicycle WHERE id = %1").arg(bicycleId);
//         QSqlQuery query(sqlQuery);
//         QSqlError error = query.lastError();
//         if (error.type() != QSqlError::NoError) {
//             qDebug() << "Error executing query:" << query.lastError().text();
//             // En caso de error, podrías devolver una respuesta de error apropiada
//             return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
//         }

//         if (query.next()) {
//             // Construir el objeto JSON con los datos de la bicicleta
//             QJsonObject jsonObject;
//             jsonObject["id"] = static_cast<qint64>(query.value("id").toInt());
//             jsonObject["brand"] = query.value("brand").toString();
//             jsonObject["model"] = query.value("model").toString();

//             // Construir la respuesta JSON
//             QJsonDocument jsonDocument(jsonObject);
//             QByteArray responseData = jsonDocument.toJson();

//             // Construir la respuesta HTTP
//             QHttpServerResponse response(responseData);
//             setCorsHeaders(response);
//             return response;
//         } else {
//             // Si no hay resultados, devolver una respuesta indicando que no se encontraron datos
//             return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
//         }
//     });

// httpServer.route("/v2/bicycle",QHttpServerRequest::Method::Options,
//                  [](const QHttpServerRequest &)
//                  {
//                      QHttpServerResponse response("");

//                      setCorsHeaders(response);
//                      return response;
//                  });

// httpServer.route(
//     "/v2/bicycle", QHttpServerRequest::Method::Post,
//     [](const QHttpServerRequest &request)
//     {
//         const auto json = byteArrayToJsonObject(request.body());
//         if (!json || !json->contains("id") || !json->contains("brand")|| !json->contains("model"))
//             return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
//         QSharedPointer<bicycle> new_bicycle(new bicycle());
//         new_bicycle->m_id = json->value("id").toInt();
//         new_bicycle->m_brand = json->value("brand").toString();
//         new_bicycle->m_model = json->value("model").toString();
//         QSqlQuery insertQuery;
//         insertQuery.prepare("INSERT INTO bicycle (id, brand, model) "
//                             "VALUES (:id, :brand, :model)");
//         insertQuery.bindValue(":id", static_cast<int>(new_bicycle->m_id));
//         insertQuery.bindValue(":brand", new_bicycle->m_brand);
//         insertQuery.bindValue(":model", new_bicycle->m_model);
//         if (!insertQuery.exec()) {
//             qDebug() << "Error: Failed to insert bicycle:" << insertQuery.lastError().text();
//             return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
//         }
//         // QHttpServerResponse(QHttpServerResponder::StatusCode::Created);
//         QHttpServerResponse response("");
//         setCorsHeaders(response);
//         return response;
//     });

// httpServer.route("/v2/bicycle/<arg>",QHttpServerRequest::Method::Options,
//                  [](qint64 bicycleId, const QHttpServerRequest &)
//                  {
//                      QHttpServerResponse response("");

//                      setCorsHeaders(response);
//                      return response;
//                  });

// httpServer.route(
//     "/v2/bicycle/<arg>", QHttpServerRequest::Method::Put,
//     [](qint64 bicycleId, const QHttpServerRequest &request)
//     {
//         const auto json = byteArrayToJsonObject(request.body());
//         if (!json || !json->contains("brand")|| !json->contains("model"))
//             return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

//         QSqlQuery updateQuery;
//         updateQuery.prepare("UPDATE bicycle SET brand = :brand, model = :model WHERE id = :id");
//         updateQuery.bindValue(":id", bicycleId);
//         updateQuery.bindValue(":brand", json->value("brand").toString());
//         updateQuery.bindValue(":model", json->value("model").toString());

//         if (!updateQuery.exec()) {
//             qDebug() << "Error: Failed to update bicycle:" << updateQuery.lastError().text();
//             return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
//         }

//         // QHttpServerResponse response(QHttpServerResponder::StatusCode::Ok);
//         QHttpServerResponse response("");

//         setCorsHeaders(response);
//         return response;
//     });

// httpServer.route(
//     "/v2/bicycle/<arg>", QHttpServerRequest::Method::Delete,
//     [](qint64 bicycleId, const QHttpServerRequest &request)
//     {
//         QSqlQuery deleteQuery;
//         deleteQuery.prepare("DELETE FROM bicycle WHERE id = :id");
//         deleteQuery.bindValue(":id", bicycleId);
//         if (!deleteQuery.exec()) {
//             qDebug() << "Error: Failed to delete bicycle:" << deleteQuery.lastError().text();
//             return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
//         }
//         // QHttpServerResponse response(QHttpServerResponder::StatusCode::Ok);
//         QHttpServerResponse response("");
//         setCorsHeaders(response);
//         return response;
//     });

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
}
