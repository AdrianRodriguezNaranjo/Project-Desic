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
#include "database.h"
#include "chatserver.h"
// #include "http_server.h"

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
    db.setPassword("admin");

    // Open the connection to the database
    if (!db.open()) {
        qDebug() << "Error: Failed to open the database connection:" << db.lastError().text();
        return 1;
    }

    // Create the tables in the PostgreSQL database
    if (!Database::createLineTable()) {
        return 1;
    }

    // Iniciar el servidor HTTP
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
        "/v2/schedule", QHttpServerRequest::Method::Get,
        [](const QHttpServerRequest &)
        {
            QSqlQuery query("SELECT * FROM schedule");
            QSqlError error = query.lastError();
            if (error.type() != QSqlError::NoError) {
                qDebug() << "Error executing query:" << query.lastError().text();
                // En caso de error, podrías devolver una respuesta de error apropiada
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
                jsonObject["time"] = schedule.time.toString("hh:mm");
                jsonArray.append(jsonObject);
            }
            QJsonDocument jsonDocument(jsonArray);
            QByteArray responseData = jsonDocument.toJson();

            // Construye la respuesta HTTP
            QHttpServerResponse response(responseData);
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
