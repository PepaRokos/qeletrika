//
// Created by pepa on 21.9.22.
//

#ifndef QELETRIKA_SETTINGS_H
#define QELETRIKA_SETTINGS_H

#include <QDialog>
#include "autotablemodel.h"
#include "data.h"


class SettingsTableModel : public AutoTableModel<LowTariffTime> {
Q_OBJECT
public:
    SettingsTableModel(QObject* parent = nullptr) : AutoTableModel<LowTariffTime>(parent) {}
};

QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE

class SettingsDialog : public QDialog {
Q_OBJECT

public:
    explicit SettingsDialog(const Settings& settings, QWidget* parent = nullptr);
    ~SettingsDialog() override;

    const Settings& settings() const;

private slots:
    void on_buttonBox_accepted();
    void on_toolButtonAdd_clicked();
    void on_toolButtonRemove_clicked();
    void on_toolButtonAddHoliday_clicked();
    void on_toolButtonRemoveHoliday_clicked();

private:
    Ui::Settings* ui;
    SettingsTableModel* m_lowPriceModel;
    SettingsTableModel* m_lowPriceModelHoliday;
    Settings m_settings;
};


#endif //QELETRIKA_SETTINGS_H
