#include "AddAimDialog.h"
#include "ui_addaimdialog.h"
#include<QMessageBox>
#include<QSqlError>

AddAimDialog::AddAimDialog(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),db(db),
    ui(new Ui::AddAimDialog)
{
    ui->setupUi(this);
    query=new QSqlQuery(db);
}

AddAimDialog::~AddAimDialog()
{
    delete ui;
    delete query;
}

void AddAimDialog::on_buttonBox_accepted()
{
    int TempID=ui->ID_lineEdit->text().toInt();
    QString TempAim=ui->Aim_plainTextEdit->toPlainText();

    query->prepare("SELECT fullname,jobpos FROM employee where id = :ID;");
    query->bindValue(":ID",TempID);
    query->exec();

    while(query->next())
    {
        Name=query->value(0).toString();
        JobPos=query->value(1).toString();
    }

    if((Name =="") || (JobPos=="")) QMessageBox::warning(this,"Внимание","Сотрудник не найден, проверьте правильность ввденного ID");
    else
    {


        QMessageBox::StandardButton check=QMessageBox::information(this,"Внимание","Вы добавляете задание сотруднику "+Name+" c должностью "+JobPos+" и id= "+QString::number(TempID)+"?",QMessageBox::Yes | QMessageBox::No);

        //Если пользователь соглашается с выбором человека  которому дает задание
        if(check==QMessageBox::Yes)
        {
            query->prepare("INSERT INTO empaims(empid,aim) VALUE(:empid,:aim)");
            query->bindValue(":empid",TempID);
            query->bindValue(":aim",TempAim);

            if(query->exec()) QMessageBox::information(this,"Внимание","Задание для сотрудника "+Name+" c должностью "+JobPos+" успешно добавлено!");
            else QMessageBox::warning(this,"Внимание",query->lastError().text());

        }
    }

}
