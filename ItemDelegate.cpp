//
// Created by pepa on 19.9.22.
//

#include "ItemDelegate.h"

ItemDelegate::ItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

ItemDelegate::ItemDelegate(int precision, QObject* parent) : QStyledItemDelegate(parent), m_precision{precision} {}

QString ItemDelegate::displayText(const QVariant& value, const QLocale& locale) const {
    switch(value.type()) {
        case QMetaType::Double:
            return locale.toString(value.toDouble(), 'f', m_precision);
        case QMetaType::Float:
            return locale.toString(value.toFloat(), 'f', m_precision);
        default:
            return QStyledItemDelegate::displayText(value, locale);
    }
}

NumModel::NumModel(QObject* parent, const QSqlDatabase& db) : QSqlTableModel(parent, db) {}

QVariant NumModel::data(const QModelIndex& idx, int role) const {
    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignRight;
    }

    return QSqlTableModel::data(idx, role);
}
