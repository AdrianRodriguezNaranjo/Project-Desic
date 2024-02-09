#include "../include/precompiled.h"
#include "bicycle.h"

#include <QtCore/qcoreapplication.h>

#include <QxOrm_Impl.h>
#include <QtHttpServer>

#include <QCoreApplication>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QDebug>

#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QHttpServerResponder>


#define API_KEY "SecretKey"

// static bool checkApiKeyHeader(const QList<QPair<QByteArray, QByteArray>> &headers)
// {
//     for (const auto &[key, value] : headers)
//     {
//         if (key == "api_key" && value == API_KEY)
//         {
//             return true;
//         }
//     }
//     return false;
// }

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
    db.setDatabaseName("Prueba");
    db.setUserName("postgres");
    db.setPassword("alumno");

    // Open the connection to the database
    if (!db.open()) {
        qDebug() << "Error: Failed to open the database connection:" << db.lastError().text();
        return 1;
    }
    // QFile::remove("Prueba");
    // qx::QxSqlDatabase::getSingleton()->setDriverName("QPSQL");
    // qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    // qx::QxSqlDatabase::getSingleton()->setDatabaseName("Prueba");
    // qx::QxSqlDatabase::getSingleton()->setUserName("postgres");
    // qx::QxSqlDatabase::getSingleton()->setPassword("alumno");

    // qx::QxSqlDatabase::getConnectOptions();

    // Create the bicycle table in the PostgreSQL database
    QSqlQuery query;
    if (!query.exec("DROP TABLE bicycle")) {
        qDebug() << "Error: Failed to create the bicycle table:" << query.lastError().text();
        return 1;
    }
    if (!query.exec("CREATE TABLE IF NOT EXISTS bicycle ("
                    "id SERIAL PRIMARY KEY,"
                    "brand VARCHAR(255),"
                    "model VARCHAR(255)"
                    ")")) {
        qDebug() << "Error: Failed to create the bicycle table:" << query.lastError().text();
        return 1;
    }
    // Create a list of 3 bicycles

    // QSqlError daoError = qx::dao::create_table<bicycle>();
    // daoError = qx::dao::create_table<bicycle>();
    QList<QSharedPointer<bicycle>> bicycleX;

    // Create bicycle objects
    QSharedPointer<bicycle> bicycle_1(new bicycle());
    bicycle_1->m_id = 1;
    bicycle_1->m_brand = "BH";
    bicycle_1->m_model = "Star";

    QSharedPointer<bicycle> bicycle_2(new bicycle());
    bicycle_2->m_id = 2;
    bicycle_2->m_brand = "Orbea";
    bicycle_2->m_model = "Sky";

    QSharedPointer<bicycle> bicycle_3(new bicycle());
    bicycle_3->m_id = 3;
    bicycle_3->m_brand = "Decathlon";
    bicycle_3->m_model = "Super";

    // Add bicycles to the list
    bicycleX.append(bicycle_1);
    bicycleX.append(bicycle_2);
    bicycleX.append(bicycle_3);

    // Insert the list of bicycles into the PostgreSQL database
    for (const auto &bicycle : bicycleX) {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO bicycle (id, brand, model) "
                            "VALUES (:id, :brand, :model)");
        insertQuery.bindValue(":id", static_cast<int>(bicycle->m_id));
        insertQuery.bindValue(":brand", bicycle->m_brand);
        insertQuery.bindValue(":model", bicycle->m_model);

        if (!insertQuery.exec()) {
            qDebug() << "Error: Failed to insert bicycle:" << insertQuery.lastError().text();
            return 1;
        }
    }

    // Close the connection to the database

    qDebug() << "Bicycles inserted successfully.";

    QHttpServer httpServer;

    httpServer.route(
        "/", []()
        { return "Bicycles API using QxOrm & QtHttpServer"; });

    httpServer.route(
        "/v2/bicycle", QHttpServerRequest::Method::Get,
        [](const QHttpServerRequest &)
        {
            QSqlQuery query("SELECT * FROM bicycle");
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                // En caso de error, podrías devolver una respuesta de error apropiada
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            // Construye la lista de bicicletas a partir de los resultados de la consulta
            QList<bicycle> bicycleList;
            while (query.next()) {
                bicycle bicycle;
                bicycle.m_id = query.value("id").toInt();
                bicycle.m_brand = query.value("brand").toString();
                bicycle.m_model = query.value("model").toString();
                bicycleList.append(bicycle);
            }

            // Construye la respuesta JSON
            QJsonArray jsonArray;
            for (const bicycle &bike : bicycleList) {
                QJsonObject jsonObject;
                jsonObject["id"] = static_cast<qint64>(bike.m_id);
                jsonObject["brand"] = bike.m_brand;
                jsonObject["model"] = bike.m_model;
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
        "/v2/bicycle/<arg>", QHttpServerRequest::Method::Get,
        [](qint64 bicycleId, const QHttpServerRequest &)
        {
            QString sqlQuery = QString("SELECT * FROM bicycle WHERE id = %1").arg(bicycleId);
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
                jsonObject["brand"] = query.value("brand").toString();
                jsonObject["model"] = query.value("model").toString();

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

    httpServer.route("/v2/bicycle",QHttpServerRequest::Method::Options,
                     [](const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");

                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/bicycle", QHttpServerRequest::Method::Post,
        [](const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("id") || !json->contains("brand")|| !json->contains("model"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
            QSharedPointer<bicycle> new_bicycle(new bicycle());
            new_bicycle->m_id = json->value("id").toInt();
            new_bicycle->m_brand = json->value("brand").toString();
            new_bicycle->m_model = json->value("model").toString();
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO bicycle (id, brand, model) "
                                "VALUES (:id, :brand, :model)");
            insertQuery.bindValue(":id", static_cast<int>(new_bicycle->m_id));
            insertQuery.bindValue(":brand", new_bicycle->m_brand);
            insertQuery.bindValue(":model", new_bicycle->m_model);
            if (!insertQuery.exec()) {
                qDebug() << "Error: Failed to insert bicycle:" << insertQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            // QHttpServerResponse(QHttpServerResponder::StatusCode::Created);
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    httpServer.route("/v2/bicycle/<arg>",QHttpServerRequest::Method::Options,
                     [](qint64 bicycleId, const QHttpServerRequest &)
                     {
                         QHttpServerResponse response("");

                         setCorsHeaders(response);
                         return response;
                     });

    httpServer.route(
        "/v2/bicycle/<arg>", QHttpServerRequest::Method::Put,
        [](qint64 bicycleId, const QHttpServerRequest &request)
        {
            const auto json = byteArrayToJsonObject(request.body());
            if (!json || !json->contains("brand")|| !json->contains("model"))
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE bicycle SET brand = :brand, model = :model WHERE id = :id");
            updateQuery.bindValue(":id", bicycleId);
            updateQuery.bindValue(":brand", json->value("brand").toString());
            updateQuery.bindValue(":model", json->value("model").toString());

            if (!updateQuery.exec()) {
                qDebug() << "Error: Failed to update bicycle:" << updateQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }

            // QHttpServerResponse response(QHttpServerResponder::StatusCode::Ok);
            QHttpServerResponse response("");

            setCorsHeaders(response);
            return response;
        });

    httpServer.route(
        "/v2/bicycle/<arg>", QHttpServerRequest::Method::Delete,
        [](qint64 bicycleId, const QHttpServerRequest &request)
        {
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM bicycle WHERE id = :id");
            deleteQuery.bindValue(":id", bicycleId);
            if (!deleteQuery.exec()) {
                qDebug() << "Error: Failed to delete bicycle:" << deleteQuery.lastError().text();
                return QHttpServerResponse(QHttpServerResponder::StatusCode::InternalServerError);
            }
            // QHttpServerResponse response(QHttpServerResponder::StatusCode::Ok);
            QHttpServerResponse response("");
            setCorsHeaders(response);
            return response;
        });

    const auto port = httpServer.listen(QHostAddress::Any, 49080);
    if (!port)
    {
        qDebug() << QCoreApplication::translate("QHttpServerExample",
                                                "Server failed to listen on a port.");
        return 0;
    }

    qDebug() << QCoreApplication::translate(
                    "QHttpServerExample",
                    "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)")
                    .arg(port);

    return app.exec();
}
