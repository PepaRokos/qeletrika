#include "itablemodel.h"

ITableModel::ITableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_checkboxSelect = false;
}

bool ITableModel::checkboxSelect() const
{
    return m_checkboxSelect;
}

void ITableModel::setCheckboxSelect(bool checkboxSelect)
{
    m_checkboxSelect = checkboxSelect;
}

void ITableModel::filter(const QString &filter)
{
    handleFilter(filter);
}

void ITableModel::restore()
{
    handleRestore();
}

Qt::ItemFlags ITableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0 && m_checkboxSelect)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    if (m_editableCols.contains(index.column()))
    {
        return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsDragEnabled;
}

void ITableModel::setEditableCols(const QList<int> cols)
{
    m_editableCols = cols;
}

