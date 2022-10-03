//
// Created by pepa on 2.9.22.
//

#ifndef QELETRIKA_MAINWINDOW_H
#define QELETRIKA_MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "PricesDownloader.h"
#include "CoursesDownloader.h"
#include "DataProvider.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_review_triggered();
    void on_actionDownload_prices_triggered();
    void on_actionBuyingSettings_triggered();
    void on_checkDays_toggled(bool check);
    void on_coursesComplete(const Period& period, const QList<double>& courses);
    void on_pricesComplete(const Period& period, const QList<QList<double>>& prices);

private:
    Ui::MainWindow* ui;
    PricesDownloader m_pricesDownloader;
    CoursesDownloader m_coursesDownloader;
    DataProvider m_provider;
    QSqlTableModel* m_coursesModel;
    QSqlTableModel* m_pricesModel;
    QSqlTableModel* m_reviewModel;
    bool m_dlCoursesProg{false};
    bool m_dlPricesProg{false};

    void fillPeriods();
    void fillCourses();
    void fillPrices();
    void fillReview();
    void enableActions();
    void disableActions();
};


#endif //QELETRIKA_MAINWINDOW_H
