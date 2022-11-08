//
// Created by pepa on 9.9.22.
//

#include <QCalendar>
#include "CoursesDownloader.h"

const QString CoursesDownloader::BASE_URL = "https://www.cnb.cz/cs/financni-trhy/devizovy-trh/kurzy-devizoveho-trhu/kurzy-devizoveho-trhu/denni_kurz.txt?date=%1.%2.%3";

CoursesDownloader::CoursesDownloader(QObject* parent) : QObject(parent) {
    connect(&m_manager, &QNetworkAccessManager::finished, this, &CoursesDownloader::reqFinished);
}

void CoursesDownloader::reqFinished(QNetworkReply* reply) {
    QString strReply(reply->readAll());
    reply->deleteLater();

    int day = 0;
    for (const auto& line : strReply.split("\n")) {
        auto dateSplit = line.split(".");

        if (dateSplit.count() > 0 && dateSplit[0].toInt() > 0) {
            day = dateSplit[0].toInt();
            ++m_daysDone;
        }

        auto curSplit = line.split("|");

        if (curSplit.count() > 0 && curSplit[0] == "EMU") {
            m_courses[day - 1] = curSplit[4].replace(",", ".").toDouble();
            break;
        }
    }

    if (m_days == m_daysDone) {
        int listStartZeros = -1;

        for (int i = 0; i < m_courses.count(); i++) {
            if (m_courses[i] == 0 && i > 0) {
                m_courses[i] = m_courses[i - 1];
            }

            if (m_courses[i] == 0) {
                ++listStartZeros;
            } else if (listStartZeros > -1) {
                for (int j = 0; j <= listStartZeros; j++) {
                    m_courses[j] = m_courses[i];
                }
            }
        }

        for (int i = m_courses.count() - 1; i < 1; i--) {

        }

        emit downloadComplete(m_period, m_courses);
    }
}

void CoursesDownloader::downloadMonth(const Period& period) {
    m_courses.clear();
    m_daysDone = 0;
    m_period = period;
    QCalendar calendar;
    m_days = calendar.daysInMonth(period.month(), period.year());

    for (int i = 0; i < m_days; i++) {
        m_courses.append(0);
    }

    for (int i = 0; i < m_days; i++) {
        auto url = BASE_URL.arg(i + 1, 2, 10, QLatin1Char('0'))
                .arg(period.month(), 2, 10, QLatin1Char('0'))
                .arg(period.year());
        m_manager.get(QNetworkRequest(QUrl(url)));
    }
}
