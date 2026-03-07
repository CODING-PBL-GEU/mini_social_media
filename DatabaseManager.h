#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
#include<QString>

class DatabaseManager{
private:
    QSqlDatabase db;
public:
    DatabaseManager();
    bool connectDatabase();
    bool createTables();
};
#endif // DATABASEMANAGER_H
