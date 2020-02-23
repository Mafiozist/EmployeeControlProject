#include "a_accessdialog.h"
#include "ui_a_accessdialog.h"
#include<QMessageBox>

a_accessdialog::a_accessdialog(QSqlDatabase db,QString dbname,QWidget *parent) :
    QDialog(parent),db(db),CurDbName(dbname),
    ui(new Ui::a_accessdialog)
{
    ui->setupUi(this);
    sqlmodel=new QSqlTableModel(this,db);

    for(int i=0; i<TNames.length();++i){
        //Добавляем имена таблиц в комбо бокс
        ui->TableNames_comboBox->addItem(TNames.at(i));
    }

    ui->tableView->setSortingEnabled(true);
}

a_accessdialog::~a_accessdialog()
{
    delete ui;
    delete query;
    delete adialog;
    delete jpdialog;
    delete sqlmodel;
    db.close();
}

//Используется для форматирования таблиц и добавления в вариантов сортировки в комбо бокс
void a_accessdialog::TableCorrect()
{
    ui->Sort_comboBox->clear();
    QStringList sortEmployee={"Зарплата","Админ статус","Должность","ФИО"};
    //При форматировании таблицы добавляет
    //При обновлении комбо  бокса изменяются сортировочные возможности для таблиц
    //Проверка на соот таблицу
    if(ui->TableNames_comboBox->currentText()=="employee")
    {
        sqlmodel->setHeaderData(0, Qt::Horizontal, tr("ID сотрудника"));
        sqlmodel->setHeaderData(1, Qt::Horizontal, tr("ФИО"));
        sqlmodel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
        sqlmodel->setHeaderData(3, Qt::Horizontal, tr("Зарплата"));
        sqlmodel->setHeaderData(4, Qt::Horizontal, tr("Логин"));
        sqlmodel->setHeaderData(5, Qt::Horizontal, tr("Пароль"));
        sqlmodel->setHeaderData(6, Qt::Horizontal, tr("Админ статус"));
        sqlmodel->setHeaderData(7, Qt::Horizontal, tr("Начал  в"));
        sqlmodel->setHeaderData(8, Qt::Horizontal, tr("Закончил в"));
        sqlmodel->setHeaderData(9, Qt::Horizontal, tr("Часы в этом месяце"));
        sqlmodel->setHeaderData(10, Qt::Horizontal, tr("Часы в посл месяце"));
        sqlmodel->setHeaderData(11, Qt::Horizontal, tr("Все время работы"));
        ui->tableView->hideColumn(12);//Скрываем поле фото(должно быть в двоичном коде)

        ui->Sort_comboBox->addItems(sortEmployee);
    }

    if(ui->TableNames_comboBox->currentText()=="empaims")
    {
        ui->Sort_comboBox->clear();
        sqlmodel->setHeaderData(0, Qt::Horizontal, tr("ID задания"));
        sqlmodel->setHeaderData(1, Qt::Horizontal, tr("ID сотрудника"));
        sqlmodel->setHeaderData(2, Qt::Horizontal, tr("Д/в добавления"));
        sqlmodel->setHeaderData(3, Qt::Horizontal, tr("Задание"));
        sqlmodel->setHeaderData(4, Qt::Horizontal, tr("Статус"));
    }

    if(ui->TableNames_comboBox->currentText()=="empjobpos")
    {
        ui->Sort_comboBox->clear();
        sqlmodel->setHeaderData(0, Qt::Horizontal, tr("ID должности"));
        sqlmodel->setHeaderData(1, Qt::Horizontal, tr("Должность"));
    }
}

//Устанавливаем таблицу и корректируем заголовки
void a_accessdialog::on_UpdateTable_pushButton_clicked()
{
    sqlmodel->setTable(ui->TableNames_comboBox->currentText());
    sqlmodel->select();
    ui->tableView->setModel(sqlmodel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    TableCorrect();
}

//Осуществление добавления данных в бд по таблице
void a_accessdialog::on_add_pushButton_clicked()
{
    //Проверка на соот таблицу и запуск соот диалогового окна
    if(ui->TableNames_comboBox->currentText()=="employee")
    {
        QMessageBox::information(this,"Внимание","Добавление сотрудников осуществляется через систему регистрации");
    }

    if(ui->TableNames_comboBox->currentText()=="empaims")
    {
        adialog=new AddAimDialog(db);
        adialog->exec();
    }

    if(ui->TableNames_comboBox->currentText()=="empjobpos")
    {
        jpdialog=new AddJobPosDialog(db);
        jpdialog->exec();
    }
}


void a_accessdialog::on_Sort_pushButton_clicked()
{
    if(ui->TableNames_comboBox->currentText()!="employee") QMessageBox::information(this,"Внимание","Для этой таблицы используйте сортировку встроенную в таблицу.(По клику на столбец)");
    else
    {
        if(ui->Sort_lineEdit->text()=="") QMessageBox::information(this,"Внимание","Содержание сортировки не заполнено!");
        else
        {
            if(ui->Sort_comboBox->currentText()=="Зарплата")
            {
                sqlmodel->setTable(ui->TableNames_comboBox->currentText());
                sqlmodel->setFilter("salary " + ui->Sort_lineEdit->text());
            }

            if(ui->Sort_comboBox->currentText()=="Админ статус")
            {
                sqlmodel->setTable(ui->TableNames_comboBox->currentText());
                sqlmodel->setFilter("astatus " + ui->Sort_lineEdit->text());
            }

            if(ui->Sort_comboBox->currentText()=="Зарплата")
            {
                sqlmodel->setTable(ui->TableNames_comboBox->currentText());
                sqlmodel->setFilter("salary " + ui->Sort_lineEdit->text());
            }

            //Реализовать функцию которая будет искать по частям и выводить полностью

            if(ui->Sort_comboBox->currentText()=="Должность")
            {
                sqlmodel->setTable(ui->TableNames_comboBox->currentText());
                sqlmodel->setFilter("jobpos = '" + ui->Sort_lineEdit->text().toLower() + "'");
            }

            if(ui->Sort_comboBox->currentText()=="ФИО")
            {
                sqlmodel->setTable(ui->TableNames_comboBox->currentText());
                sqlmodel->setFilter("fullname = '" + ui->Sort_lineEdit->text().toLower() + "'");
            }

            sqlmodel->select();
            if(sqlmodel->lastError().text()!=" ") QMessageBox::warning(this,"Ошибка",sqlmodel->lastError().text());

            TableCorrect();
            qDebug()<<sqlmodel->query().lastQuery();
        }
    }
}

void a_accessdialog::on_Remove_pushButton_clicked()
{
    deldial=new DeleteDialog(db,ui->TableNames_comboBox->currentText());
    deldial->exec();
}
