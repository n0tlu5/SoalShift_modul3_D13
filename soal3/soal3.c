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

void* timer(void *arg){
    while(1){
        if(ssleep) ssleep--;
        if(wsleep) wsleep--;
        sleep(1);
    }
}

void* agmal(void *arg){
    while(1){
        if(!strcmp(buffer, "Agmal Ayo Bangun disabled 10 s") && sc==3){
            sc=0;
            stat=0;
            wsleep=10;
        }
        if(stat!=1 || wsleep)
            continue;
        else{
            wc++;
            Wake_status+=15;            
            if(wc==3){
                memset(buffer, 0, 1000);
                strcpy(buffer,"Fitur Iraj Ayo Tidur disabled 10 s");
                stat=3;
            }
            stat=0;
        }
    }
}

void* iraj(void *arg){
    while(1){
        if(!strcmp(buffer, "Fitur Iraj Ayo Tidur disabled 10 s") && wc==3){
            wc=0;
            stat=0;
            ssleep=10;
        }
        if(stat!=2 || ssleep)
            continue;
        else{
            sc++;
            Spirit_status-=10;
            if(sc==3){
                memset(buffer, 0, 1000);
                strcpy(buffer,"Agmal Ayo Bangun disabled 10 s");
                stat=3;
            }
            stat=0;
        }
        
    }
}

int main(int argc, char *argv[]){
    pthread_t tid;

    if(pthread_create(&tid, NULL, timer, NULL)<0){
        perror("creating thread");
        exit(1);
    }
    
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
            printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n", Wake_status, Spirit_status);
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