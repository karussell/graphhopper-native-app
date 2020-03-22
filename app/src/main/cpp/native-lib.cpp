#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define  LOGE(ignore, ...)  __android_log_print(ANDROID_LOG_ERROR, "graphhopper", __VA_ARGS__)

extern "C" {
    // this interface is from GraalVM
    // https://github.com/oracle/graal/blob/master/substratevm/src/com.oracle.svm.hosted/src/com/oracle/svm/hosted/image/AbstractBootImage.java#L81
    int run_main(int paramArgc, char** paramArgv);
}

int start_logger(const char *app_name);
static int pfd[2];
static pthread_t thr;
static const char *tag = "myapp";

// for graphhopper logs we need this and the start_logger since android eats fprintf
static void *thread_func(void * arg) {
    ssize_t rdsz;
    char buf[128];
    while ((rdsz = read(pfd[0], buf, sizeof buf - 1)) > 0)
    {
        if (buf[rdsz - 1] == '\n')
            --rdsz;
        buf[rdsz] = 0; /* add null-terminator */
        __android_log_write(ANDROID_LOG_DEBUG, tag, buf);
    }
    return 0;
}

int start_logger(const char *app_name) {
    tag = app_name;

    /* make stdout line-buffered and stderr unbuffered */
    setvbuf(stdout, 0, _IOLBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);

    /* create the pipe and redirect stdout and stderr */
    pipe(pfd);
    dup2(pfd[1], 1);
    dup2(pfd[1], 2);

    /* spawn the logging thread */
    if (pthread_create(&thr, 0, thread_func, 0) == -1)
        return -1;
    pthread_detach(thr);
    return 0;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void* reserved) {
    start_logger("GraphHopper");
    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT jdouble JNICALL Java_com_graphhopper_myapplication_MainActivity_fromNative
        (JNIEnv *env, jobject activity) {
    LOGE(stderr, "Call GraphHopper");
    // /mnt/sdcard/Download/graphhopper/maps/europe_germany_berlin-gh/
    const char *args[] = {"myapp", "rungh", "/data/local/tmp/europe_germany_berlin-gh/", "10.", "11.", "12.03", "3.1415"};
    double res = run_main(6, (char **) args);
    return res;
}
