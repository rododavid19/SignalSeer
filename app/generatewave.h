#ifndef GENERATEWAVE_H
#define GENERATEWAVE_H
#include <engine.h>


class generateWave
{
public:
    generateWave();
    void initialize();

private:
    // TODO:  Maybe Engine instance can hold all this info. Just use reference
    // to call getters/setters
    float m_amplitude;
    float m_frequency;
    float m_phase;
    float m_time;
    float m_delta_time;

    void initializeGeneration(double sampleRate);
    void getBufferWriter();

    //Engine &m_engine;

};

#endif // GENERATEWAVE_H
