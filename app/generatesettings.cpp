#include "generatesettings.h"
#include "ui_generatesettings.h"
#include "QtDebug"

generateSettings::generateSettings(QWidget *parent, Engine *engine) :
    QDialog(parent)
  ,ui(new Ui::generateSettings)
  ,m_engine(*engine)
{
    ui->setupUi(this);
    qDebug() << "generateSettings:: By Reference: "  << &m_engine;

}

generateSettings::~generateSettings()
{
    delete ui;
}

void generateSettings::on_frequencySpinBox_valueChanged(int value)
{
    ui->frequency_dial->setValue(value);
}

void generateSettings::on_frequency_dial_valueChanged(int value)
{
    ui->frequencySpinBox->setValue(value);
}

void generateSettings::on_amplitude_dial_valueChanged(int value)
{
    ui->amplitudeSpinBox->setValue(value);
}

void generateSettings::on_amplitudeSpinBox_valueChanged(int value)
{
    ui->amplitude_dial->setValue(value);
}

void generateSettings::on_phaseSlider_valueChanged(int value)
{
    ui->phaseSpinBox->setValue(value);
}

void generateSettings::on_phaseSpinBox_valueChanged(int value)
{
    ui->phaseSlider->setValue(value);
}

void generateSettings::on_buttonBox_accepted()
{
    m_engine.setFrequency(ui->frequency_dial->value());
    m_engine.setAmplitude(ui->amplitude_dial->value());
    m_engine.setPhase(ui->phaseSlider->value());

}

void generateSettings::on_buttonBox_rejected()
{
    int amp = m_engine.getAmplitude();
    int freq = m_engine.getFrequency();
    auto phase = m_engine.getPhase();
    ui->phaseSlider->setValue(phase);
    ui->phaseSpinBox->setValue(phase);
    ui->amplitude_dial->setValue(amp);
    ui->amplitudeSpinBox->setValue(amp);
    ui->frequency_dial->setValue(freq);
    ui->frequencySpinBox->setValue(freq);
    //ui->
}

void generateSettings::on_generateSettings_rejected()
{
    int amp = m_engine.getAmplitude();
    int freq = m_engine.getFrequency();
    auto phase = m_engine.getPhase();
    ui->phaseSlider->setValue(phase);
    ui->phaseSpinBox->setValue(phase);
    ui->amplitude_dial->setValue(amp);
    ui->amplitudeSpinBox->setValue(amp);
    ui->frequency_dial->setValue(freq);
    ui->frequencySpinBox->setValue(freq);
}


