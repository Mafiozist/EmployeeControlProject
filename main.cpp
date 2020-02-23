#include "mainwindow.h"
#include"LoginDialog.h"
#include <QApplication>
#include<QIcon>
#include<AAccessDialog.h>
#include<QSqlQuery>
#include<QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Указываем имя для базы данных
    QSqlDatabase db;
    db=QSqlDatabase::addDatabase("QMYSQL");//Т.к. база данных в программе должна добавлятся один раз -

    //передаем ее через параметр конструктору
    db.setPort(3307);
    db.setHostName("localhost");
    db.setPassword("root");
    db.setUserName("root");
    db.setDatabaseName("empcomp");

    MainWindow w(db);
    QIcon icon(":/Target.jpg");
    w.setWindowIcon(icon);
    w.show();

    return a.exec();
}

//Если базы нет, нужно бы ее создать
/*
    else
    {
        QSqlQuery* query=new QSqlQuery(db);
        query->prepare("CREATE DATABASE empcomp;");
        query->exec();

        //Создание таблицы рабочих
        query->prepare("CREATE table employee("
                       "id int NOT NULL AUTO_INCREMENT,"
                       "fullname TEXT,"
                       "jobpos TEXT,"
                       "salary double,"
                       "login varchar(16),"
                       "pass  varchar(16),"
                       "begofwork datetime,"
                       "endofwork datetime,"
                       "PRIMARY KEY(id)"
                       ");");
        query->exec();

        //Создание таблицы должностей
        query->prepare("create table empjobpos("
                       "id int NOT NULL AUTO_INCREMENT,"
                       "JobPosName Text,"
                       "PRIMARY KEY(id)"
                       ");");
        query->exec();

        //Создание таблицы целей
        query->prepare("CREATE TABLE empaims "
                       "( aid int(50) NOT NULL AUTO_INCREMENT, "
                       "empid int(11), "
                       "adddate datetime CURRENT_TIMESTAMP(), "
                       "aim TEXT, aimstatus boolean, "
                       "PRIMARY KEY(aid), "
                       "FOREIGN KEY empid REFERENCES employee(id) );");
        query->exec();
    }
*/
