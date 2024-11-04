#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CONNECTIONS 32
#define MAXLINE 1552



struct packet {
  uint16_t len;
  char message[2000];
};

int recv_all1(int sockfd, struct packet *buffer) {
    uint16_t catiprimit=0;
    while(catiprimit != 2 ){
        catiprimit=recv(sockfd,&(buffer->len),2,0);
    }

  uint16_t bytes_received = 0;
  uint16_t len = buffer->len;
  uint16_t bytes_remaining = buffer->len;
  char *buff = buffer->message;


  while (bytes_remaining) {
      bytes_received = recv(sockfd, buff + len - bytes_remaining, bytes_remaining, 0);
      if(bytes_received == -1){
        fprintf(stderr,"recv() failed!\n");
      }

      if (bytes_received == 0) {
          return len - bytes_remaining;
      }
      bytes_remaining -= bytes_received;
  }

  return len - bytes_remaining;
}
int send_all1(int sockfd, struct packet *buffer) {
    uint16_t catitrimisi=0;
    while (catitrimisi != 2)
    {
        catitrimisi=send(sockfd,&(buffer->len),2,0);
    }
    
  uint16_t bytes_sent = 0;
  uint16_t len = buffer->len;
  uint16_t bytes_remaining = buffer->len;
  char *buff = buffer->message;

  while (bytes_remaining) {
      bytes_sent = send(sockfd, buff + len - bytes_remaining, bytes_remaining, 0);
      if(bytes_sent == -1){
        fprintf(stderr,"send() failed!\n");
      }

      if (bytes_sent == 0) {
          return len - bytes_remaining;
      }
      bytes_remaining -= bytes_sent;
  }
  
  return len - bytes_remaining;
}



int main(int argc, char *argv[]){

    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    struct packet *mesaj_primit;
    int lungime_mesaj;
    

    uint16_t port;
    int rc = sscanf(argv[3], "%hu", &port);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    

    struct sockaddr_in server_addr;
  

   
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(argv[2]);

     if(connect(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("[CLIENT] Unable to connect\n");
        return -1;
    }

    //zi-i serverului cine esti
    struct packet*pachet=malloc(1*sizeof(struct packet));
    uint16_t lungime=strlen(argv[1])+3+4;
    pachet->len=lungime;
    strcpy(pachet->message,"Init");
    strcat(pachet->message,"  ");
    strcat(pachet->message,argv[1]);
    
    send_all1(listenfd,pachet);


    struct pollfd poll_fds[MAX_CONNECTIONS];
    int num_clients = 2;

    poll_fds[0].fd=STDIN_FILENO;
    poll_fds[0].events=POLLIN;
    poll_fds[1].fd=listenfd;
    poll_fds[1].events=POLLIN;
    int sw=0;
    while (1)
    {
        rc = poll(poll_fds, num_clients, -1);

        for(int i=0;i<num_clients;i++){
            if((poll_fds[i].revents & POLLIN) != 0){
                if(poll_fds[i].fd == STDIN_FILENO){

                    mesaj_primit=calloc(1,sizeof(struct packet));
                    fgets(mesaj_primit->message,1000,stdin);
                    lungime_mesaj=strlen(mesaj_primit->message);
                    mesaj_primit->len=lungime_mesaj;
                    char sir1[5]="exit";
                    sir1[5]='\0';
                    mesaj_primit->message[mesaj_primit->len-1]='\0';
                    if(strcmp(mesaj_primit->message,sir1)==0){
                        pachet=malloc(1*sizeof(struct packet));
                        lungime=strlen(argv[1])+3+6;
                        pachet->len=lungime;
                        strcpy(pachet->message,"Inchis");
                        strcat(pachet->message,"  ");
                        strcat(pachet->message,argv[1]);
                        send_all1(listenfd,pachet);
                        close(listenfd);
                        return 0;
                        
                    }else if(strncmp(mesaj_primit->message,"subscribe",9)==0){
                        struct packet*mamatata=malloc(1*sizeof(struct packet));
                        mamatata->len=3+strlen(argv[1])+7;
                        strcpy(mamatata->message,"Sub");
                        strcat(mamatata->message,"  ");
                        strcat(mamatata->message,argv[1]);
                        char topic[100];
                        char *p=strtok(mesaj_primit->message," ");
                        p=strtok(NULL," ");
                        strcat(mamatata->message,"  ");
                        strcat(mamatata->message,p);
                        strcpy(topic,p);
                        mamatata->len=mamatata->len+strlen(p);
                        p=strtok(NULL," ");
                        strcat(mamatata->message,"  ");
                        strcat(mamatata->message,p);
                        mamatata->len=mamatata->len+strlen(p);
                        send_all1(listenfd,mamatata);
                        
                        printf("Subscribed to topic.\n");

                    }else if(strncmp(mesaj_primit->message,"unsubscribe",11)==0){
                        struct packet * terog=malloc(1*sizeof(struct packet));
                        terog->len=5+strlen(argv[1])+7;
                        strcpy(terog->message,"Unsub");
                        strcat(terog->message,"  ");
                        strcat(terog->message,argv[1]);
                        char topic[100];
                        char *p=strtok(mesaj_primit->message," ");
                        p=strtok(NULL," ");
                        strcat(terog->message,"  ");
                        strcat(terog->message,p);
                        strncpy(topic,p,strlen(p));
                        terog->len=terog->len+strlen(p);
                        
                        send_all1(listenfd,terog);
                        
                        printf("Unsubscribed to topic.\n");
                    }

                }else if(poll_fds[i].fd==listenfd){
                    struct packet*mamatata=malloc(1*sizeof(struct packet));
                    mamatata->len=0;
                    
                    int rc = recv_all1(listenfd,mamatata);
                    
                    if(strcmp(mamatata->message,"Existi!")==0){
                        
                        close(listenfd);
                        return 0 ;
                    }else if(strncmp(mamatata->message,"Inchide",7)==0){
                        close(listenfd);
                        return 0;
                    }else if(strncmp(mamatata->message,"UDP",3)==0){
                        printf("%s\n",mamatata->message + 5);
                        //printf("%s\n",mamatata->message + 5);

                    }
                    
                }
            }
        }
        if(sw==1)
            break;
    }
    


    close(listenfd);
    return 0;
}