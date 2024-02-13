#include "database.h"

bool Database::createLineTable()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    if (!query.exec("DROP TABLE IF EXISTS schedule")) {
        qDebug() << "Error: Failed to drop the schedule table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("DROP TABLE IF EXISTS line")) {
        qDebug() << "Error: Failed to drop the line table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("DROP TABLE IF EXISTS bus_stop")) {
        qDebug() << "Error: Failed to drop the bus_stop table:" << query.lastError().text();
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
                    "location VARCHAR(255) NOT NULL,"
                    "line_id INT,"
                    "imagenFilePath VARCHAR(255),"
                    "FOREIGN KEY (line_id) REFERENCES line(id) ON DELETE CASCADE"
                    ")")) {
        qDebug() << "Error: Failed to create the schedule table:" << query.lastError().text();
        return false;
    }


    return true;
}
