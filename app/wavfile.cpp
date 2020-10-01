#include "wavfile.h"
#include <qendian.h>

/*
Positions 	Sample Value 	Description
1 - 4 	"RIFF" 	Marks the file as a riff file. Characters are each 1 byte long.
5 - 8 	File size (integer) 	Size of the overall file - 8 bytes, in bytes (32-bit integer). Typically, you'd fill this in after creation.
9 -12 	"WAVE" 	File Type Header. For our purposes, it always equals "WAVE".
13-16 	"fmt " 	Format chunk marker. Includes trailing null
17-20 	16 	Length of format data as listed above
21-22 	1 	Type of format (1 is PCM) - 2 byte integer
23-24 	2 	Number of Channels - 2 byte integer
25-28 	44100 	Sample Rate - 32 byte integer. Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz.
29-32 	176400 	(Sample Rate * BitsPerSample * Channels) / 8.
33-34 	4 	(BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo
35-36 	16 	Bits per sample
37-40 	"data" 	"data" chunk header. Marks the beginning of the data section.
41-44 	File size (data) 	Size of the data section.

Chunk ID  "RIFF"  in  ASCII  ("RIFX"  files  identify  the
                 samples audio data in hi/lo format instead of  the  normal
                 lo/hi format)
*/

struct chunk
{
    char        id[4]; //"fmt "
    quint32     size;
};
struct RIFFHeader
{
    chunk       descriptor;     // "RIFF"
    char        type[4];        // "WAVE"
};
struct WAVEHeader
{
    chunk       descriptor;
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
};
struct DATAHeader
{
    chunk       descriptor;
};

struct CombinedHeader
{
    RIFFHeader  riff;
    WAVEHeader  wave;
};



wavFile::wavFile(QObject *parent)
    : QFile(parent)
    , m_headerLength(0)
{

}

bool wavFile::open(const QString &fileName)
{
    close();
    setFileName(fileName);
    return QFile::open(QIODevice::ReadOnly) && readHeader();
}


const QAudioFormat &wavFile::fileFormat() const
{
    return m_fileFormat;
}

qint64 wavFile::headerLength() const
{
    return m_headerLength;
}

bool wavFile::readHeader()
{
    seek(0);
    CombinedHeader header;
    // compile time directive to change header to char* if intercovertible
    // bool, with implicit side effect. Initiates data read()
    bool result = read(reinterpret_cast<char *>(&header), sizeof(CombinedHeader)) == sizeof(CombinedHeader);
    if (result) {
        if ((memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0
            || memcmp(&header.riff.descriptor.id, "RIFX", 4) == 0)
            && memcmp(&header.riff.type, "WAVE", 4) == 0
            && memcmp(&header.wave.descriptor.id, "fmt ", 4) == 0
            && (header.wave.audioFormat == 1 || header.wave.audioFormat == 0)) {

            // Read off remaining header information
            DATAHeader dataHeader;

            // 'CHUNK'
            if (qFromLittleEndian<quint32>(header.wave.descriptor.size) > sizeof(WAVEHeader)) {
                // Extended data available

                // Some type of non-PCM checker?
                quint16 extraFormatBytes;
                if (peek((char*)&extraFormatBytes, sizeof(quint16)) != sizeof(quint16))
                    return false;
                const qint64 throwAwayBytes = sizeof(quint16) + qFromLittleEndian<quint16>(extraFormatBytes);
                if (read(throwAwayBytes).size() != throwAwayBytes)
                    return false;
            }

            if (read((char*)&dataHeader, sizeof(DATAHeader)) != sizeof(DATAHeader))
                return false;

            // Establish format
            if (memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0)
                m_fileFormat.setByteOrder(QAudioFormat::LittleEndian);
            else
                m_fileFormat.setByteOrder(QAudioFormat::BigEndian);

            int bps = qFromLittleEndian<quint16>(header.wave.bitsPerSample);
            m_fileFormat.setChannelCount(qFromLittleEndian<quint16>(header.wave.numChannels));
            m_fileFormat.setCodec("audio/pcm");
            m_fileFormat.setSampleRate(qFromLittleEndian<quint32>(header.wave.sampleRate));
            m_fileFormat.setSampleSize(qFromLittleEndian<quint16>(header.wave.bitsPerSample));
            m_fileFormat.setSampleType(bps == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
        } else {
            result = false;
        }
    }
    m_headerLength = pos();
    return result;
}
