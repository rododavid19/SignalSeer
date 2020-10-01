#include "signalsettingsdialog.h"
#include "ui_signalsettingsdialog.h"

#include <QAudioDeviceInfo>
#include <QAudioDeviceInfo>
#include <engine.h>
#include <QDebug>

signalSettingsDialog::signalSettingsDialog(QWidget *parent, Engine *engine)
    :  QDialog(parent)
    ,m_engine(*engine)
    ,ui(new Ui::signalSettingsDialog)
{
    ui->setupUi(this);
    qDebug() << "signalSettingsDialog:: " << &m_engine;
    m_outputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for ( QAudioDeviceInfo &deviceInfo : m_outputDevices)
        ui->outputDeviceComboBox->addItem(deviceInfo.deviceName(),
                                          QVariant::fromValue(deviceInfo));
    m_inputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for ( QAudioDeviceInfo &deviceInfo : m_inputDevices)
        ui->inputDeviceComboBox->addItem(deviceInfo.deviceName(),
                                         QVariant::fromValue(deviceInfo));

}


signalSettingsDialog::~signalSettingsDialog()
{
    delete ui;
}

void signalSettingsDialog::on_accept_button_clicked()
{
    // TODO:  add checker here for empty field
    m_engine.setAudioInputDevice(m_inputDevice);
    m_engine.setAudioOutputDevice(m_outputDevice);
    this->accept();
}

void signalSettingsDialog::on_inputDeviceComboBox_activated(int index)
{
    m_inputDevice = ui->inputDeviceComboBox->itemData(index).value<QAudioDeviceInfo>();
    qDebug() << "SignalSettingsDialog:: Audio InputDevice Primed : " << m_inputDevice.deviceName();
}

void signalSettingsDialog::on_outputDeviceComboBox_activated(int index)
{
    m_outputDevice = ui->outputDeviceComboBox->itemData(index).value<QAudioDeviceInfo>();
    qDebug() << "SignalSettingsDialog:: Audio OutputDevice Primed :" << m_outputDevice.deviceName();
}

/**
 * Input/output Audio devices change dynamically.
 */
void signalSettingsDialog::on_signalSettingsDialog_rejected()
{
    // Use line below for QDebug in For loops
    // ui->inputDeviceComboBox->itemData(index).value<QAudioDeviceInfo>();
    m_outputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    ui->outputDeviceComboBox->clear();
    m_inputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    ui->inputDeviceComboBox->clear();

    for ( QAudioDeviceInfo &deviceInfo : m_outputDevices)
        ui->outputDeviceComboBox->addItem(deviceInfo.deviceName(),
                                          QVariant::fromValue(deviceInfo));

    for ( QAudioDeviceInfo &deviceInfo : m_inputDevices)
        ui->inputDeviceComboBox->addItem(deviceInfo.deviceName() ,
                                         QVariant::fromValue(deviceInfo));
}
