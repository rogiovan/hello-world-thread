#include <stdlib.h>
#include <errno.h>
#include <sys/utsname.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MESSAGE_MAX_LEN (100)

typedef struct
{
    pthread_t thread_id;  // ID returned by pthread_create()
    char message[MESSAGE_MAX_LEN];
} thread_params_t;

void log_info(const char *message)
{
    char log_message[128];
    sprintf(log_message, "[COURSE:1][ASSIGNMENT:1]: %s\n", message);
    syslog(LOG_INFO, "%s", log_message);
}

void *thread_worker(void *thread_params)
{
    thread_params_t *params = (thread_params_t *)thread_params;
    if (params == NULL)
    {
        fprintf(stderr, "NULL pointer!");
        exit(EXIT_FAILURE);
    }
    log_info(params->message);
}

int main(int argc, char *argv[])
{
    thread_params_t thread_params;
    char message[MESSAGE_MAX_LEN];

    /* Retrieve system information.*/
    int result = 0;
    struct utsname uname_buff;
    result = uname(&uname_buff);
    if (result == -1)
    {
        int error_code = errno;
        fprintf(stderr, "Error calling \"uname\": %s\n",
                strerror(error_code));
        exit(EXIT_FAILURE);
    }
    /* Log system information.*/
    sprintf(message, "%s %s %s %s %s %s",
            uname_buff.sysname,
            uname_buff.nodename,
            uname_buff.release,
            uname_buff.version,
            uname_buff.machine,
            "GNU/Linux");
    log_info(message);

    /* Log greeting message from main.*/
    log_info("Hello World from Main!");

    /* Create thread.*/
    sprintf(thread_params.message, "Hello World from Thread!");
    pthread_create(&thread_params.thread_id,
                   NULL,  // Use default attributes.
                   thread_worker,
                   (void *)&thread_params);

    /* Wait for thread to terminate.*/
    pthread_join(thread_params.thread_id, NULL);

    exit(EXIT_SUCCESS);
}