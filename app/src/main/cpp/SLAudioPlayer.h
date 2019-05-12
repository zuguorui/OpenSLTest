//
// Created by zu on 2019/5/12.
//

#ifndef OPENSLTEST_SLAUDIOPLAYER_H
#define OPENSLTEST_SLAUDIOPLAYER_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>


class SLAudioPlayer {
public:
    bool createEngine();
    bool createPlayer();

private:
    SLPlayItf playerItf = NULL;
    SLEngineItf engineItf = NULL;
    SLObjectItf engineObject = NULL;
    SLObjectItf playerObject = NULL;
};


#endif //OPENSLTEST_SLAUDIOPLAYER_H
