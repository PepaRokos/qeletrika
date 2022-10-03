//
// Created by pepa on 21.9.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Settings.h" resolved

#include "settings.h"
#include "ui_settings.h"


SettingsDialog::SettingsDialog(const Settings& settings, QWidget* parent) :
        QDialog(parent), ui(new Ui::Settings) {
    ui->setupUi(this);

    ui->lineEditMonth->setText(QString::number(settings.monthFee(), 'f', 2));
    ui->lineEditDistributionHigh->setText(QString::number(settings.distributionFeeHigh(), 'f', 2));
    ui->lineEditDistributionLow->setText(QString::number(settings.distributionFeeLow(), 'f', 2));
    ui->groupBoxFix->setChecked(settings.fixPrice());
    ui->lineEditLow->setText(QString::number(settings.lowPrice(), 'f', 2));
    ui->lineEditHigh->setText(QString::number(settings.highPrice(), 'f', 2));

    m_lowPriceModel = new SettingsTableModel(this);
    m_lowPriceModel->setData(settings.lowTariff());
    m_lowPriceModel->setEditableCols({0, 1});
    ui->tableLowTarifTimes->setModel(m_lowPriceModel);

    m_lowPriceModelHoliday = new SettingsTableModel(this);
    m_lowPriceModelHoliday->setData(settings.lowTariffHoliday());
    m_lowPriceModelHoliday->setEditableCols({0, 1});
    ui->tableLowTariffHoliday->setModel(m_lowPriceModelHoliday);
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted() {
    m_settings = {ui->lineEditMonth->text().toDouble(),
                  ui->lineEditDistributionLow->text().toDouble(),
                  ui->lineEditDistributionHigh->text().toDouble(),
                  ui->groupBoxFix->isChecked(),
                  ui->lineEditLow->text().toDouble(),
                  ui->lineEditHigh->text().toDouble(),
                  m_lowPriceModel->list(),
                  m_lowPriceModelHoliday->list()};
    accept();
}

const Settings& SettingsDialog::settings() const {
    return m_settings;
}

void SettingsDialog::on_toolButtonAdd_clicked() {
    m_lowPriceModel->addRow(LowTariffTimePtr::create());
}

void SettingsDialog::on_toolButtonRemove_clicked() {
    auto index = ui->tableLowTarifTimes->currentIndex();
    if (index.row() >= 0) {
        m_lowPriceModel->removeRowAt(index);
    }

}

void SettingsDialog::on_toolButtonAddHoliday_clicked() {
    m_lowPriceModelHoliday->addRow(LowTariffTimePtr::create());
}

void SettingsDialog::on_toolButtonRemoveHoliday_clicked() {
    auto index = ui->tableLowTariffHoliday->currentIndex();
    if (index.row() >= 0) {
        m_lowPriceModelHoliday->removeRowAt(index);
    }
}
