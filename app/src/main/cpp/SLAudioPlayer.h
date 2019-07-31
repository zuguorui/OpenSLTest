//
// Created by zu on 2019/5/12.
//

#ifndef OPENSLTEST_SLAUDIOPLAYER_H
#define OPENSLTEST_SLAUDIOPLAYER_H

#include <iostream>
#include <stdlib.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>


class SLAudioPlayer {
public:
    bool createEngine();
    bool createPlayer();

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

    static const SLEnvironmentalReverbSettings reverbSettings =
            SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
};


#endif //OPENSLTEST_SLAUDIOPLAYER_H
