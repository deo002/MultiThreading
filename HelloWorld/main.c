//
//  main.c
//  MultiThreading
//
//  Created by Dearsh Oberoi on 21/03/25.
//

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include<stdlib.h>

/* A static function is a function which will be visibale only to the file it is defined in */
static void* thread_callback_fn(void *arg) {
    char *str = (char *)arg;
   
    while(1) {
        printf("%s", str);
        sleep(1);
    }
}

void thread1_create(void) {
    pthread_t pthread;
    /*
     Always pass global or heap variables to a thread.
     The local variables go out of scope once the function terminates.
     The thread might still be doing it's work even if the function terminates.
     If the thread accesses this local variable once it(variable) goes out of
     scope(func terminates), there will be a segmentation fault.
     
     A static variable is a global variable.
    */
    static char* str = "Hello world!";
    int rc  = pthread_create(&pthread, NULL, thread_callback_fn, (void *)str);
    
    if(rc != 0) {
        perror("Error occured, thread could not be created, %s");
        exit(0);
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    thread1_create();
    printf("pausing main function\n");
    /*
     A thread always exixts corresponding to a process.
     If the process terminates before the thread does it's work,
     the thread terminates too.
    */
//    pause();
    
    /*
     This only terminates the main thread, but not the process. So, the other child threads
     execute without problem.
    */
    pthread_exit(0);
    
    return 0;
}

