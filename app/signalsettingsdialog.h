#ifndef SIGNALSETTINGSDIALOG_H
#define SIGNALSETTINGSDIALOG_H

#include <QAudioDeviceInfo>
#include <QDialog>
#include <QWidget>
#include <engine.h>

namespace Ui {
class signalSettingsDialog;
}

class signalSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    signalSettingsDialog(QWidget *parent = 0, Engine *engine = 0);
    ~signalSettingsDialog();

private:
    Engine  &m_engine;
    Ui::signalSettingsDialog *ui;
    QAudioDeviceInfo m_inputDevice;
    QAudioDeviceInfo m_outputDevice;

    QList<QAudioDeviceInfo> m_outputDevices;
    QList<QAudioDeviceInfo> m_inputDevices;

private slots:
    void on_accept_button_clicked();
    void on_inputDeviceComboBox_activated(int index);
    void on_outputDeviceComboBox_activated(int index);
    void on_signalSettingsDialog_rejected();
};

#endif // SIGNALSETTINGSDIALOG_H
