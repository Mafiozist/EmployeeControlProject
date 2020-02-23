#ifndef REGORLOGINDIALOG_H
#define REGORLOGINDIALOG_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
#include<QMessageBox>
#include<MainWindow.h>

class MainWindow;

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QSqlDatabase& db,QWidget *parent = nullptr);
    ~LoginDialog();
//Методы взаимодействия с бд
    bool CheckLoginInDB(QString Enterlogin);
    int CheckLoginPassInDB(QString Enterlogin,QString EnterPass);
    int  GetEmpID() const;
    void SetEmpID(int id);
    bool getLoginStatus() const;
    int  CheckOnBadSympbols(QString str,int StrMaxlength=16, int StrMinLength=4);
    bool ShowErrMessage(QString EnterLogin, QString EnterPass);

public slots:

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    int EmpID;
    bool LoginStatus=false;

    QSqlDatabase db;
    QSqlQuery* query;

    Ui::LoginDialog *ui;
};

#endif // REGORLOGINDIALOG_H
