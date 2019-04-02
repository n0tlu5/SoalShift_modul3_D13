#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int facttable[1000];
int counttable[1000];
int status;

int myfact(int n){
    if(facttable[n]||n==0) return facttable[n];
    facttable[n]=n*myfact(n-1);
    return facttable[n];
}
 
void* fact(void *arg){
    int n = (int)*(int *) arg;
    status=0;
    facttable[n]=myfact(n);
}

int main(int argc, char *argv[]){
    if(argc == 1){
        puts("program needs integer argumen(s)");
        exit(0);
    }

    pthread_t tid[100];
    int err;
    int i;
    facttable[0]=1;

    for(i=1;i<argc;i++){
        int n = atoi(argv[i]);
        int *ptrn = &n;
        counttable[n]++;
        if(!facttable[n]){
            status=1;
            err = pthread_create(&(tid[i]), NULL, fact, (void *) ptrn); //membuat thread pertama
            if(err){
                fprintf(stderr,"Error - pthread_create() return code: %d\n",err);
                exit(EXIT_FAILURE);
            }
            while(status);
        }
    }
    
    for(i=1;i<argc;i++){
        pthread_join(tid[i], NULL);
    }

    for(i=0;i<1000;i++){
        if(counttable[i]){
            while(counttable[i]--){
                printf("%d! = %d\n", i, facttable[i]);
            }
        }
    }
}