#include "signaldisplayWidget.h"
#include "ui_signaldisplayWidget.h"
#include <signalsettingsdialog.h>
#include <engine.h>

signalDisplayWidget::signalDisplayWidget(QWidget *parent) :
    QWidget(parent)
  ,ui(new Ui::signalDisplay)
  ,m_engine(new Engine(this))
  ,m_settings_dialog(new signalSettingsDialog(this, m_engine))  // this needs engine
  ,m_mode(NoMode)
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
    m_mode = mode;
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


    }
    if (arg1 == "Generate"){
        this->m_generate_dialog->exec();
        this->generateWave();

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

    ui->waveformPlot->addGraph(); // Will create a new line

    global_buffer.resize(100);
    double m_time = 0.0;
    //TODO: sample rate must change here based on provided wav file
    double delta_time = 1.0/44100.0;

    QVector<double> x(global_buffer.size());
    for (int i = 0; i < global_buffer.size(); i++){
        global_buffer[i] =  amplitude  * sin(2 * M_PI * frequency * m_time + phase);
        x[i] = i;
        m_time += delta_time;
    }
    ui->waveformPlot->xAxis->setRange(0, global_buffer.size());
   // ui->waveformPlot->axisRect()->setupFullAxesBox();

   // ui->waveformPlot->yAxis->setRange(-5.0, -5.0);

    ui->waveformPlot->graph()->addData(x, global_buffer);
    ui->waveformPlot->rescaleAxes();
   // ui->waveformPlot->plotLayout()->insertRow(0);
    //ui->waveformPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->waveformPlot, "Generated Wave", QFont("sans", 12, QFont::Bold)));
    ui->waveformPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->waveformPlot->replot();

}


void signalDisplayWidget::on_startButton_clicked()
{
    // TODO: 1: check that in and out devices have been selected, if not then pop signalsettingsDialog
    //  2: play whatever is in buffer, if buffer is empty prompt the user to select Generate or Load or Record, then do nothing.
    //  if buffer not empty, then play

}
