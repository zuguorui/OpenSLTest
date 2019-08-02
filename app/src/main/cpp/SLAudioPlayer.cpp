//
// Created by zu on 2019/5/12.
//

#include "SLAudioPlayer.h"
#include "SinTable.h"
#include <android/log.h>

#define MODULE_NAME  "SLAudioPlayer"
#define LOGV(showLog, ...) if((showLog)) {__android_log_print(ANDROID_LOG_VERBOSE, MODULE_NAME, __VA_ARGS__);}
#define LOGD(showLog, ...) if((showLog)) {__android_log_print(ANDROID_LOG_DEBUG, MODULE_NAME, __VA_ARGS__);}
#define LOGI(showLog, ...) if((showLog)) {__android_log_print(ANDROID_LOG_INFO, MODULE_NAME, __VA_ARGS__);}
#define LOGW(showLog, ...) if((showLog)) {__android_log_print(ANDROID_LOG_WARN, MODULE_NAME, __VA_ARGS__);}
#define LOGE(showLog, ...) if((showLog)) {__android_log_print(ANDROID_LOG_ERROR, MODULE_NAME, __VA_ARGS__);}
#define LOGF(showLog, ...) if((showLog)) {__android_log_print(ANDROID_LOG_FATAL, MODULE_NAME, __VA_ARGS__);}

int32_t sinPtr = 0;
int16_t *buffer = NULL;

#define PERIOD 10

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    for(int i = 0; i < BUFFER_LEN / 2; i++)
    {
        int32_t ptr = sinPtr + i * PERIOD;
        int16_t value = (int16_t)(SIN_TABLE[ptr % SIN_TABLE_LEN] * INT16_MAX);
        buffer[2 * i] = value;
        buffer[2 * i + 1] = value;
    }
    sinPtr += BUFFER_LEN / 2 * PERIOD;
    sinPtr = sinPtr % SIN_TABLE_LEN;
    LOGD(true, "player callback");
    (*bq)->Enqueue(bq, buffer, BUFFER_LEN * sizeof(int16_t));
}

SLAudioPlayer::SLAudioPlayer() {
    buffer = (int16_t*)calloc(BUFFER_LEN, sizeof(int16_t));
}

SLAudioPlayer::~SLAudioPlayer() {
    free(buffer);
    buffer = NULL;
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
    SLDataFormat_PCM pcmFormat = {SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                               SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, SL_BYTEORDER_LITTLEENDIAN};

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

    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PAUSED);
    if(result != SL_RESULT_SUCCESS)
    {
        finalResult = false;
    }
    (void*)result;

    bqPlayerCallback(bqPlayerBufferQueue, NULL);
    return finalResult;

}

bool SLAudioPlayer::play() {
    if(bqPlayerPlay == NULL)
    {
        return false;
    }

    SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);

    return result == SL_RESULT_SUCCESS;
}

bool SLAudioPlayer::stop() {
    if(bqPlayerPlay == NULL)
    {
        return false;
    }
    SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_STOPPED);
    return result == SL_RESULT_SUCCESS;
}

bool SLAudioPlayer::destroy() {
    SLresult result;
    if(bqPlayerPlay != NULL)
    {
        result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_STOPPED);
        bqPlayerPlay = NULL;
        (*bqPlayerObject)->Destroy(bqPlayerObject);
        bqPlayerObject = NULL;
        bqPlayerBufferQueue = NULL;
        bqPlayerVolume = NULL;
    }
    if(engineObject != NULL)
    {
        (*engineObject)->Destroy(engineObject);
    }
    return true;
}