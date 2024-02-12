#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

class Database
{
public:
    static bool createLineTable();
};

#endif // DATABASE_H
