#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define  LOGE(ignore, ...)  __android_log_print(ANDROID_LOG_ERROR, "graphhopper", __VA_ARGS__)


extern "C" {
    int run_main(int paramArgc, char** paramArgv);
// original was: int *run_main(int paramArgc, char** paramArgv);
}

int start_logger(const char *app_name);
static int pfd[2];
static pthread_t thr;
static const char *tag = "myapp";

// we need this and the start_logger since android eats fprintf
static void *thread_func(void * arg)
{
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
int start_logger(const char *app_name)
{
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


int callGraal() {
    LOGE(stderr, "We called Graal");

    double lat1 = 10.;
    double lon1 = 10.;
    double lat2 = 10.;
    double lon2 = 11.0;
    LOGE(stderr, "We should start runGH now");
    fprintf(stderr, "We should start runGH now from FPRINTF");

    const char *args[] = {"myapp", "rungh", "10.", "11.", "12.03", "3.1415"};
    run_main(6, (char **) args);

    LOGE(stderr, "We DID start runGH now");
    return 12;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void* reserved) {
    fprintf(stderr, "[GDBG] JNI Onload for nativelib\n");
    start_logger("GraalCompiled");
    LOGE(stderr, "[GDBG] JNI Onload for nativelib\n");
    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT jint JNICALL Java_com_graphhopper_myapplication_MainActivity_fromNative
        (JNIEnv *env, jobject activity) {
    LOGE(stderr, "[GDBG] string from native called \n");
    return callGraal();
}
