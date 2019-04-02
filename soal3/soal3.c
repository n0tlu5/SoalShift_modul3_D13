#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

char buffer[1024]="";
int stat=0;
int Wake_status=0;
int Spirit_status=100;
int wc=0;
int wsleep=0;
int ssleep=0;
int sc=0;

void* agmal(void *arg){
    while(1){
        if(stat!=1) continue;
        if(!strcmp(buffer, "Agmal Ayo Bangun disabled 10 s")){
            stat=0;
            wsleep=1;
            sleep(10);
            wsleep=0;
        }else if(stat==1){
            wc++;
            Wake_status+=15;
            memset(buffer, 0, 1000);
            if(wc==3){
                wc=0;
                stat=2;
                strcpy(buffer,"Fitur Iraj Ayo Tidur disabled 10 s");
            }else{
                stat=0;
            }
        }
    }
}

void* iraj(void *arg){
    while(1){
        if(stat!=2) continue;
        if(!strcmp(buffer, "Fitur Iraj Ayo Tidur disabled 10 s")){
            stat=0;
            ssleep=1;
            sleep(10);
            ssleep=0;
        }else if(stat==2){
            sc++;
            Spirit_status-=10;
            memset(buffer, 0, 1000);
            if(sc==3){
                sc=0;
                stat=1;
                memset(buffer, 0, 1000);
                strcpy(buffer,"Agmal Ayo Bangun disabled 10 s");
            }else{
                stat=0;
            }
        }
    }
}

int main(int argc, char *argv[]){
    pthread_t tid;
    
    if(pthread_create(&tid, NULL, agmal, NULL)<0){
        perror("creating thread");
        exit(1);
    }

    if(pthread_create(&tid, NULL, iraj, NULL)<0){
        perror("creating thread");
        exit(1);
    }
    
    while(Wake_status<100 && Spirit_status>0){
        if(stat!=0) continue;
        memset(buffer, 0, 1000);
        gets(buffer);
        if(!strcmp(buffer, "All Status")){
            printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n%d\n%d\n", Wake_status, Spirit_status,wc,sc);
        }else if(!strcmp(buffer, "Agmal Ayo Bangun") & !wsleep){
            stat=1;
        }else if(!strcmp(buffer, "Iraj Ayo Tidur" ) & !ssleep){
            stat=2;
        }
    }

    if(Wake_status>=100){
        puts("Agmal Terbangun,mereka bangun pagi dan berolahraga");
    }else{
        puts("Iraj ikut tidur, dan bangun kesiangan bersama Agmal");
    }
    return 0;
}