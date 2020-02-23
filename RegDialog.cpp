#include "RegDialog.h"
#include "ui_regdialog.h"
#include"LoginDialog.h"
#include <QFileDialog>
#include<QDebug>
#include<QBuffer>

#define ErrAuth "Ошибка авторизации"

RegDialog::RegDialog(QSqlDatabase& db,QWidget *parent) :
    QDialog(parent),db(db),
    ui(new Ui::RegDialog)
{
    //Конструктор формы
    query=new QSqlQuery(db);
    ui->setupUi(this);

    ui->Photo_label->setMaximumSize(175,175);
    ui->Photo_label->setMinimumSize(175,175);

    if(db.open())//Для записи всех возможных вариантов должностей
    {
        query->prepare("SELECT JobPosName FROM empjobpos" );
        if(query->exec())
        {
            while(query->next())
            {
                ui->JobPos_comboBox->addItem(query->value(0).toString());
            }
        }
        else qDebug()<<"regdial.cpp: Ошибка в запросе добавления в AllJobPoses: "<<query->lastError().text();
    }
    else QMessageBox::warning(this,tr("Внимание"),tr("База данных не подключена!"));
}

RegDialog::~RegDialog()
{
    delete ui;
    delete query;
}

QString RegDialog::GetName()
{
    return ui->Name_lineEdit->text();
}

double RegDialog::GetSalary()
{
    return ui->Salary_lineEdit->text().toDouble();
}

QString RegDialog::GetLogin()
{
    return  ui->Login_lineEdit->text();
}

QString RegDialog::GetPass()
{
    return ui->Password_lineEdit->text();
}
//Нужно реализовать доступ к фото
QString RegDialog::GetPhoto()
{
    return QString::number(0);
}

//Проверка на существование объекта, где логин и пароль соответсвуют Enterlogin и EnterPass
int RegDialog::CheckLoginPassInDB(QString Enterlogin, QString EnterPass)
{
    query->prepare("SELECT id,login,pass FROM employee");
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
    qDebug()<<"regdialog.cpp: Count(кол-во совпадений): "<<count<<"Id который выходит из функции CheckLoginPassInDB: "<<TempID;

    if(count==0) return false;
    else return TempID;
}

bool RegDialog::CheckLoginInDB(QString CurrentLogin)
{
    query->prepare("SELECT login from empcomp.employee ");
    query->exec();

    int count=0;
    while(query->next())
    {
        if(CurrentLogin==query->value(0).toString())
        {
            count++;
        }

    }
    qDebug()<<"regdialog.cpp: Count in CheckLoginInDB: "<<count;
    if(count==0) return false;
    else return true;
}

//Функция проверяющая строку на ошибки
int RegDialog::CheckOnBadSympbols(QString str,int StrMaxlength, int StrMinLength)//StrMaxlength- Колличество выделяемой памяти под строку(возможно внутри бд)
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

bool RegDialog::ShowErrMessage(QString EnterLogin, QString EnterPass)
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
        if(CheckLoginInDB(EnterLogin))
        {
            QMessageBox::warning(this,tr(ErrAuth),tr("Пользователь уже зарегестрирован!"));
        }
        return true;
    }
    else return false;
}

void RegDialog::on_buttonBox_accepted()
{

    QString EnterName=GetName();
    double  EnterSalary=GetSalary();
    QString EnterJobPos=ui->JobPos_comboBox->currentText();
    QString EnterLogin=GetLogin();
    QString EnterPass=GetPass();

    if(CheckLoginInDB(EnterLogin)==true) QMessageBox::warning(this,"Внимание","Данный пользователь зарегистрирован, используйте другой логин!");
    else
    {
        //Если есть ошибки при вводе выводим их
        if(ShowErrMessage(EnterLogin,EnterPass));
        else
        {
            query->prepare("INSERT INTO employee(fullname,salary,jobpos,login,pass,photo)"
                           "VALUE (:name,:salary,:jobpos,:login,:pass,:photo)");

            query->bindValue(":name",  EnterName);
            query->bindValue(":salary",EnterSalary);
            query->bindValue(":jobpos",EnterJobPos);
            query->bindValue(":login", EnterLogin);
            query->bindValue(":pass",  EnterPass);
            query->bindValue(":photo", qByteArray);

            if(query->exec()) QMessageBox::information(this,"Регистрация","Регистрация прошла успешно!");
            else QMessageBox::warning(this,"Регистрация","Вы не зарегистрированы!");
        }
    }
}

void RegDialog::on_buttonBox_rejected()
{
//Ничего не делать
}

//Реализация фотографий
void RegDialog::on_UpdatePhoto_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"Открыть файл","C://","Images(*.jpg *.png *.jpeg *.bmp *.gif)");
    if(QString::compare(filename,QString())!=0)
    {
        bool loadimage=image.load(filename);
        if(loadimage)
        {
            image=image.scaledToWidth(ui->Photo_label->width(),Qt::SmoothTransformation);
            ui->Photo_label->setPixmap(QPixmap::fromImage(image));

            QBuffer qBuffer(&qByteArray,this);

            if(image.sizeInBytes()!=0)
            {
                  qBuffer.open(QIODevice::WriteOnly);
                  image.save(&qBuffer,"PNG");
                  qBuffer.close();
            }
        }
    }
}
