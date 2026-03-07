#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWindow)
{
     ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QString username=ui->usernameField->text();
    QString password=ui->passwordField->text();

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username=? AND password=?");
    query.addBindValue(username);
    query.addBindValue(password);
    if(query.exec() && query.next())
    {
        QMessageBox ::information(this,"Login","Login Successfully");

    }
    else
    {
        QMessageBox ::warning(this,"Login","Invalid username or password");
    }
}

void LoginWindow::on_registerButton_clicked()
{
    QMessageBox::information(this,"register","Registration window will open here");
}
