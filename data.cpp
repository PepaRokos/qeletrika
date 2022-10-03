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

int LowTariffTime::from() const {
    return m_from;
}

int LowTariffTime::to() const {
    return m_to;
}

bool LowTariffTime::isLow(int hour) {
    return hour >= m_from && hour <= m_to;
}

void LowTariffTime::setFrom(int from) {
    m_from = from;
}

void LowTariffTime::setTo(int to) {
    m_to = to;
}

Settings::Settings(double monthFee, double distributionFeeLow, double distributionFeeHigh, bool fixPrice,
                   double lowPrice,
                   double highPrice, const QList<LowTariffTimePtr>& lowTariff,
                   const QList<LowTariffTimePtr> lowTariffHoliday) : m_monthFee(monthFee), m_distributionFeeLow(distributionFeeLow),
                                                               m_distributionFeeHigh(distributionFeeHigh), m_fixPrice(fixPrice), m_lowPrice(lowPrice),
                                                               m_highPrice(highPrice), m_lowTariff(lowTariff),
                                                               m_lowTariffHoliday(lowTariffHoliday) {}

double Settings::monthFee() const {
    return m_monthFee;
}

bool Settings::fixPrice() const {
    return m_fixPrice;
}

double Settings::lowPrice() const {
    return m_lowPrice;
}

double Settings::highPrice() const {
    return m_highPrice;
}

const QList<LowTariffTimePtr>& Settings::lowTariff() const {
    return m_lowTariff;
}

double Settings::priceForHour(int hour, bool holiday) {
    QList<LowTariffTimePtr> lowTariff = holiday ? m_lowTariffHoliday : m_lowTariff;

    for (const auto& time : lowTariff) {
        if (time->from() <= hour && time->to() >= hour) {
            return m_lowPrice;
        }
    }

    return m_highPrice;
}

double Settings::distributionFeeLow() const {
    return m_distributionFeeLow;
}

double Settings::distributionFeeHigh() const {
    return m_distributionFeeHigh;
}

const QList<LowTariffTimePtr>& Settings::lowTariffHoliday() const {
    return m_lowTariffHoliday;
}
