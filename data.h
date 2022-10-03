//
// Created by pepa on 9.9.22.
//

#ifndef QELETRIKA_DATA_H
#define QELETRIKA_DATA_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

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

class LowTariffTime : public QObject {
    Q_OBJECT
    Q_PROPERTY(int from READ from WRITE setFrom)
    Q_PROPERTY(int to READ to WRITE setTo)
public:
    explicit LowTariffTime(QObject* parent = nullptr) : QObject(parent) {}
    LowTariffTime(int from, int to) : QObject(nullptr), m_from(from), m_to(to) {}
    ~LowTariffTime() = default;

    int from() const;
    int to() const;
    void setFrom(int from);
    void setTo(int to);

    bool isLow(int hour);

private:
    int m_from{0};
    int m_to{0};
};

using LowTariffTimePtr = QSharedPointer<LowTariffTime>;

class Settings {
public:
    Settings() = default;

    Settings(double monthFee, double distributionFeeLow, double distributionFeeHigh, bool fixPrice, double lowPrice,
             double highPrice, const QList<LowTariffTimePtr>& lowTariff,
             const QList<LowTariffTimePtr> lowTariffHoliday);

    double monthFee() const;

    double distributionFeeLow() const;

    double distributionFeeHigh() const;

    bool fixPrice() const;

    double lowPrice() const;

    double highPrice() const;

    const QList<LowTariffTimePtr>& lowTariff() const;

    const QList<LowTariffTimePtr>& lowTariffHoliday() const;

    double priceForHour(int hour, bool holiday);

private:
    double m_monthFee{0};
    double m_distributionFeeLow{0};
    double m_distributionFeeHigh{0};
    bool m_fixPrice{false};
    double m_lowPrice{0};
    double m_highPrice{0};
    QList<LowTariffTimePtr> m_lowTariff;
    QList<LowTariffTimePtr> m_lowTariffHoliday;
};

#endif //QELETRIKA_DATA_H
