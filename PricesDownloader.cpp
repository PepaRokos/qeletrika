//
// Created by pepa on 3.9.22.
//

#include <QNetworkReply>
#include <QCalendar>
#include <gq/Document.h>
#include <gq/Node.h>
#include "PricesDownloader.h"
#include "DataProvider.h"

const QString PricesDownloader::BASE_URL = "https://www.ote-cr.cz/cs/kratkodobe-trhy/elektrina/denni-trh?date=%1-%2-%3";

PricesDownloader::PricesDownloader(QObject* parent) : QObject(parent) {
    connect(&m_netManager, &QNetworkAccessManager::finished, this, &PricesDownloader::reqFinished);
}

void PricesDownloader::downloadMonth(const Period& period) {
    m_listData.clear();
    m_daysDone = 0;
    m_period = period;
    QCalendar calendar;
    m_days = calendar.daysInMonth(period.month(), period.year());

    for (int i = 0; i < m_days; i++) {
        m_listData.append(QList<double>());
    }

    for (int i = 0; i < m_days; i++) {
        auto dayUrl = BASE_URL.arg(period.year())
                .arg(period.month(), 2, 10, QLatin1Char('0'))
                .arg(i + 1, 2, 10, QLatin1Char('0'));
        m_netManager.get(QNetworkRequest(QUrl(dayUrl)));
    }
}

void PricesDownloader::reqFinished(QNetworkReply* reply) {
    QString strReply(reply->readAll());
    reply->deleteLater();

    if (!strReply.isEmpty()) {
        parseReply(strReply);
    }

    ++m_daysDone;
    if (m_days == m_daysDone) {
        emit downloadComplete(m_period, m_listData);
    }
}

void PricesDownloader::parseReply(const QString& reply) {
    CDocument document;
    document.parse(reply.toStdString());

    auto selTitle = document.find("h3.chart_title");
    if (selTitle.nodeNum() == 0) {
        return;
    }

    QString strTitle = selTitle.nodeAt(0).text().c_str();
    QStringRef refDay(&strTitle, strTitle.indexOf("-") + 1, 3);
    auto curDay = refDay.toInt() - 1;

    auto selTable = document.find("table.report_table");
    if (selTable.nodeNum() < 2) {
        return;
    }

    auto selTr = selTable.nodeAt(1).find("tr");

    for (int i = 0; i < selTr.nodeNum(); i++) {
        auto selPrice = selTr.nodeAt(i).find("td");
        if (selPrice.nodeNum() == 0) {
            continue;
        }

        auto price = QString(selPrice.nodeAt(0).text().c_str())
                .replace(",", ".")
                .replace(" ", "")
                .toDouble();

        if (price != 0) {
            m_listData[curDay].append(price);
        }
    }
}
