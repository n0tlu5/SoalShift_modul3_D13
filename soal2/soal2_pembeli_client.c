#include <stdio.h>
#include <unistd.h>
#include<string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 8081

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    memset(buffer, 0, 1000);
    valread = read( sock , buffer, 1024);
    if(!strcmp(buffer,"Refused")){
        puts(buffer);
        return 0;
    }else{
        puts(buffer);
    }

    while(1){
        printf("Inpute message :");
        scanf("%s", buffer);
        send(sock , buffer, strlen(buffer), 0);
        if(!strcmp(buffer, "close")){
            return 0;
        }
        memset(buffer, 0, 1000);
        valread = read( sock , buffer, 1024);
        puts(buffer);
        memset(buffer, 0, 1000);
    }    
    return 0;
}