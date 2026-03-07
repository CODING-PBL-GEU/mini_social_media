#include "User.h"

User::User(){}

User::User(int id,QString username,QString password){
    this->id=id;
    this->username=username;
    this->password=password;
}

int User::getId()const{
    return id;
}

QString User::getUsername() const
{
    return username;
}

void User::setId(int id)
{
    this->id=id;
}

void User::setUsername(QString username)
{
    this->username=username;
}

void User::setPassword(QString password)
{
    this->password=password;
}
