//
// Created by pepa on 6.9.22.
//

#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QDir>
#include "DataProvider.h"

DataProvider::DataProvider() {
    m_database = QSqlDatabase::addDatabase("QSQLITE");
}

bool DataProvider::openDb() {
    QString strAppPath = QDir::homePath() + "/.qeletrika";
    QDir appHome(strAppPath);

    if (!appHome.exists()) {
        QDir::home().mkdir(".qeletrika");
    }

    m_database.setDatabaseName(strAppPath + "/data.db");

    if (!m_database.open()) {
        return false;
    }

    if (m_database.tables().size() > 0) {
        return true;
    }

    QFile fileSql(":/sql/db.sql");

    if (!fileSql.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QSqlQuery q;
    QString strSql(fileSql.readAll());
    for (const auto& tableSql : strSql.split(";", Qt::SkipEmptyParts)) {
        if (tableSql != "\n" && !q.exec(tableSql)) {
            auto lastError = q.lastError();
            auto errText = lastError.text();
            return false;
        }
    }

    return true;
}

void DataProvider::coursesFor(const Period& period) {
    QString sql = "select p.day, c.course from courses c join period p on p.id = c.period where p.year = %1 and p.month = %2";
    m_coursesModel->setQuery(sql.arg(period.year()).arg(period.month()));
}

void DataProvider::saveCourses(const Period& period, const QList<double>& courses) {
    QSqlQuery queryDel;
    queryDel.prepare("delete from courses where period in (select id from period where month = :m and year = :y)");
    queryDel.bindValue(":m", period.month());
    queryDel.bindValue(":y", period.year());
    queryDel.exec();

    for (int i = 0; i < courses.count(); i++) {
        auto idPeriod = periodId(period.month(), period.year(), i + 1);

        if (idPeriod <= 0) {
            idPeriod = newPeriod(period.month(), period.year(), i + 1);
        }

        QSqlQuery query;
        query.prepare("insert into courses(period, course) values(:p, :c)");
        query.bindValue(":p", idPeriod);
        query.bindValue(":c", courses[i]);
        query.exec();
    }
}

int DataProvider::periodId(int month, int year, int day) {
    QSqlQuery query;
    query.prepare("select id from period where year = :y and month = :m and day = :d");
    query.bindValue(":y", year);
    query.bindValue(":m", month);
    query.bindValue(":d", day);

    if (!query.exec()) {
        return -1;
    }

    if (query.size() == 0) {
        return 0;
    }

    query.first();

    return query.record().field("id").value().toInt();
}

int DataProvider::newPeriod(int month, int year, int day) {
    QSqlQuery query;
    query.prepare("insert into period(year, month, day) values(:y, :m, :d)");
    query.bindValue(":y", year);
    query.bindValue(":m", month);
    query.bindValue(":d", day);

    if (query.exec()) {
        QSqlQuery qId("select max(id) as mid from period");
        qId.exec();
        qId.first();

        return qId.record().value("mid").toInt();
    }

    return 0;
}

void DataProvider::setCoursesModel(QSqlQueryModel* coursesModel) {
    m_coursesModel = coursesModel;
}

QList<Period> DataProvider::periodsForCombo() {
    QSqlQuery query("select distinct year, month from period order by year desc, month desc");
    QList<Period> ret;

    if (!query.exec()) {
        auto err = query.lastError();
        auto errText = err.text();
        return ret;
    }

    if (query.first()) {
        do {
            Period period(query.value("month").toInt(), query.value("year").toInt());
            ret.append(period);
        } while (query.next());
    }

    return ret;
}

void DataProvider::savePrices(const Period& period, const QList<QList<double>>& prices) {
    QSqlQuery queryDel;
    queryDel.prepare("delete from prices where period in (select id from period where month = :m and year = :y)");
    queryDel.bindValue(":m", period.month());
    queryDel.bindValue(":y", period.year());
    queryDel.exec();

    for (int i = 0; i < prices.count(); i++) {
        auto idPeriod = periodId(period.month(), period.year(), i + 1);

        if (idPeriod <= 0) {
            idPeriod = newPeriod(period.month(), period.year(), i + 1);
        }

        for (int j = 0; j < prices[i].count(); j++) {
            QSqlQuery query;
            query.prepare("insert into prices(period, hour, price) values(:p, :h, :pr)");
            query.bindValue(":p", idPeriod);
            query.bindValue(":h", j + 1);
            query.bindValue(":pr", prices[i][j]);
            query.exec();
        }
    }
}

void DataProvider::setPricesModel(QSqlQueryModel* pricesModel) {
    m_pricesModel = pricesModel;
}

void DataProvider::pricesFor(const Period& period) {
    QString sql = R"(select p.day, pr.hour, pr.price, (pr.price * c.course) as price_home from prices pr
        join period p on p.id = pr.period
        join courses c on p.id = c.period
                                           where p.year = %1 and p.month = %2)";
    m_pricesModel->setQuery(sql.arg(period.year()).arg(period.month()));
}

