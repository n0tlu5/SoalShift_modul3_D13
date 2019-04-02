#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 8080

int *value;
int total_connections=0;

void* display(void *arg){
    while(1){
        printf("stock : %d\n", *value);
        sleep(5);
    }
}

void* connection_handler(void *socket_desc){
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        // proses client message
		if(!strcmp(client_message, "tambah")){
            *value = *value + 1;
        }else if(!strcmp(client_message, "close")){
            read_size=0;
            break;
        }
		
		//clear the message buffer
		memset(client_message, 0, 2000);
    }

    total_connections-=1;
     
    if(read_size == 0){
        puts("Client disconnected");
        fflush(stdout);
    }else if(read_size == -1){
        perror("recv failed");
    }

    close(sock);
         
    return 0;
}

int main(){
    pthread_t thread;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    /* shared mem */
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    /* END shared mem */

    /* display thread */
    if(pthread_create(&thread, NULL, display, NULL)<0){
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }
    /* END display thread */

    /* create, bind, and listen */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    /* END create, bind, and listen */

    while(new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) {
        memset(buffer, 0, 1000);
        if(total_connections<1){
            strcpy(buffer, "Accepted");
            puts("New connection accepted");
            if( pthread_create(&thread , NULL ,  connection_handler , (void*) &new_socket) < 0){
                perror("could not create thread");
                return 1;
            }
            puts("Handler assigned");
            write(new_socket , buffer , strlen(buffer));
            total_connections+=1;
        }else{
            strcpy(buffer, "Refused");
            puts("New connection refused");
            write(new_socket , buffer , strlen(buffer));
            close(new_socket);
        }
    }

    if (new_socket < 0){
        perror("accept failed");
        return 1;
    }

    return 0;
}