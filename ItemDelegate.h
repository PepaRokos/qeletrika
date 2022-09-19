//
// Created by pepa on 19.9.22.
//

#ifndef QELETRIKA_ITEMDELEGATE_H
#define QELETRIKA_ITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QSqlTableModel>
#include <QSqlDatabase>

class ItemDelegate : public QStyledItemDelegate {
public:
    explicit ItemDelegate(QObject* parent = nullptr);
    ItemDelegate(int precision, QObject* parent = nullptr);
    ~ItemDelegate() override = default;

    QString displayText(const QVariant& value, const QLocale& locale) const override;

private:
    int m_precision{2};
};

class NumModel : public QSqlTableModel {
public:
    NumModel(QObject* parent = nullptr, const QSqlDatabase& db = QSqlDatabase());

    QVariant data(const QModelIndex& idx, int role) const override;
};

#endif //QELETRIKA_ITEMDELEGATE_H
