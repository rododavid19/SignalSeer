#include "signaldisplayWidget.h"
#include "ui_signaldisplayWidget.h"
#include <signalsettingsdialog.h>
#include <engine.h>

signalDisplayWidget::signalDisplayWidget(QWidget *parent) :
    QWidget(parent)
  ,ui(new Ui::signalDisplay)
  ,m_engine(new Engine(this))
  ,m_settings_dialog(new signalSettingsDialog(this, m_engine))  // this needs engine
  ,mode(NoMode)
  ,m_generate_dialog(new generateSettings(this, m_engine))        // this needs engine
  ,decoder(new QAudioDecoder(this))

{
    ui->setupUi(this);
    qDebug() << "signalDisplayWidget:: Instantiated: "  << m_engine;

    connect(decoder, SIGNAL(bufferReady()), this, SLOT(fillBuffer()) );
    emit(decoder->bufferReady());

}

signalDisplayWidget::~signalDisplayWidget()
{
    delete ui;
}

void signalDisplayWidget::reset()
{
    m_engine->reset();
}

void signalDisplayWidget::on_settingsButton_clicked()
{
    m_settings_dialog->exec();

    QString input_device = m_engine->getinputDevice().deviceName();
    if (input_device != ""){
        ui->inputDevice->setText(input_device);
    }
    QString output_device = m_engine->getoutputDevice().deviceName();
    if (output_device != ""){
        ui->outputDevice->setText(output_device);
    }
}



// TODO:
void signalDisplayWidget::setMode(Mode mode)
{
    this->mode = mode;
}


void signalDisplayWidget::fillBuffer()
{
    qDebug() << "fillBuffer Called";
}

void signalDisplayWidget::on_modeBox_activated(const QString &arg1)
{
    if (arg1 == "Load"){
        const QString dir;
        const QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open WAV file"), dir, "*.wav");

        // TODO: Check audio format, i.e mp3, acc, lossless (decompression needed)
        // Gather all wavFile data from HEADER. i.e duration, amplitude, enough to Plot!
        // consider stereo


    }
    if (arg1 == "Generate"){
        m_generate_dialog->exec();
        setMode(GenerateMode);
        generateWave(); //TODO: refactor to engine class and then call using Engine reference

    }
    if (arg1 == "Record"){

    }
}

/**
Generates Sine Wave with selected parameters from generateDialog
*/
void signalDisplayWidget::generateWave()
{
    auto amplitude = m_engine->getAmplitude();
    auto frequency = m_engine->getFrequency();
    auto phase = m_engine->getPhase();

    // TODO:  Future feature could be adding a gprrah to the same plot and allowing user
    // to select wave that they want to play
    ui->waveformPlot->clearGraphs();
    ui->waveformPlot->addGraph(); // Will create a new line,

    global_buffer.resize(1*44100);
    graph_buffer.resize(1*44100);
    double m_time = 0.0;
    //TODO: sample rate must change here based on provided wav file
    double delta_time = 1.0/44100.0;

    QVector<double> x(global_buffer.size());

    // first define max amplitude for PCM, based off bits per sample. i.e if bitsPerSample = 16, then this is 2^15-1
    //       i < samplerateHZ * seconds
    for(int i =0; i< 44100 * 1; i++ ){
        double timeIndexSecond = (double)i/ 44100.0;//  44100 needs to change to sampleRateHZ
        double tonePeriodSeconds = 1.0 / frequency;
        double radians = timeIndexSecond/ tonePeriodSeconds * (2*M_PI);
        double sample = sin(radians);
        //sample get envelope
        //sample get volume
        x[i] = timeIndexSecond;
        double samplePCM = (sample + 1.0) / 2.0 * 256;
        //TODO: here amplitude is divided by 100 becasue it was arbitraily chosen, this shoudl change
        global_buffer[i] = samplePCM; //* amplitude/100;
        graph_buffer[i] = sample;

    }

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    ui->waveformPlot->xAxis->setTicker(timeTicker);

    ui->waveformPlot->graph()->addData(x, graph_buffer);
    ui->waveformPlot->rescaleAxes();
    ui->waveformPlot->axisRect()->setupFullAxesBox();
   // ui->waveformPlot->plotLayout()->insertRow(0);
    //ui->waveformPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->waveformPlot, "Generated Wave", QFont("sans", 12, QFont::Bold)));
    ui->waveformPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->waveformPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    //TODO: if you add QCP::iSelectPlotabbles to Interactions, and lines of code below then you can select range of data from plot
    //ui->waveformPlot->setSelectionRectMode(QCP::srmSelect);
    //ui->waveformPlot->graph()->setSelectable(QCP::stDataRange);
    ui->waveformPlot->replot();

}


void signalDisplayWidget::on_startButton_clicked()
{
    // TODO: 1: check that in and out devices have been selected, if not then pop signalsettingsDialog
    //  2: play whatever is in buffer, if buffer is empty prompt the user to select Generate or Load or Record, then do nothing.
    //  if buffer not empty, then play


    //TODO: get format from engine and then instantiate or use reference
    // TODO: 1st check mode
    //TODO: if user has not selected out/in devices then set default as selected and continue
    const auto outputDevice = this->m_engine->getoutputDevice();

    const Mode current_mode = getMode();
    if (current_mode == NoMode){
        int a = 4;
    }
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt); // TODO: doesn't seeem to matter?...

    audio_out = new QAudioOutput(format, this);

    // TODO: Change this to setDevice from engine, and if No device was set then select default
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
        if (!info.isFormatSupported(format)) {
            qWarning() << "Raw audio format not supported by backend, cannot play audio.";
            return;
        }
        //TODO: setBuffer to sampleRate * Seconds
    audio_out->setBufferSize(44100);
    auto out_device = audio_out->start();
    //qDebug() << "AudioDevice Buffer size is " << audio_out->bufferSize();
    auto bytesWritten = out_device->write(global_buffer);
    //qDebug() << "AudioDevice played " << audio_out->bufferSize() << " bytes";

}

