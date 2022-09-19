//
// Created by pepa on 9.9.22.
//

#ifndef QELETRIKA_DATA_H
#define QELETRIKA_DATA_H

#include <QObject>
#include <QList>

class Period {
public:
    Period() = default;
    Period(int month, int year);

    ~Period() = default;

    int year() const;
    int month() const;

    QString toString() const;

private:
    int m_year;
    int m_month;
};

Q_DECLARE_METATYPE(Period)

class Review {
public:
    Review() = default;
    explicit Review(const Period& period);

    ~Review() = default;

    void addSell(int day, double price);
    void addBuy(int day, double price);

    const Period& period() const;
    const QList<QList<double>>& buy() const;
    const QList<QList<double>>& sell() const;

private:
    Period m_period;
    QList<QList<double>> m_buy;
    QList<QList<double>> m_sell;

    QList<double> m_regBuy;
    QList<double> m_regSell;

    void addReg(int day, double price, bool bSell);
};

#endif //QELETRIKA_DATA_H
