#include"DatabaseManager.h"

DatabaseManager::DatabaseManager()
{}

bool DatabaseManager::connectDatabase()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("socialnetwork.db");
    if(!db.open())
    {
        qDebug()<<"DATA BASE CONNECTION FAIL :"<<db.lastError();
        return false;
    }
    qDebug()<<"Database connected ";
    return true;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXITS users("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE,"
               "password TEXT,"
               "email TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS post ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "user_id INTEGER, "
               "content TEXT ,"
               "timesstamp DATETIME DEFAULT CURRENT_TIMESTAMP)");

    query.exec("CREATE TABLE IF NOT EXISTS friends("
               "user_id INTEGER ,"
               "friend_id INTEGER) ");

    return true;
}
