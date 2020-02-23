#ifndef MODIFYEDITABILITYMODEL_H
#define MODIFYEDITABILITYMODEL_H

#include <QIdentityProxyModel>
#include<QModelIndex>


//Класс для блокировки взаимодействия с колонками таблицы задания сотрудника(кроме колонки с статусом выполнения(тобишь кол-во колонок -1))
//Колонка статуса должна быть 4-ой
class ModifyEditabilityModel: public QIdentityProxyModel {
    Q_OBJECT

public:
    ModifyEditabilityModel (QObject * parent = 0) : QIdentityProxyModel(parent)
    {}
    Qt::ItemFlags flags (const QModelIndex & index) const {
        if (index.column() == 4) {
            return QIdentityProxyModel::flags(index);
        } else {
            return QIdentityProxyModel::flags(index) & ~Qt::ItemIsEditable;
        }
    }
};

#endif // MODIFYEDITABILITYMODEL_H
