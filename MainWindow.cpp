#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoginDialog.h"    //Диалоговое окно регистрации и логина
#include<QSqlTableModel>         //Табличное представление запроса в бд
#include<QMessageBox>            //Нотификация через виджет
#include<QDateTime>              //дата и время
#include<math.h>

#define dbErr "Ошибка базы данных"


//Чтобы кто-то случайно не закрыл рабочую зону нужно спросить пароль!


MainWindow::MainWindow(QSqlDatabase& db,QWidget *parent)
    : QMainWindow(parent),db(db),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    query=new QSqlQuery(db);
    sqlmodel=new QSqlTableModel(this,db);

    ui->Photo_label->setMinimumSize(175,175);
    ui->Photo_label->setMaximumSize(175,175);
    ui->Aims_tableView->setSortingEnabled(true);

    MainWindow::setWindowTitle("СУПЗ");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete query;
    delete sqlmodel;
    delete  authdial;
    delete modifmodel;
    delete regdial;
    delete dirdial;
    db.close();
}

void MainWindow::SetEmpID(int id)
{
    EmpID=id;
}

int MainWindow::GetEmpID()
{
    return EmpID;
}

void MainWindow::SetName(QString name)
{
    ui->Name_Label->setText(name);
}

void MainWindow::SetJobPos(QString jobpos)
{
    ui->JobPos_label->setText(jobpos);
}

void MainWindow::ExitFunc()
{
    //Обнулить все переменные и отчистить таблицу
    StartButtonClicked=false;
    EndButtinClicked=false;
    EmpID=0;
    ui->Name_Label->clear();
    ui->JobPos_label->clear();
    ui->Aims_tableView->clearSpans();
    sqlmodel->clear();
    ui->Photo_label->setPixmap(QPixmap(":/default_icon.png"));
}

//Возвращает разницу во времени в секундах
qint64 MainWindow::CheckWHour(QString TimeBeg,QString TimeEnd)
{
    //TimeBeg,TimeEnd между датой и временем содержится символ T, по этому нобходимо его удалить
    //Для дальнейшей работы с типом данных datetime
    TimeBeg.remove(10,1);//Удаляю символ T
    TimeBeg.insert(10," ");//Добавляю " "
    TimeEnd.remove(10,1);//Удаляю символ T
    TimeEnd.insert(10," ");//Добавляю " "
    TimeBeg.remove(19,4);//Удаляю милисекунды
    TimeEnd.remove(19,4);

    //Занношу время взятое из базы в beg и end  по шаблону бд
    QDateTime beg=QDateTime::fromString(TimeBeg,"yyyy-MM-dd hh:mm:ss");
    QDateTime end=QDateTime::fromString(TimeEnd,"yyyy-MM-dd hh:mm:ss");

    return  beg.secsTo(end);
}

