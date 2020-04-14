#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>

#define  LOGE(ignore, ...)  __android_log_print(ANDROID_LOG_ERROR, "graphhopper", __VA_ARGS__)

extern "C" {
// this interface is from GraalVM
// https://github.com/oracle/graal/blob/master/substratevm/src/com.oracle.svm.hosted/src/com/oracle/svm/hosted/image/AbstractBootImage.java#L81
int run_main(int paramArgc, char **paramArgv);
}

int start_logger(const char *app_name);

static int pfd[2];
static pthread_t thr;
static const char *tag = "myapp";

// for graphhopper logs we need this and the start_logger since android eats fprintf
static void *thread_func(void *arg) {
    ssize_t rdsz;
    char buf[128];
    while ((rdsz = read(pfd[0], buf, sizeof buf - 1)) > 0) {
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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    start_logger("GraphHopper");
    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT jstring JNICALL Java_com_graphhopper_myapplication_MainActivity_fromNative
        (JNIEnv *env, jobject activity, jstring jExchangeFileName) {
    LOGE(stderr, "Call GraphHopper");

    const char *exchange_file_name = env->GetStringUTFChars(jExchangeFileName, 0);
    // Berlin (0.04s)
    const char *args[] = {"myapp", "rungh", "/data/local/tmp/graph-cache/", exchange_file_name,
                          "52.5169", "13.3884", "52.5147", "13.3883"};
    // Dresden -> Berlin (1s)
    // const char *args[] = {"myapp", "rungh", "/data/local/tmp/graph-cache/", exchange_file_name, "51.131108", "13.721924", "52.5147", "13.3883"};
    // Hof -> Berlin (2s)
    // const char *args[] = {"myapp", "rungh", "/data/local/tmp/graph-cache/", exchange_file_name, "50.190968", "11.678467", "52.5147", "13.3883"};
    // Nürnberg -> Berlin (4s) also crashes sometimes
    // const char *args[] = {"myapp", "rungh", "/data/local/tmp/graph-cache/", exchange_file_name, "49.368066", "11.217041", "52.5147", "13.3883"};
    // Munich -> Berlin ... crashes probably because of too much memory but even largeHeap does not help :(
    // A/libc: Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x75150ff038 in tid 25991 (r.myapplication), pid 25991 (r.myapplication)
    // https://stackoverflow.com/q/17840521/194609
    // const char *args[] = {"myapp", "rungh", "/data/local/tmp/graph-cache/", exchange_file_name, "48.118463", "11.539282", "52.5147", "13.3883"};

    // TODO does not work multiple times and not for longer routes -> how is memory management done?
    run_main(8, (char **) args);

    // uh, is this ugliness really necessary?
    env->ReleaseStringUTFChars(jExchangeFileName, exchange_file_name);

    // now read created json
    std::ifstream ifs(exchange_file_name);
    std::string fileContent((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));
    return env->NewStringUTF(fileContent.c_str());
}
