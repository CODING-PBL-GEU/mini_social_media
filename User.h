#ifndef USER_H
#define USER_H
#include<QString>

class User{
private:
    int id;
    QString username;
    QString password;

public:
    User();
    User(int id,QString username,QString password);
    int getId() const;
    QString getUsername() const;
    void setId(int d);
    void setUsername(QString Username);
    void setPassword(QString password);
};

#endif // USER_H
