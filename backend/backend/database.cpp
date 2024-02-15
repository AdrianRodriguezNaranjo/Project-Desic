#include "database.h"

bool Database::createLineTable()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    if (!query.exec("DROP TABLE IF EXISTS user_lines")) {
        qDebug() << "Error: Failed to drop the user_lines table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("DROP TABLE IF EXISTS bus_stop")) {
        qDebug() << "Error: Failed to drop the bus_stop table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("DROP TABLE IF EXISTS schedule")) {
        qDebug() << "Error: Failed to drop the schedule table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("DROP TABLE IF EXISTS line")) {
        qDebug() << "Error: Failed to drop the line table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS line ("
                    "id SERIAL PRIMARY KEY,"
                    "number int,"
                    "firstbusstop VARCHAR(255),"
                    "lastbusstop VARCHAR(255)"
                    ")")) {
        qDebug() << "Error: Failed to create the line table:" << query.lastError().text();
        return false;
    }    

    if (!query.exec("CREATE TABLE IF NOT EXISTS schedule ("
                    "id SERIAL PRIMARY KEY,"
                    "time TIME,"
                    "line_id INT,"
                    "FOREIGN KEY (line_id) REFERENCES line(id) ON DELETE CASCADE"
                    ")")) {
        qDebug() << "Error: Failed to create the schedule table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS bus_stop ("
                    "id SERIAL PRIMARY KEY,"
                    "line_id INT,"
                    "location VARCHAR(255),"
                    "imagenFilePath VARCHAR(255),"
                    "FOREIGN KEY (line_id) REFERENCES line(id) ON DELETE CASCADE"
                    ")")) {
        qDebug() << "Error: Failed to create the bus_stop table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("DROP TABLE IF EXISTS bus_user")) {
        qDebug() << "Error: Failed to drop the user table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS bus_user ("
                    "id SERIAL PRIMARY KEY,"
                    "username VARCHAR(255),"
                    "password VARCHAR(255),"
                    "name VARCHAR(255),"
                    "discriminator VARCHAR(255)"
                    ")")) {
        qDebug() << "Error: Failed to create the bus_user table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS user_lines ("
                    "user_id INT,"
                    "line_id INT,"
                    "PRIMARY KEY (user_id, line_id),"
                    "FOREIGN KEY (user_id) REFERENCES bus_user(id) ON DELETE CASCADE,"
                    "FOREIGN KEY (line_id) REFERENCES line(id) ON DELETE CASCADE"
                    ")")) {
        qDebug() << "Error: Failed to create the user_lines table:" << query.lastError().text();
        return false;
    }

    return true;
}
