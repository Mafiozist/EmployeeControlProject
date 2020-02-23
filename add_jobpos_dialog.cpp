#include "add_jobpos_dialog.h"
#include "ui_addjobposdialog.h"
#include<QMessageBox>
#include<QDebug>
AddJobPosDialog::AddJobPosDialog(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),db(db),
    ui(new Ui::AddJobPosDialog)
{
    ui->setupUi(this);
    query=new QSqlQuery(db);
}

AddJobPosDialog::~AddJobPosDialog()
{
    delete ui;
    delete query;
    db.close();
}

void AddJobPosDialog::on_buttonBox_accepted()
{
    if(db.open())
    {
        bool checkstatus=false;
        query->prepare("SELECT JobPosName FROM empjobpos;");
        query->exec();
        while(query->next())
        {
            QString TempJobPos=query->value(0).toString();

            if(TempJobPos.toLower()==ui->Addjobpos_lineEdit->text().toLower())
            {
                QMessageBox::information(this,"Внимание","Данная должность была добавлена ранее");
                checkstatus=true;
                break;
            }
        }

        if(checkstatus==false)
        {
            query->prepare("INSERT INTO empjobpos(JobPosName) VALUE(:JobPosName);");
            query->bindValue(":JobPosName",ui->Addjobpos_lineEdit->text());

            if(query->exec()) QMessageBox::information(this,"Внимание","Новая должность успешно добавлена!");
            else qDebug()<<"AddJobPos: Ошибка: "<<query->lastError().text();
        }

    }

    else QMessageBox::warning(this,"Внимание","База данных не работает!");


}
