//
//  main.c
//  MultiThreading
//
//  Created by Dearsh Oberoi on 22/03/25.
//

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

/* Any thread can invoke pthread_join for any other joinable thread, not just parent thread */

void *thread_fn_callback(void *th_id) {
    int *_th_id = (int *)th_id;
    
    int cnt = 0;
    while(cnt < *_th_id) {
        printf("Thread with th_id %d doing some work\n", *_th_id);
        ++cnt;
    }
    
    int *result = calloc(1, sizeof(int));
    *result = *_th_id * *_th_id;
    
    free(_th_id);
    _th_id = NULL;
    
    return (void *)result;
}

void thread_create(pthread_t *pthread, int th_id) {
    
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    int *_th_id = calloc(1, sizeof(int));
    *_th_id = th_id;
    int rc = pthread_create(pthread, &attr, thread_fn_callback, (void *)_th_id);
    
    if(rc != 0) {
        printf("Error occured, thread could not be created, errno = %d", rc);
    }
}

int main(int argc, char **argv) {
        
    pthread_t pthread1, pthread2;
    
    void *thread1_result, *thread2_result;
    
    thread_create(&pthread1, 200);
    thread_create(&pthread2, 100);
    
    pthread_join(pthread1, &thread1_result);
    pthread_join(pthread2, &thread2_result);
    
    free(thread1_result);
    free(thread2_result);
    thread1_result = NULL;
    thread2_result = NULL;
    
    return 0;
}

