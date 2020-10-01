#ifndef GENERATESETTINGS_H
#define GENERATESETTINGS_H

#include <QDialog>
#include "signalsettingsdialog.h"

namespace Ui {
class generateSettings;
}
class signalSettingsDialog;

class generateSettings : public QDialog
{
    Q_OBJECT

public:
    explicit generateSettings(QWidget *parent = nullptr, Engine *engine = 0);
    ~generateSettings();


private:
//    int m_frequency;
//    int m_amplitude;
    Engine &m_engine;


private slots:

    void on_frequencySpinBox_valueChanged(int arg1);

    void on_frequency_dial_valueChanged(int value);

    void on_amplitude_dial_valueChanged(int value);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_generateSettings_rejected();

    void on_amplitudeSpinBox_valueChanged(int arg1);

    void on_phaseSlider_valueChanged(int value);

    void on_phaseSpinBox_valueChanged(int arg1);

private:
    Ui::generateSettings *ui;
};

#endif // GENERATESETTINGS_H
