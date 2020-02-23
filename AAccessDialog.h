#ifndef AACCESSDIALOG_H
#define AACCESSDIALOG_H

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
#include<QMessageBox>
#include<QSqlTableModel>

#include"AddAimDialog.h"
#include"AddJobPosDialog.h"
#include"deletedialog.h"

namespace Ui {
class AAccessDialog;
}

class AAccessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AAccessDialog(QSqlDatabase db,QString dbname="empcomp", QWidget *parent = nullptr);
    ~AAccessDialog();

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

    Ui::AAccessDialog *ui;
};

#endif // A_ACCESSDIALOG_H
