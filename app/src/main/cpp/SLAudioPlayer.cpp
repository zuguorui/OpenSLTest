//
// Created by zu on 2019/5/12.
//

#include "SLAudioPlayer.h"

bool SLAudioPlayer::createEngine() {
    SLresult result = 0;
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineItf);
    return result == SL_RESULT_SUCCESS;
}

bool SLAudioPlayer::createPlayer() {
    if(engineItf == NULL)
    {
        return false;
    }
    SLresult result;

}