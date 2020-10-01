#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QAudioBuffer>

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QAudioFormat>
#include <QBuffer>
#include <QByteArray>
#include <QAudioDecoder>

class QAudioInput;
class QAudioOutput;

// Use references when you can, and pointers when you have to
class Engine:public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent);
    ~Engine();
    QAudioDeviceInfo &getinputDevice()  { return m_inputDevice; }
    QAudioDeviceInfo &getoutputDevice() { return m_outputDevice; }
    void setAudioInputDevice( QAudioDeviceInfo &device);
    void setAudioOutputDevice( QAudioDeviceInfo &device);

    int &getFrequency() {return m_frequency;}
    int &getAmplitude() {return m_amplitude;}
    int &getPhase() {return m_phase;}
    void setPhase(int phase);
    void setFrequency(int freq);
    void setAmplitude(int amp);


    void generateWave();


    // 1: audioOutput, 0: audioInput
    QAudio::Mode mode() const { return m_mode; }

    /* STATE
     * 0: processing
     * 1: suspended
     * 2: closed
     * 3: Idle
     * 4: interrupted
     */
    QAudio::State state() const { return m_state; }
    void reset();
    bool loadFile(const QString &fileName);

signals:
   void stateChanged(QAudio::Mode mode, QAudio::State state);

//public slots:
//    void setAudioInputDevice(const QAudioDeviceInfo &device);
//    void setAudioOutputDevice(const QAudioDeviceInfo &device);


private:
   int m_frequency;
   int m_amplitude;
   int m_phase;
   QAudioDeviceInfo m_inputDevice;
   QAudioDeviceInfo m_outputDevice;

//   QList<QAudioDeviceInfo> m_outputDevices;
//   QList<QAudioDeviceInfo> m_inputDevices;

   void setState(QAudio::State state);
   void setState(QAudio::Mode mode, QAudio::State state);

   QAudioDecoder *decoder;
   QAudioBuffer buffer;
//    WavFile*            m_file;
    // We need a second file handle via which to read data into m_buffer
    // for analysis
//    WavFile*            m_analysisFile;
    QAudio::Mode        m_mode;
    QAudio::State       m_state;
    QAudioFormat        m_format;
//    QAudioDeviceInfo m_inputDevice;
//    QAudioDeviceInfo m_outputDevice;
    // list more than likely not needed, only selected device. signalSettings takes care of lists
//    QList<QAudioDeviceInfo> m_outputDevices;
//    QList<QAudioDeviceInfo> m_inputDevices;


};
#endif // ENGINE_H
