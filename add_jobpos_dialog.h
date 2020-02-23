#ifndef ADDJOBPOSDIALOG_H
#define ADDJOBPOSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class AddJobPosDialog;
}

class AddJobPosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddJobPosDialog(QSqlDatabase db,QWidget *parent = nullptr);
    ~AddJobPosDialog();

private slots:
    void on_buttonBox_accepted();

private:
    QSqlDatabase db;
    QSqlQuery* query;

    Ui::AddJobPosDialog *ui;
};

#endif // ADDJOBPOSDIALOG_H
