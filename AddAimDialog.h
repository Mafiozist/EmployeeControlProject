#ifndef ADDAIMDIALOG_H
#define ADDAIMDIALOG_H

#include<QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>

namespace Ui {
class AddAimDialog;
}

class AddAimDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAimDialog(QSqlDatabase db,QWidget *parent = nullptr);
    ~AddAimDialog();

private slots:
    void on_buttonBox_accepted();

private:
    QSqlDatabase db;
    QSqlQuery* query;
    QString Name;
    QString JobPos;

    Ui::AddAimDialog *ui;
};

#endif // ADDAIMDIALOG_H