void DataProvider::saveReview(const Review& review) {
    QSqlQuery queryDel;
    queryDel.prepare("delete from consumptions where period in (select id from period where month = :m and year = :y)");
    queryDel.bindValue(":m", review.period().month());
    queryDel.bindValue(":y", review.period().year());
    queryDel.exec();

    if (review.buy().size() != review.sell().size()) {
        return;
    }

    for (int i = 0; i < review.buy().size(); i++) {
        auto idPeriod = periodId(review.period().month(), review.period().year(), i + 1);

        if (idPeriod <= 0) {
            idPeriod = newPeriod(review.period().month(), review.period().year(), i + 1);
        }

        if (review.buy()[i].size() != review.sell()[i].size()) {
            continue;
        }

        for (int j = 0; j < review.buy()[i].size(); j++) {
            QSqlQuery query;
            query.prepare("insert into consumptions(period, hour, buy, sell) values(:p, :h, :b, :s)");
            query.bindValue(":p", idPeriod);
            query.bindValue(":h", j + 1);
            query.bindValue(":b", review.buy()[i][j]);
            query.bindValue(":s", review.sell()[i][j]);
            query.exec();
        }
    }
}

void DataProvider::setReviewModel(QSqlQueryModel* reviewModel) {
    m_reviewModel = reviewModel;
}

void DataProvider::reviewFor(const Period& period, bool dayView) {
    QSqlQuery query;

    if (dayView) {
        query.prepare("select day, buy, sell, spent, earn from day_view where year = :y and month = :m");
    } else {
        query.prepare("select day, hour, buy, sell, spent, earn from hour_view where year = :y and month = :m");
    }

    query.bindValue(":y", period.year());
    query.bindValue(":m", period.month());
    query.exec();
    m_reviewModel->setQuery(query);
}

QString DataProvider::totalBought(const Period& period) {
    QSqlQuery query;
    query.prepare("select sum(buy) as bought, sum(spent) as spent from day_view where year = :y and month = :m");
    query.bindValue(":y", period.year());
    query.bindValue(":m", period.month());
    query.exec();
    query.first();

    return QString("%1/%2").arg(query.value("bought").toDouble(), 0, 'f', 3)
        .arg(query.value("spent").toDouble(), 0, 'f', 2);
}

QString DataProvider::totalEarn(const Period& period) {
    QSqlQuery query;
    query.prepare("select sum(sell) as sold, sum(earn) as earn from day_view where year = :y and month = :m");
    query.bindValue(":y", period.year());
    query.bindValue(":m", period.month());
    query.exec();
    query.first();

    return QString("%1/%2").arg(query.value("sold").toDouble(), 0, 'f', 3)
        .arg(query.value("earn").toDouble(), 0, 'f', 2);
}

