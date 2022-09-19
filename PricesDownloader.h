//
// Created by pepa on 3.9.22.
//

#ifndef QELETRIKA_PRICESDOWNLOADER_H
#define QELETRIKA_PRICESDOWNLOADER_H

#include <QObject>
#include <QList>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "data.h"
#include "DataProvider.h"

class PricesDownloader : public QObject {
Q_OBJECT

public:
    explicit PricesDownloader(QObject* parent = nullptr);
    ~PricesDownloader() override = default;

    void downloadMonth(const Period& period);

signals:
    void downloadComplete(const Period& period, const QList<QList<double>>& list);

private:
    QList<QList<double>> m_listData;
    QNetworkAccessManager m_netManager;
    int m_days{0};
    int m_daysDone{0};
    QString m_sReply;
    Period m_period;
    void parseReply(const QString& reply);

    static const QString BASE_URL;

private slots:
    void reqFinished(QNetworkReply* reply);
};


#endif //QELETRIKA_PRICESDOWNLOADER_H
