#ifndef ITABLEMODEL_H
#define ITABLEMODEL_H

#include <QString>
#include <QAbstractTableModel>
#include <QList>

class ITableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ITableModel(QObject *parent = NULL);

protected:
    virtual void handleFilter(const QString &filter) = 0;
    virtual void handleRestore() = 0;
    bool m_checkboxSelect;

public slots:
    void filter(const QString &filter);
    void restore();

signals:
    void editCompleted();

    // QAbstractItemModel interface
public:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setEditableCols(const QList<int> cols);

    bool checkboxSelect() const;
    void setCheckboxSelect(bool checkboxSelect);

private:
    QList<int> m_editableCols;
};

#endif // ITABLEMODEL_H
