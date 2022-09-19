#include "data.h"

//
// Created by pepa on 9.9.22.
//

Period::Period(int month, int year) : m_year(year), m_month(month) {}

int Period::year() const {
    return m_year;
}

int Period::month() const {
    return m_month;
}

QString Period::toString() const {
    return QString("%1. %2").arg(m_month).arg(m_year);
}

Review::Review(const Period& period) : m_period(period) {}

const Period& Review::period() const {
    return m_period;
}

const QList<QList<double>>& Review::buy() const {
    return m_buy;
}

const QList<QList<double>>& Review::sell() const {
    return m_sell;
}

void Review::addSell(int day, double price) {
    addReg(day, price, true);
}

void Review::addBuy(int day, double price) {
    addReg(day, price, false);
}

void Review::addReg(int day, double price, bool bSell) {
    QList<QList<double>>* pPrice = bSell ? &m_sell : &m_buy;
    QList<double>* pReg = bSell ? &m_regSell : &m_regBuy;

    if (pReg->count() == 3) {
        double sum = price;
        for (const auto& p : *pReg) {
            sum += p;
        }

        sum = sum / 4;

        if (pPrice->count() == day) {
            (*pPrice)[day - 1].append(sum);
        } else {
            pPrice->append(QList<double>() << sum);
        }

        pReg->clear();
    } else {
        pReg->append(price);
    }
}
