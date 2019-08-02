//
// Created by zu on 2019/5/12.
//

#ifndef OPENSLTEST_SLAUDIOPLAYER_H
#define OPENSLTEST_SLAUDIOPLAYER_H

#include <iostream>
#include <stdlib.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define BUFFER_LEN 128


class SLAudioPlayer {
public:
    SLAudioPlayer();
    ~SLAudioPlayer();
    bool createEngine();
    bool createPlayer();
    bool play();
    bool stop();

    bool destroy();

private:

    // engine interfaces
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine;

// output mix interfaces
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;

    SLObjectItf bqPlayerObject = NULL;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
    SLVolumeItf bqPlayerVolume;
    SLmilliHertz bqPlayerSampleRate = 0;

};


#endif //OPENSLTEST_SLAUDIOPLAYER_H
