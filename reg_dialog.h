#ifndef REGDIALOG_H
#define REGDIALOG_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QBuffer>

namespace Ui {
class RegDialog;
}

class RegDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegDialog(QSqlDatabase& db,QWidget *parent = nullptr);
    ~RegDialog();

    QString GetName();
    double  GetSalary();
    QVector<QString> GetAllJobPoses();
    QString GetLogin();
    QString GetPass();
    QString GetPhoto();
    int CheckLoginPassInDB(QString Enterlogin, QString EnterPass);
    bool CheckLoginInDB(QString CurrentLogin);
    int CheckOnBadSympbols(QString str,int StrMaxlength=16, int StrMinLength=4);
    bool ShowErrMessage(QString login,QString password);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_UpdatePhoto_pushButton_clicked();

private:
    QSqlQuery* query;
    QSqlDatabase db;
    QImage image;
    QByteArray qByteArray;

    Ui::RegDialog *ui;
};

#endif // REGDIALOG_H
