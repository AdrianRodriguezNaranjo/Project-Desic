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
#include "database.h"
#include "chatserver.h"
#include "routes_bus_stop.h"
#include "routes_line.h"
#include "routes_schedule.h"
#include "routes_user.h"

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

    // Create the tables in the PostgreSQL database
    if (!Database::createLineTable()) {
        return 1;
    }

    QHttpServer httpServer;

    Routes_Line::setupRoutes(httpServer);
    Routes_Schedule::setupRoutes(httpServer);
    Routes_Bus_Stop::setupRoutes(httpServer);
    Routes_User::setupRoutes(httpServer);

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
