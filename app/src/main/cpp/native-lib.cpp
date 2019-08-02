#include <jni.h>
#include <string>
#include "SLAudioPlayer.h"

SLAudioPlayer *audioPlayer = NULL;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_opensltest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jboolean JNICALL
        Java_com_example_opensltest_MainActivity_nCreateEngine(JNIEnv* env, jobject instance)
{
    audioPlayer = new SLAudioPlayer();
    return audioPlayer->createEngine();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_opensltest_MainActivity_nCreatePlayer(JNIEnv* env, jobject instance)
{
    if(audioPlayer == NULL)
    {
        return false;
    }
    return audioPlayer->createPlayer();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_opensltest_MainActivity_nPlay(JNIEnv* env, jobject instance)
{
    if(audioPlayer == NULL)
    {
        return false;
    }
    return audioPlayer->play();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_opensltest_MainActivity_nStop(JNIEnv* env, jobject instance)
{
    if(audioPlayer == NULL)
    {
        return false;
    }
    return audioPlayer->stop();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_opensltest_MainActivity_nDestroy(JNIEnv* env, jobject instance)
{
    if(audioPlayer == NULL)
    {
        return false;
    }
    audioPlayer->destroy();
    audioPlayer = NULL;
    return true;
}




