#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#define N 10

pthread_t handles[N];

void mem_cleanup_handler(void *arg) {
    free(arg);
}

void file_handler_cleanup_handler(void *arg) {
    fclose(arg);
}

void *thread_fn_callback(void *arg) {
    pthread_cleanup_push(mem_cleanup_handler, arg);
    unsigned int* t_id = (unsigned int *)arg;
    unsigned int id = *t_id;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    char filename[100];
    sprintf(filename, "thread_%u.txt", id);
    FILE* file_ptr = fopen(filename, "a");
    if(file_ptr == NULL) {
        // return statement doesn't invoke the handlers, but pthread_exit does.
        // So, instead of return, it would be a better idea to use pthread_exit.
        printf("Couldn't open file %s, errno= %d\n", filename, errno);
        pthread_exit(0); 
    }
    pthread_cleanup_push(file_handler_cleanup_handler, file_ptr);

    while(1) {
        fprintf(file_ptr, "Thread with id %u running\n", id);
        fflush(file_ptr);
        sleep(1);
    }

    // passing them a non zero value as arg ensures that handlers are executed as well
    // as popped out
    // passing them a zero value just pops them out without executing them

    // So, when the loop stops executing and the funtion terminates normally, 
    // the cleanup handlers will still be called and popped out
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    return 0;
}

int main() {

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    for(unsigned int i = 0; i < N; i++) {
        unsigned int* t_id = calloc(1, sizeof(unsigned int));
        *t_id = i;
        pthread_create(&handles[i], &attr, thread_fn_callback, (void *)t_id);
    }

    while(1) {
        unsigned int tid;
        printf("Enter the thread id to terminate:\n");
        scanf("%u", &tid);
        pthread_cancel(handles[tid]);
    }

    pthread_exit(0);

    return 0;
}