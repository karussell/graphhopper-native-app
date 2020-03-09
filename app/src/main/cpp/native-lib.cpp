#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_graphhopper_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello Test";
    return env->NewStringUTF(hello.c_str());
}
