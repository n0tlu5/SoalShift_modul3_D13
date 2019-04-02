#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>

char monstername[50];
int *value;
int health=300;
int hunger=200;
int hygine=100;
int curscene=0;
int ehealth=100;
int myfood=0;
int bathcd=0;

void changemode(int);
int  kbhit(void);
void* display(void *arg);
void* meta1(void *arg);
void* meta2(void *arg);
void* regen(void *arg);

int main(){
    pthread_t thread;
    char ch;

    /* shared mem */
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    /* END shared mem */

    puts("insert monstername :");
    gets(monstername);
    
    if(pthread_create(&thread, NULL, display, NULL)<0){
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&thread, NULL, regen, NULL)<0){
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&thread, NULL, meta1, NULL)<0){
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&thread, NULL, meta2, NULL)<0){
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }

    changemode(1);
    while(1){
        if(hunger<=0){
            printf("%s died starving\n", monstername);
        }
        if(hygine<=0){
            printf("%s died due to illness\n", monstername);
        }
        while(!kbhit());
        ch = getchar();
        if(curscene==0){
            if(ch=='1'){
                if(myfood){
                    hunger+=15;
                    myfood--;
                }else{
                    puts("insufficient food");
                }
            }else if(ch=='2'){
                if(bathcd<=0){
                    bathcd=20;
                    hygine+=30;
                }else{
                    puts("bath is in cooldown");
                }
            }else if(ch=='3'){
                curscene=1;
            }else if(ch=='4'){
                curscene=2;
            }else if(ch=='5'){
                exit(0);
            }
        }else if(curscene==1){
            if(ch=='1'){
                health-=20;
                ehealth-=20;
                if(health<=0){
                    printf("%s died in the battle\n", monstername);
                    exit(0);
                }else if(ehealth<=0){
                    printf("%s won the battle\n", monstername);
                    sleep(0.5);
                    curscene=0;
                }
            }else if(ch=='2'){
                curscene=0;
            }
        }else if(curscene==2){
            if(ch=='1'){
                if(*value>0){
                    *value = *value - 1;
                    myfood++;
                }else{
                    puts("shop's food is out of stock");
                }
            }else if(ch=='2'){
                curscene=0;
            }            
        }
    }
}

void* display(void *arg){
    while(1){
        if(curscene==0){
            printf("Standby Mode\nHealth : %d\nHunger : %d\nHygiene : %d\nFood left : %d\n", health, hunger, hygine, myfood);
            if(bathcd<=0){
                puts("Bath is ready");
            }else{
                printf("Bath will be ready in %ds\n", bathcd);
            }
            printf("Choices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");
        }else if(curscene==1){
            printf("Battle Mode\n%s’s Health : %d\nEnemy’s Health : %d\nChoices\n1. Attack\n2. Run\n", monstername, health, ehealth);
        }else if(curscene==2){
            printf("Shop Mode\nShop food stock : %d\nYour food stock : %d\nChoices\n1. Buy\n2. Back\n", *value, myfood);
        }
        bathcd--;
        sleep(1);
        system("clear");
    }
}

void* meta1(void *arg){
    while(1){
        if(curscene==1) continue;
        hygine-=10;
        sleep(30);
    }
}

void* meta2(void *arg){
    while(1){
        if(curscene==1) continue;
        hunger-=5;
        sleep(10);
    }
}

void* regen(void *arg){
    while(1){
        if(curscene==0){
            health+=5;
            sleep(10);
        }
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