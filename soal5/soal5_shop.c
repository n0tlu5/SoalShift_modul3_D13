#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>

int *value;

void* display(void *arg);
void changemode(int);
int  kbhit(void);

int main(){
    pthread_t thread;
    char ch;

    /* shared mem */
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    /* END shared mem */

    if(pthread_create(&thread, NULL, display, NULL)<0){
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }

    changemode(1);
    while(1){
        while(!kbhit());
        ch = getchar();
        if(ch=='1'){
            *value = *value + 1;            
        }else if(ch=='2'){
            exit(0);
        }   
    }

    return 0;
}

void* display(void *arg){
    while(1){
        printf("Shop\nFood stock : %d\nChoices\n1. Restock\n2. Back\n", *value);
        sleep(1);
        system("clear");
    }
}

void changemode(int dir){
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 
int kbhit (void){
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
 
}