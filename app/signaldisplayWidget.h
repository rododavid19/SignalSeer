#ifndef SIGNALDISPLAY_H
#define SIGNALDISPLAY_H

#include <QWidget>
#include <signalsettingsdialog.h>
#include <engine.h>
#include <generatesettings.h>
#include <QFileDialog>
#include<QAction>
#include <QAudioBuffer>
#include <QAudioDecoder>
#include <qcustomplot.h>
#include <QVector>

class Engine;
class signalSettingsDialog;

namespace Ui {
class signalDisplay;
}

class signalDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit signalDisplayWidget(QWidget *parent = nullptr);
    ~signalDisplayWidget();
    QVector<double>            global_buffer;

private:
    enum Mode {
        NoMode,
        RecordMode,
        GenerateMode,
        LoadFileMode
    };

    // TODO: notice that Engine, and signalWidget have their own reset()
    void reset();
    void setMode(Mode mode);
    void generateWave();

    Mode                    m_mode;
    Ui::signalDisplay       *ui;
    Engine                  *m_engine;
    signalSettingsDialog    *m_settings_dialog;
    generateSettings        *m_generate_dialog;
    QAudioBuffer            buffer;
    QAudioDecoder           *decoder;
    QCPGraph                *waveGraph;


private slots:
    void fillBuffer();
    void on_settingsButton_clicked();
    void on_modeBox_activated(const QString &arg1);
    void on_startButton_clicked();
};

#endif // SIGNALDISPLAY_H