//Метод изменяющий значения проработанного времени в зависимости от дня
void MainWindow::ChangeHourState(QString TimeBeg,QString TimeEnd,double hour)
{
    TimeBeg.remove(10,1);//Удаляю символ T
    TimeBeg.insert(10," ");//Добавляю " "
    TimeEnd.remove(10,1);//Удаляю символ T
    TimeEnd.insert(10," ");//Добавляю " "
    TimeBeg.remove(19,4);//Удаляю милисекунды
    TimeEnd.remove(19,4);

    //Заношу время взятое из базы в beg и end  по шаблону бд
    QDateTime beg=QDateTime::fromString(TimeBeg,"yyyy-MM-dd hh:mm:ss");
    QDateTime end=QDateTime::fromString(TimeEnd,"yyyy-MM-dd hh:mm:ss");

    if(end.date().daysInMonth() == end.date().day()) //Нужно убрать возможность накручив
    {
        query->prepare("UPDATE employee SET hworked = hworked + :newhour WHERE id = :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->bindValue(":newhour",hour);
        query->exec();

        query->prepare("UPDATE employee SET last_month_hour_work = hworked  WHERE id = :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->exec();

        query->prepare("UPDATE employee SET hworked =0 WHERE id = :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->bindValue(":newhour",hour);
        query->exec();

        query->prepare("UPDATE employee SET allhworked = allhworked + last_month_hour_work  WHERE id = :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->exec();
    }

    else
    {
        query->prepare("UPDATE employee SET hworked = hworked + :newhour WHERE id = :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->bindValue(":newhour",hour);
        query->exec();
    }

}

void MainWindow::on_UpdateAims_pushButton_clicked()
{
    if(EmpID==0) QMessageBox::information(this,"Внимание","Вы не авторизованы!");
    else if(StartButtonClicked==false) ui->statusbar->showMessage(tr("Вы не начали рабочий день!"),1200);
    else if(EndButtinClicked==true) QMessageBox::information(this,"Внимание","Вы уже закончили рабочий день!");
    else
    {
        if(db.open())
        {

            //Перевод Emp id в строку для дальнейшей проверки
            QString TempEmpID=QString::number(EmpID);
            qDebug()<<"mainwindow.cpp: id принятый в работу: "<<TempEmpID;

            //По таблицу выбираем колонки которые нам нужны (дальше по айди нужно выбирать какие задания конкретно выводить)
            sqlmodel->setTable("empaims");

            sqlmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);//Данные заносятся в базу при изминении
            sqlmodel->setFilter("empid = " + TempEmpID+ " and aimstatus = 0");
            sqlmodel->select();

            sqlmodel->setHeaderData(2, Qt::Horizontal, tr("Дата/время добавления"));
            sqlmodel->setHeaderData(3, Qt::Horizontal, tr("Текст задания"));
            sqlmodel->setHeaderData(4, Qt::Horizontal, tr("Статус выполнения"));

            //Создаем прокси модель, которая будет блокировать доступ к всем колонкам таблицы, кроме посл
            modifmodel=new ModifyEditabilityModel(ui->Aims_tableView);
            modifmodel->setSourceModel(sqlmodel);

            //Заносить данные в базу, при их изменении
            sqlmodel->setEditStrategy(QSqlTableModel::OnFieldChange);

            if(sqlmodel->lastError().text()!=" ")
            {
                QMessageBox::critical(this,"Error",sqlmodel->lastError().text());
            }

            //Устанавливаю таблицу на отображение в tableview
            ui->Aims_tableView->setModel(modifmodel);

            //Расстягиваю заголовки до изменяемого значения
            ui->Aims_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->Aims_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            //Не показываю первые 2 столбца с ID
             ui->Aims_tableView->hideColumn(0);
             ui->Aims_tableView->hideColumn(1);


            // Запрет выделения столбца и выделение всей строки
            ui->Aims_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->Aims_tableView->show();
            //Конец оформления данных полученных из бд отвечающих за задачи сотрудников
        }
    }
}

void MainWindow::on_StartWork_pushButton_clicked()
{
    if(EmpID==0) QMessageBox::information(this,"Внимание","Вы не авторизованы!");
    else if(EndButtinClicked==true) QMessageBox::information(this,"Внимание","Вы уже закончили рабочий день!");
    else
    {
        if(StartButtonClicked==false)
        {
            if(db.open())
            {
                //Заношу в базу окончания время работы
                query->prepare("UPDATE employee SET begofwork = CURRENT_TIMESTAMP() WHERE id = :EmpID;");
                query->bindValue(":EmpID",EmpID);
                query->exec();

                if(query->exec()) qDebug()<<"StartWork: Запрос в базу выполнен!";
                else qDebug()<<endl<<query->lastError().text();

                StartButtonClicked=true;
            }
        }
        else QMessageBox::information(this,tr("Внимание"),tr("Вы уже начали свой рабочий день!"));
    }
}

void MainWindow::on_EndOfWork_pushButton_clicked() //Как учитывать время когда оно в 00?
{
    //Если EmpID=0 значит пользователь не авторизован
    if(EmpID==0) QMessageBox::information(this,"Внимание","Вы не авторизованы!");

    //Если кнопка начала не нажата, то зачем нажимать кнопку закончить работу?
    else if(StartButtonClicked==false) QMessageBox::information(this,"Внимание","Вы не начали рабочий день!");
    else if(EndButtinClicked==true) QMessageBox::information(this,"Внимание","Вы уже закончили рабочий день!");
    else
    {
        //Устанавливаю время уже после работы
        query->prepare("UPDATE employee SET endofwork = CURRENT_TIMESTAMP() WHERE id = :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->exec();
        query->clear();

        //Получаю время из бд
        query->prepare("SELECT begofwork,endofwork FROM employee WHERE id = :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->exec();

        QString tempBegOfWork;
        QString tempEndOfWork;

        while(query->next())
        {
            tempBegOfWork=query->value(0).toString();
            tempEndOfWork=query->value(1).toString();
        }

        //Округление + функция возвращающая разницу во времени
        double hour=CheckWHour(tempBegOfWork,tempEndOfWork)/3600.00;
        QString strhour=QString::number(hour,'f',3);
        double newHour=strhour.toDouble();
        ChangeHourState(tempBegOfWork,tempEndOfWork,newHour);

        EndButtinClicked=true;
        sqlmodel->clear();
    }
}

void MainWindow::on_Auth_action_triggered()
{
    ExitFunc();//Защита от переавторизации
    LoginDialog* authdial=new LoginDialog(db);

    if(authdial->exec())
    {
        if(authdial->getLoginStatus()==true)
        {
            EmpID=authdial->GetEmpID();

            if(db.open())
            {
                QByteArray imgfromDB;
                QImage image;

                //Начало работы с профилем работника (Фотография, ФИО, Рабочая позиция)
                query->prepare("SELECT fullname,jobpos,photo FROM employee where id = :Empid;");
                query->bindValue(":Empid",EmpID);//Заменяет :Empid на конкретное значение

                if(query->exec()) qDebug()<<"Auth: Запрос в базу выполнен!";
                else qDebug()<<endl<<query->lastError().text();

                while(query->next())
                {
                    EmpName=query->value(0).toString();
                    EmpJobPos=query->value(1).toString();
                    imgfromDB=query->value(2).toByteArray();
                }

                //Проверка на пустату в колонке фото из БД
                if(!imgfromDB.isEmpty())
                {
                    image.loadFromData(imgfromDB);
                    image.scaled(175,175,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                    ui->Photo_label->setPixmap(QPixmap::fromImage(image).scaled(175,175,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                }

                qDebug()<<"mainwindow.cpp: Нашел по id=" + QString::number(EmpID) +" Имя: "<<EmpName<<" Рабочую позицию: "<<EmpJobPos;

                ui->Name_Label->setText(EmpName);
                ui->JobPos_label->setText(EmpJobPos);
                QMessageBox::information(this,tr("Авторизация"),tr("Успешно авторизованы!"));

                //В случае авторизации нового пользователя, необходимо обнулить данные о нажатиях кнопок старого
                StartButtonClicked=false;
                EndButtinClicked=false;
            }
            else QMessageBox::warning(this,tr(dbErr),tr("База данных не подключена!"));

        }
    }

}

void MainWindow::on_Reg_action_triggered()
{
    if(db.open())
    {
        regdial=new RegDialog(db);
        regdial->exec();
    }
    else QMessageBox::warning(this,"Внимание","База данных не подключена!");
}

void MainWindow::on_Director_action_triggered()
{
    //Директорский доступ
    if(EmpID==0) QMessageBox::information(this,"Внимание","Вы не авторизованы!");
    else if(StartButtonClicked==false) QMessageBox::information(this,"Внимание","Вы не начали рабочий день!");
    else if(EndButtinClicked==true) QMessageBox::information(this,"Внимание","Вы уже закончили рабочий день!");
    else
    {
        //Проверяем наличие статуса админа
        query->prepare("SELECT astatus FROM employee WHERE id= :EmpID;");
        query->bindValue(":EmpID",EmpID);
        query->exec();
        query->next();
        int astatus=query->value(0).toInt();

        if(astatus>0)
        {
               dirdial=new AAccessDialog(db);
                dirdial->exec();
        }
        else QMessageBox::information(this,"Внимание","У вас нет доступа к этой функции, обратитесь к администрации");
    }
}

void MainWindow::on_Quit_action_triggered()
{
    if(EmpID==0) QMessageBox::information(this,"Внимание","Вы не авторизованы!");
    else
    {
        ExitFunc();
        QMessageBox::information(this,"Авторизация","Вы вышли из учетной записи!");
    }
}
