#ifndef A_ACCESSDIALOG_H
#define A_ACCESSDIALOG_H

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
#include<QMessageBox>
#include<QSqlTableModel>

#include"add_aim_dialog.h"
#include"add_jobpos_dialog.h"
#include"deletedialog.h"

namespace Ui {
class a_accessdialog;
}

class a_accessdialog : public QDialog
{
    Q_OBJECT

public:
    explicit a_accessdialog(QSqlDatabase db,QString dbname="empcomp", QWidget *parent = nullptr);
    ~a_accessdialog();

    void TableCorrect();
    void SortByPart();

private slots:

    void on_UpdateTable_pushButton_clicked();

    void on_add_pushButton_clicked();

    void on_Sort_pushButton_clicked();

    void on_Remove_pushButton_clicked();

private:
    QSqlQuery* query;//Запрос
    QSqlDatabase db;//БД
    QSqlTableModel* sqlmodel;//Табличное представление бд
    QStringList TNames=db.tables(); //Все таблицы из текущей бд
    QStringList SearchCriteria;//Массив критериев поиска
    QString CurDbName;//Название базы данных

    //Диалоговые окна
    AddAimDialog* adialog;
    AddJobPosDialog* jpdialog;
    DeleteDialog* deldial;

    Ui::a_accessdialog *ui;
};

#endif // A_ACCESSDIALOG_H
