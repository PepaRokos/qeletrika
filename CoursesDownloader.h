//
// Created by pepa on 9.9.22.
//

#ifndef QELETRIKA_COURSESDOWNLOADER_H
#define QELETRIKA_COURSESDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include "DataProvider.h"

class CoursesDownloader : public QObject{
Q_OBJECT

public:
    CoursesDownloader(QObject* parent = nullptr);
    ~CoursesDownloader() = default;

    void downloadMonth(const Period& period);

signals:
    void downloadComplete(const Period& period, const QList<double>& courses);

private:
    QNetworkAccessManager m_manager;
    QList<double> m_courses;
    static const QString BASE_URL;
    int m_days{0};
    int m_daysDone{0};
    Period m_period;

private slots:
    void reqFinished(QNetworkReply* reply);
};


#endif //QELETRIKA_COURSESDOWNLOADER_H
