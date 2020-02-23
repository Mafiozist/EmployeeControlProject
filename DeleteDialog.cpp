#include "deletedialog.h"
#include "ui_deletedialog.h"
#include<QMessageBox>
#include<QSqlError>
#include<QIcon>

DeleteDialog::DeleteDialog(QSqlDatabase db, QString tablename,QWidget *parent) :
    QDialog(parent),db(db),tablename(tablename),
    ui(new Ui::DeleteDialog)
{
    query=new QSqlQuery(db);
    ui->setupUi(this);

    QIcon icon(":/Res/img/delete.png");
    DeleteDialog::setWindowIcon(icon);
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
    delete query;
}

void DeleteDialog::on_buttonBox_accepted()
{
    int tempid=ui->lineEdit->text().toInt();
    if(tablename=="employee")
    {
        query->clear();
        query->prepare("DELETE FROM employee WHERE id = :id;" );
        query->bindValue(":id",tempid);
    }

    if(tablename=="empaims")
    {
        query->clear();
        query->prepare("DELETE FROM empaims WHERE aid = :id;");
        query->bindValue(":id",tempid);
    }

    if(tablename=="empjobpos")
    {
        query->clear();
        query->prepare("DELETE FROM  empjobpos WHERE id = :id;");
        query->bindValue(":id",tempid);
    }

    QMessageBox::StandardButton check=QMessageBox::question(this,"Внимание","Вы действительно хотите удалить данный id?");
    if(check==QMessageBox::Yes)
    {
        if(query->exec())QMessageBox::information(this,"Внимание","Удаление прошло успешно");
        else QMessageBox::warning(this,"Внимание",query->lastError().text());
    }
}
