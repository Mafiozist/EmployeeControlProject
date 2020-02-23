#include "LoginDialog.h"
#include "ui_logindialog.h"
#include<QDebug>
#include"MainWindow.h"
#include<QIcon>
#include<QMessageBox>

#define ErrAuth "Ошибка авторизации"

LoginDialog::LoginDialog(QSqlDatabase &db,QWidget *parent) :
    QDialog(parent),db(db),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QIcon icon(":/login icon.png");
    LoginDialog::setWindowIcon(icon);

    query=new QSqlQuery(db);
}

LoginDialog::~LoginDialog()
{
    delete ui;
    delete query;
    db.close();
}

bool LoginDialog::CheckLoginInDB(QString CurrentLogin)
{
    query->prepare("SELECT login FROM employee ");
    query->exec();

    int count=0;
    while(query->next())
    {
        if(CurrentLogin==query->value(0).toString())
        {
            count++;

        }

    }
    qDebug()<<"Count in CheckLoginInDB: "<<count;

    if(count==0) return false;
    else return true;

}
//Проверка на существование объекта, где логин и пароль соответсвуют Enterlogin и EnterPass
int LoginDialog::CheckLoginPassInDB(QString Enterlogin, QString EnterPass)
{
    query->prepare("SELECT id,login,pass FROM employee;");
    query->exec();

    int count=0;
    int TempID=0;
    while(query->next())
    {
        if((Enterlogin==query->value(1).toString()) && EnterPass==query->value(2).toString())
        {
            count++;
            TempID=query->value(0).toInt();
        }

    }
    qDebug()<<"Count(кол-во совпадений) in CheckLoginPassInDB: "<<count<<"Id который выходит из функции CheckLoginPassInDB: "<<TempID;

    if(count==0) return false;
    else return TempID;
}

bool LoginDialog::ShowErrMessage(QString EnterLogin, QString EnterPass)
{
    if(CheckOnBadSympbols(EnterLogin)!=0 || CheckOnBadSympbols(EnterPass)!=0)//Если есть ошибки
    {

        if(CheckOnBadSympbols(EnterLogin)==1 || CheckOnBadSympbols(EnterPass)==1)
        {
            QMessageBox::warning(this,tr(ErrAuth),tr("В логине или пароле обнаружен пробельный символ!"));
        }

        if(CheckOnBadSympbols(EnterLogin)==2 || CheckOnBadSympbols(EnterPass)==2)
        {
            QMessageBox::warning(this,tr(ErrAuth),tr("Логин или пароль не может быть меньше 4 и более 16 символов!"));

        }
        return true;
    }
    else return false;
}

int LoginDialog::GetEmpID() const
{
    return EmpID;
}

//Функция проверяющая строку на ошибки
int LoginDialog::CheckOnBadSympbols(QString str,int StrMaxlength, int StrMinLength)//StrMaxlength- Колличество выделяемой памяти под строку(возможно внутри бд)
{
    for(int i=0;i<str.length();i++)
    {
        if((str.at(i)==" "))
        {
            return 1;//Ошибка, обнаружен пробел в строке
        }
    }

    if(str.length()>StrMaxlength ||str.length() < StrMinLength) return 2;//Ошибка, строка слишком короткая или длинная

    return 0;
}

void LoginDialog::on_buttonBox_accepted()
{
    if(db.open())
    {
        qDebug()<<"База данных работает!";

        //Записываем введенные данные с формы
        QString EnterLogin=ui->Login_lineEdit->text();
        QString EnterPass=ui->Pass_lineEdit->text();

        //Проверяем на наличие ошибок, которые проверяются в функции, если есть выводим сообщение
        if(ShowErrMessage(EnterLogin,EnterPass));

        //Если в логине или пароле нет ошибок, которые мы отлавливаем
        else
        {
            int TempEmpID=0;

            //Если существует логин & комбинация логин и пароль в бд
            if((CheckLoginInDB(EnterLogin)==true) && (CheckLoginPassInDB(EnterLogin,EnterPass)))
            {

                //Если есть совпадения, то вызываю функцию снова и беру id
                TempEmpID=CheckLoginPassInDB(EnterLogin,EnterPass);
                EmpID=TempEmpID;
                qDebug()<<"После проверки"<<TempEmpID;
                LoginStatus=true;
            }

            //Если комбинация логин пароль не найдена
            else if(CheckLoginPassInDB(EnterLogin,EnterPass)==0)
            {
                QMessageBox::information(this,ErrAuth,"Проверьте правильность введенных данных");
            }

            else
            {
                qDebug()<<"3-й вариант проверки в regorlogin.cpp";
            }
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Ошибка"),tr("База данных не подключена!"));
    }
}

void LoginDialog::on_buttonBox_rejected()
{
//Ничего не делать
}

bool LoginDialog::getLoginStatus() const
{
    return LoginStatus; 
}
