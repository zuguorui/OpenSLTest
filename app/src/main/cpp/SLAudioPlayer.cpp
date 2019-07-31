//
// Created by zu on 2019/5/12.
//

#include "SLAudioPlayer.h"

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{

}

bool SLAudioPlayer::createEngine() {
    bool finalResult = true;
    SLresult result;
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    SLInterfaceID ids[1] = {SL_IID_OUTPUTMIX};
    SLboolean reqs[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, ids, reqs);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    return finalResult;
}

bool SLAudioPlayer::createPlayer() {
    bool finalResult = true;
    SLresult result;

    SLDataLocator_AndroidSimpleBufferQueue bufferQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM pcmFormat = {SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16 * 2,
                               SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};

    SLDataSource audioSrc = {&bufferQueue, &pcmFormat};

    SLDataLocator_OutputMix locOutputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&locOutputMix, NULL};

    SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
    SLboolean reqs[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSink, 2, ids, reqs);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    return finalResult;

}