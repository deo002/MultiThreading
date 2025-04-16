#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define N 10

pthread_t handles[N];

void *thread_fn_callback(void *arg) {
    unsigned int* t_id = (unsigned int *)arg;
    unsigned int id = *t_id;
    free(t_id);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    char filename[100];
    sprintf(filename, "thread_%u.txt", id);
    FILE* file_ptr = fopen(filename, "a");

    while(1) {
        fprintf(file_ptr, "Thread with id %u running\n", id);
        fflush(file_ptr);
        sleep(1);
    }
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