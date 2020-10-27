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
    QByteArray            global_buffer;
    QVector<double>       graph_buffer;
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
    Mode getMode(){ return mode; }
    void generateWave();

    Mode                    mode;
    Ui::signalDisplay       *ui;
    Engine                  *m_engine;
    signalSettingsDialog    *m_settings_dialog;
    generateSettings        *m_generate_dialog;
    QAudioBuffer            buffer;
    QAudioDecoder           *decoder;
    QCPGraph                *waveGraph;
    QAudioBuffer            audio_buffer;
    QAudioOutput            *audio_out;


private slots:
    void fillBuffer();
    void on_settingsButton_clicked();
    void on_modeBox_activated(const QString &arg1);
    void on_startButton_clicked();
};

#endif // SIGNALDISPLAY_H
