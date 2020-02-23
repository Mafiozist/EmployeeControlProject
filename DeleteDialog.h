#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>

namespace Ui {
class DeleteDialog;
}

class DeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteDialog(QSqlDatabase db,QString tablename,QWidget *parent = nullptr);
    ~DeleteDialog();

private slots:
    void on_buttonBox_accepted();

private:
    QSqlDatabase db;
    QString tablename;
    QSqlQuery* query;

    Ui::DeleteDialog *ui;
};

#endif // DELETEDIALOG_H
