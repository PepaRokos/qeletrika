#ifndef ODBTABLEMODEL_H
#define ODBTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSharedPointer>
#include <QMetaProperty>
#include <QModelIndex>
#include <QDebug>
#include "itablemodel.h"

template<class T>
class AutoTableModel : public ITableModel
{

public:
    explicit AutoTableModel(QObject *parent = NULL)
        :ITableModel(parent)
    {
        filtered = false;
        m_checkboxSelect = false;
    }

    virtual ~AutoTableModel() {}

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)

        return m_list.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)

        QObject *entity = new T();
        int colCount = entity->metaObject()->propertyCount() - 1;
        delete entity;

        return colCount;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (index.column() == 0 && m_checkboxSelect)
        {
            if (role == Qt::CheckStateRole)
            {
                if (m_selectedRows.contains(index.row()))
                {
                    return Qt::Checked;
                }
                else
                {
                    return Qt::Unchecked;
                }
            }
        }

        QSharedPointer<T> entity = m_list.at(index.row());
        QObject *rawEntity = (QObject*)entity.data();

        QVariant dispData = rawEntity->property(rawEntity->metaObject()->property(index.column() + 1).name());

        if (role == Qt::DisplayRole)
        {
            return dispData;
        }
        if (role == Qt::TextAlignmentRole)
        {
            if (dispData.type() == QVariant::Date
                    || dispData.type() == QVariant::Time
                    || dispData.type() == QVariant::DateTime
                    || dispData.type() == QVariant::Int
                    || dispData.type() == QVariant::Double)
            {
                return Qt::AlignRight;
            }

            return Qt::AlignLeft;
        }
        if (role == Qt::EditRole)
        {
            return dispData;
        }

        return QVariant::Invalid;
    }

    QList<QSharedPointer<T> > list()
    {
        return m_list;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (role != Qt::DisplayRole) {
            return QVariant();
        }

        if (orientation == Qt::Horizontal) {
            QObject *entity = (QObject*)new T();

            for (int i = 0; i < entity->metaObject()->propertyCount(); i++) {
                if (i == section) {
                    QString colName(entity->metaObject()->property(i + 1).name());
                    delete entity;

                    if (m_translations.contains(colName))
                    {
                        return m_translations[colName];
                    }
                    return colName;
                }
            }

            delete entity;
        }

        return QVariant(section + 1);
    }

    virtual void sort(int column, Qt::SortOrder order) override {
        if (m_list.isEmpty()) {
            return;
        }

        beginResetModel();

        QObject *rawEntity = (QObject*)m_list.at(0).data();
        const char *prop = rawEntity->metaObject()->property(column + 1).name();

        std::sort(m_list.begin(), m_list.end(), [prop, order](QSharedPointer<T> entA, QSharedPointer<T> entB) -> bool {
            if (order == Qt::DescendingOrder) {
                return ((QObject*)entA.data())->property(prop) < ((QObject*)entB.data())->property(prop);
            } else {
                return ((QObject*)entB.data())->property(prop) < ((QObject*)entA.data())->property(prop);
            }
        });

        endResetModel();
    }

    /*///////////////////////*/

    QSharedPointer<T> itemFromIndex(const QModelIndex &index) const
    {
        return m_list.at(index.row());
    }

    void setItemToIndex(const QModelIndex &index, QSharedPointer<T> data)
    {
        m_list.removeAt(index.row());
        m_list.insert(index.row(), data);

        emit dataChanged(index, index);
    }

    void addRow(QSharedPointer<T> data)
    {
        int index = rowCount() == 0 ? 0 : rowCount() - 1;
        beginInsertRows(QModelIndex(), index, index);
        insertRow(rowCount());
        m_list.append(data);
        endInsertRows();
    }

    void removeRowAt(const QModelIndex &index)
    {
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        removeRow(index.row());
        m_list.removeAt(index.row());
        endRemoveRows();
    }

    void setData(QList<QSharedPointer<T> > list)
    {
        beginResetModel();
        m_list = list;
        endResetModel();
    }

    void setTranslations(const QMap<QString, QString> &translations)
    {
        m_translations = translations;
    }

    QList<int> selectedRows() const
    {
        return m_selectedRows;
    }

    QList<QSharedPointer<T> > selectedItems()
    {
        QList<QSharedPointer<T> > ret;
        foreach (int row, m_selectedRows) {
            ret.append(m_list[row]);
        }

        return ret;
    }

    void setSelectAll(bool select) {
        if (!m_checkboxSelect) {
            return;
        }

        beginResetModel();
        m_selectedRows.clear();

        if (select)
        {
            for (int i = 0; i < rowCount(); i++)
            {
                m_selectedRows << i;
            }
        }
        endResetModel();
    }

protected:
    void handleFilter(const QString &filter) override
    {
        /*beginResetModel();
        if (!filtered)
        {
            m_fullList = m_list;
            filtered = true;
        }

        ExprEvaluator ev;
        auto it = std::copy_if(ALL(m_fullList), m_list.begin(), [&filter, &ev](QSharedPointer<T> obj){ return ev.evaluate((QObject*)obj.data(), filter); });
        m_list.erase(it, m_list.end());

        endResetModel();*/
    }

    void handleRestore() override
    {
        if (filtered)
        {
            beginResetModel();
            m_list = m_fullList;
            endResetModel();
            filtered = false;
        }
    }

private:
    QList<QSharedPointer<T> > m_list;
    QList<QSharedPointer<T> > m_fullList;
    QList<int> m_selectedRows;
    QMap<QString, QString> m_translations;
    bool filtered;

    // QAbstractItemModel interface
public:
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override
    {
        if (role == Qt::EditRole)
        {
            QSharedPointer<T> entity = m_list.at(index.row());
            QObject *rawEntity = (QObject*)entity.data();

            QVariant val = value;
            rawEntity->setProperty(rawEntity->metaObject()->property(index.column() + 1).name(), val);
        }

        if (role == Qt::CheckStateRole)
        {
            if (m_selectedRows.contains(index.row()))
            {
                m_selectedRows.removeOne(index.row());
            }
            else
            {
                m_selectedRows.append(index.row());
            }
        }

        emit editCompleted();
        return true;
    }
};

#endif // ODBTABLEMODEL_H
