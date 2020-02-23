#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSqlDatabase>
#include<QTableView>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
#include<QMessageBox>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include"login_dialog.h"
#include"reg_dialog.h"
#include"modify_editability_model.h"
#include "a_accessdialog.h"

class LoginDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QSqlDatabase &db,QWidget *parent = nullptr);
    ~MainWindow();

    void SetEmpID(int id);
    int GetEmpID();
    void SetName(QString name);
    void SetJobPos(QString jobpos);

    qint64 CheckWHour(QString tempBegOfWork,QString tempEndOfWork);
    void ChangeHourState(QString TimeBeg,QString TimeEnd,double hour);
    void ExitFunc();

private slots:

    void on_UpdateAims_pushButton_clicked();

    void on_StartWork_pushButton_clicked();

    void on_Auth_action_triggered();

    void on_Reg_action_triggered();

    void on_Director_action_triggered();

    void on_EndOfWork_pushButton_clicked();

    void on_Quit_action_triggered();

private:
    int EmpID=0;
    bool StartButtonClicked=false;
    bool EndButtinClicked=false;

    QSqlDatabase db;//БД
    QSqlQuery* query;//Запрос
    QSqlTableModel* sqlmodel;//отображение в табличном виде
    QString EmpName;//Имя текущего сотрудника
    QString EmpJobPos;//Рабочая позиция текущего сотрудника

    //Формы с которыми работае главное окно
    LoginDialog* authdial;//Окно авторизации
    RegDialog* regdial;//Окно регистрации
    ModifyEditabilityModel* modifmodel;//Блокировка доступа к данным для
    a_accessdialog* dirdial;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
