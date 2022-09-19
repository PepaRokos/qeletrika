//
// Created by pepa on 6.9.22.
//

#ifndef QELETRIKA_DATAPROVIDER_H
#define QELETRIKA_DATAPROVIDER_H

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include "data.h"


class DataProvider {
public:
    DataProvider();
    ~DataProvider() = default;

    void setCoursesModel(QSqlQueryModel* coursesModel);
    void setPricesModel(QSqlQueryModel* pricesModel);
    void setReviewModel(QSqlQueryModel* reviewModel);

    void coursesFor(const Period& period);
    void saveCourses(const Period& period, const QList<double>& courses);

    void pricesFor(const Period& period);
    void savePrices(const Period& period, const QList<QList<double>>& prices);

    void reviewFor(const Period& period, bool dayView);
    void saveReview(const Review& review);
    QString totalBought(const Period& period);
    QString totalEarn(const Period& period);

    QList<Period> periodsForCombo();

    int periodId(int month, int year, int day);
    int newPeriod(int month, int year, int day);

    bool openDb();

private:
    QSqlQueryModel* m_pricesModel;
    QSqlQueryModel* m_coursesModel;
    QSqlQueryModel* m_reviewModel;
    QSqlDatabase m_database;
};


#endif //QELETRIKA_DATAPROVIDER_H
