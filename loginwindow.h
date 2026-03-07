#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include<QWidget>
#include"ui_LoginWindow.h"
QT_BEGIN_NAMESPACE
namespace Ui{
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow :public QWidget
{
    Q_OBJECT
public:
    LoginWindow(QWidget *parent=nullptr);

    ~LoginWindow();
private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
