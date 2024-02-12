#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

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
#include "database.h"

class HttpServer
{
public:
    static void startServer();
};

#endif // HTTP_SERVER_H
