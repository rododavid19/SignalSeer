#include "engine.h"
#include <QAudioInput>
#include <QDebug>

Engine::Engine(QObject *parent)
        :QObject(parent)

{

}


Engine::~Engine()
{

}

void Engine::setFrequency(int freq)
{
    m_frequency = freq;
}

void Engine::setAmplitude(int amp)
{
    m_amplitude = amp;
}

void Engine::setPhase(int phase)
{
    m_phase = phase;
}

void Engine::setAudioInputDevice( QAudioDeviceInfo &device)
{
    m_inputDevice = device;
    qDebug() << "Engine:: Audio InputDevice Set: " << m_inputDevice.deviceName();
}

void Engine::setAudioOutputDevice( QAudioDeviceInfo &device)
{
    m_outputDevice = device;
    qDebug() << "Engine:: Audio OutputDevice Set: " << m_outputDevice.deviceName();
}

void Engine::setState(QAudio::State state)
{
    const bool changed = (m_state != state);
    m_state = state;
    if (changed)
        emit stateChanged(m_mode, m_state);
}

void Engine::setState(QAudio::Mode mode, QAudio::State state)
{
    const bool changed = (m_mode != mode || m_state != state);
    m_mode = mode;
    m_state = state;
    if (changed)
        emit stateChanged(m_mode, m_state);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void Engine::reset(){
    //stopRecording();
    //stopPlayback();
    setState(QAudio::AudioInput, QAudio::StoppedState);
    //setFormat(QAudioFormat());
    //m_generateTone = false;
    //delete m_file;
    //m_file = 0;
    //delete m_analysisFile;
    //m_analysisFile = 0;
    //m_buffer.clear();
    //m_bufferPosition = 0;
    //m_bufferLength = 0;
    //m_dataLength = 0;
    //emit dataLengthChanged(0);
    //resetAudioDevices();

}
bool Engine::loadFile(const QString &fileName)
{
    // Double reset()???
    reset();
    bool result = false;
    //Q_ASSERT(!m_generateTone);
    //Q_ASSERT(!m_file);
    qDebug() << "File Loaded: " << fileName;
    Q_ASSERT(!fileName.isEmpty());
//    m_file = new WavFile(this);
//    if (m_file->open(fileName)) {
//        if (isPCMS16LE(m_file->fileFormat())) {
//            result = initialize();
//        } else {
//            emit errorMessage(tr("Audio format not supported"),
//                              formatToString(m_file->fileFormat()));
//        }
//    } else {
//        emit errorMessage(tr("Could not open file"), fileName);
//    }
//    if (result) {
//        m_analysisFile = new WavFile(this);
//        m_analysisFile->open(fileName);
//    }
    return result;
}
