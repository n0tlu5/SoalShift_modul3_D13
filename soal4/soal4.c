#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int status;

void* stage1_1(void *arg){
    system("ps aux | head -n 11 | tail -n 10 > /home/mungkin/Documents/FolderProses1/SimpanProses1.txt");
    system("zip -j /home/mungkin/Documents/FolderProses1/KompresProses1.zip /home/mungkin/Documents/FolderProses1/SimpanProses1.txt");
    remove("/home/mungkin/Documents/FolderProses1/SimpanProses1.txt");
    status--;
}

void* stage1_2(void *arg){
    system("ps aux | head -n 11 | tail -n 10 > /home/mungkin/Documents/FolderProses2/SimpanProses2.txt");
    system("zip -j /home/mungkin/Documents/FolderProses2/KompresProses2.zip /home/mungkin/Documents/FolderProses2/SimpanProses2.txt");
    remove("/home/mungkin/Documents/FolderProses2/SimpanProses2.txt");
    status--;
}

void* stage2_1(void *arg){
    system("unzip /home/mungkin/Documents/FolderProses1/KompresProses1.zip -d /home/mungkin/Documents/FolderProses1/");
    status--;
}

void* stage2_2(void *arg){
    system("unzip /home/mungkin/Documents/FolderProses2/KompresProses2.zip -d /home/mungkin/Documents/FolderProses2/");
    status--;
}

int main(){
    pthread_t tid[2];
    status=2;
    if( pthread_create(&(tid[0]) , NULL ,  stage1_1 , NULL) < 0){
        perror("could not create thread");
        return 1;
    }

    if( pthread_create(&(tid[1]) , NULL ,  stage1_2 , NULL) < 0){
        perror("could not create thread");
        return 1;
    }

    while(status);

    puts("Menunggu 15 detik untuk mengekstrak kembali");
    sleep(15);

    status=2;
    if( pthread_create(&(tid[0]) , NULL ,  stage2_1 , NULL) < 0){
        perror("could not create thread");
        return 1;
    }

    if( pthread_create(&(tid[1]) , NULL ,  stage2_2 , NULL) < 0){
        perror("could not create thread");
        return 1;
    }
    
    while(status);

    return 0;
}