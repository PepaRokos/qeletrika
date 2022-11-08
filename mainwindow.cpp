//
// Created by pepa on 2.9.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PricesDownloader.h"
#include "ReviewLoader.h"
#include "ItemDelegate.h"
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_provider.openDb();
    connect(&m_coursesDownloader, &CoursesDownloader::downloadComplete, this, &MainWindow::on_coursesComplete);
    connect(&m_pricesDownloader, &PricesDownloader::downloadComplete, this, &MainWindow::on_pricesComplete);
    connect(ui->comboPeriod, qOverload<int>(&QComboBox::currentIndexChanged), [this](int) {
        fillCourses();
        fillPrices();
        fillReview();
    });

    m_coursesModel = new NumModel(this);
    ui->tableCourses->setModel(m_coursesModel);
    m_provider.setCoursesModel(m_coursesModel);
    ui->tableCourses->setItemDelegate(new ItemDelegate(this));

    m_pricesModel = new NumModel(this);
    ui->tablePrices->setModel(m_pricesModel);
    m_provider.setPricesModel(m_pricesModel);
    ui->tablePrices->setItemDelegate(new ItemDelegate(this));

    m_reviewModel = new NumModel(this);
    ui->tableReview->setModel(m_reviewModel);
    m_provider.setReviewModel(m_reviewModel);
    ui->tableReview->setItemDelegate(new ItemDelegate(3, this));

    fillPeriods();
    fillReview();
    fillCourses();
    fillPrices();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionExit_triggered() {
    qApp->exit();
}

void MainWindow::on_actionOpen_review_triggered() {
    auto fileName = QFileDialog::getOpenFileName(this,"Open EG.D review", "", "Excel XLSX Files (*.xlsx)");

    if (fileName.isEmpty()) {
        return;
    }

    ReviewLoader loader;
    auto status = loader.loadFile(fileName);

    switch (status) {
        case ReviewLoader::OK:
            disableActions();
            m_coursesDownloader.downloadMonth(loader.review().period());
            m_pricesDownloader.downloadMonth(loader.review().period());
            m_provider.saveReview(loader.review());
            fillReview();
            break;
        case ReviewLoader::NOT_XLSX:
            QMessageBox::warning(this, "Report not loaded", "File is not Excel XLSX file.");
            break;
        case ReviewLoader::NOT_REPORT:
            QMessageBox::warning(this, "Report not loaded", "Excel XLSX file does not look like EG.D review file.");
            break;
    }
}

void MainWindow::on_actionDownload_prices_triggered() {
    if (ui->comboPeriod->currentIndex() == -1) {
        disableActions();
        m_coursesDownloader.downloadMonth({QDate::currentDate().month(), QDate::currentDate().year()});
        m_pricesDownloader.downloadMonth({QDate::currentDate().month(), QDate::currentDate().year()});
    }
    else {
        if (QMessageBox::question(this, "Update", "Update prices in selected period?") == QMessageBox::Yes) {
            disableActions();
            m_coursesDownloader.downloadMonth(ui->comboPeriod->currentData().value<Period>());
            m_pricesDownloader.downloadMonth(ui->comboPeriod->currentData().value<Period>());
        }
    }
}

void MainWindow::on_coursesComplete(const Period& period, const QList<double>& courses) {
    m_provider.saveCourses(period, courses);
    m_provider.coursesFor(period);
    fillPeriods();
    fillReview();
    m_dlCoursesProg = false;
    enableActions();
}

void MainWindow::fillPeriods() {
    ui->comboPeriod->clear();
    for (const auto& period : m_provider.periodsForCombo()) {
        ui->comboPeriod->addItem(period.toString(), QVariant::fromValue(period));
    }
}

void MainWindow::fillCourses() {
    Period period = ui->comboPeriod->currentData().value<Period>();
    m_provider.coursesFor(period);
}

void MainWindow::on_pricesComplete(const Period& period, const QList<QList<double>>& prices) {
    m_provider.savePrices(period, prices);
    m_provider.pricesFor(period);
    fillPeriods();
    fillPrices();
    fillReview();
    m_dlPricesProg = false;
    enableActions();
}

void MainWindow::fillPrices() {
    Period period = ui->comboPeriod->currentData().value<Period>();
    m_provider.pricesFor(period);
}

void MainWindow::enableActions() {
    ui->actionDownload_prices->setEnabled(!m_dlPricesProg && !m_dlCoursesProg);
    ui->actionOpen_review->setEnabled(!m_dlPricesProg && !m_dlCoursesProg);

    if (!m_dlPricesProg && !m_dlCoursesProg) {
        ui->statusbar->showMessage("Download complete", 2000);
    }
}

void MainWindow::disableActions() {
    m_dlCoursesProg = true;
    m_dlPricesProg = true;
    ui->actionOpen_review->setEnabled(false);
    ui->actionDownload_prices->setEnabled(false);
    ui->statusbar->showMessage("Downloading data in progress...");
}

void MainWindow::fillReview() {
    Period period = ui->comboPeriod->currentData().value<Period>();
    m_provider.reviewFor(period, ui->checkDays->isChecked());
    ui->labelBought->setText(m_provider.totalBought(period));
    ui->labelSold->setText(m_provider.totalEarn(period));
}

void MainWindow::on_checkDays_toggled(bool check) {
    fillReview();
}
