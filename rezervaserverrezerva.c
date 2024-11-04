#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <stdint.h>

#define MAX_CONNECTIONS 32
#define MAXLINE 2000

struct tp{
    char*topicul;
    char sf;
};

struct topic
{
  char topic[51];
  char type[20];
  char continut[2000];
  struct sockaddr_in *cliaddr;
};

typedef struct celulag
{
  void *info;
  struct celulag *urm;
} TCelulaG, *TLG;

struct packet
{
  uint16_t len;
  char message[MAXLINE];
};

struct subscriber
{
  char id[100];
  int fd;
  TLG lista_topice;
  struct sockaddr_in cli_addr;
};

int ins_IncLG(TLG *aL, void *el)
{

  TLG aux = malloc(1 * sizeof(TCelulaG));
  aux->info = el;
  aux->urm = *aL;
  *aL = aux;
  return 1;
}
// functie de stergere subscriber din lista
int EliminareSub(TLG *aL, char *id)
{
  TLG aux = NULL;
  TLG p = *aL;
  for (; p != NULL; aux = p, p = p->urm)
  {
    struct subscriber *a = (struct subscriber *)(p->info);
    if (strcmp(a->id, id) == 0)
    {
      break;
    }
  }
  TLG b = *aL;
  if (aux == NULL)
  {
    *aL = b->urm;
  }
  else
  {
    aux->urm = p->urm;
  }
  return 1;
}

int recv_all1(int sockfd, struct packet *buffer)
{
  uint16_t catiprimit = 0;
  while (catiprimit != 2)
  {
    catiprimit = recv(sockfd, &(buffer->len), 2, 0);
  }

  uint16_t bytes_received = 0;
  uint16_t len = buffer->len;
  uint16_t bytes_remaining = buffer->len;
  char *buff = buffer->message;

  while (bytes_remaining)
  {
    bytes_received = recv(sockfd, buff + len - bytes_remaining, bytes_remaining, 0);
    if (bytes_received == -1)
    {
      fprintf(stderr, "recv() failed!\n");
    }

    if (bytes_received == 0)
    {
      return len - bytes_remaining;
    }
    bytes_remaining -= bytes_received;
  }

  return len - bytes_remaining;
}
int send_all1(int sockfd, struct packet *buffer)
{
  uint16_t catitrimisi = 0;
  while (catitrimisi != 2)
  {
    catitrimisi = send(sockfd, &(buffer->len), 2, 0);
  }

  uint16_t bytes_sent = 0;
  uint16_t len = buffer->len;
  uint16_t bytes_remaining = buffer->len;
  char *buff = buffer->message;

  while (bytes_remaining)
  {
    bytes_sent = send(sockfd, buff + len - bytes_remaining, bytes_remaining, 0);

    if (bytes_sent == -1)
    {
      fprintf(stderr, "send() failed!\n");
    }

    if (bytes_sent == 0)
    {
      return len - bytes_remaining;
    }
    bytes_remaining -= bytes_sent;
  }

  return len - bytes_remaining;
}

int Cuatre_Lista_Sub(TLG aL, struct subscriber *el)
{
  if (aL == NULL){
    return 1;
  }
  for (TLG p = aL; p != NULL; p = p->urm)
  {
    struct subscriber *aux = (struct subscriber *)(p->info);
    if (strcmp(aux->id, el->id) == 0)
    {
      return 0; // inseamna ca s-a gasit deja un subsciber cu acelasi id
    }
  }
  return 1;
}

void afis(TLG aL)
{
  TLG p = aL;
  while (p != NULL)
  {
    struct subscriber *aux = (struct subscriber *)(p->info);
    printf("%s ", aux->id);
    if (aux->lista_topice != NULL)
    {
      char *el = (char *)(aux->lista_topice->info);
      printf("%s ", el);
    }
    p = p->urm;
  }
  printf("\n");
}



int ins_offline(TLG *offline,TLG online,char*id){
    if(online == NULL){
        return 0;
    }

for (TLG p=online; p != NULL; p = p->urm)
  {
    struct subscriber *a = (struct subscriber *)(p->info);
    if (strcmp(a->id, id) == 0)
    {
        TLG aux =malloc(1*sizeof(TCelulaG));
        aux->info=p->info;
        aux->urm=*offline;
        *offline=aux;
    }
  }
  return 1;
}

struct topicnou{
    int de_cate_ori;
    struct topic* topic_trimis_offline;
    char idul[100];
};


int inserare_interesanta(TLG offline,struct topic* ola,TLG *lista_topic){
    TLG sub=offline;
    int sw=0;
    while((sub != NULL)&&(sw==0)){
        struct subscriber*asus=(struct subscriber*)(sub->info);
        TLG topicele=asus->lista_topice;
        while (topicele != NULL)
        {
            struct tp*intel=(struct tp*)(topicele->info);
            if(intel->sf == '1'){
                if(strcmp(ola->topic,intel->topicul)==0){
                
                    if(*lista_topic == NULL){
                        TLG aux=malloc(1*sizeof(TCelulaG));
                        struct topicnou*infor=malloc(1*sizeof(struct topicnou));
                        infor->topic_trimis_offline=ola;
                        infor->de_cate_ori=1;
                        strcpy(infor->idul,asus->id);
                        aux->info=infor;
                        aux->urm=*lista_topic;
                        *lista_topic=aux;

                    }
                    else{
                    int swsw=0;
                    TLG parcurgere=*lista_topic;
                    while(parcurgere != NULL){
                        struct topicnou*informatia=(struct topicnou*)(parcurgere->info);
                        if((strcmp(informatia->topic_trimis_offline->topic,ola->topic)==0)&&(strcmp(informatia->idul,asus->id)==0)){
                            swsw=1;
                            //printf("hahahaha\n");
                            informatia->de_cate_ori=informatia->de_cate_ori+1;
                            break;
                        }
                    parcurgere=parcurgere->urm;
                    }
                    if(swsw == 0){
                        TLG aux2=malloc(1*sizeof(TCelulaG));
                        struct topicnou*infor2=malloc(1*sizeof(struct topicnou));
                        infor2->topic_trimis_offline=ola;
                        infor2->de_cate_ori=1;
                        strcpy(infor2->idul,asus->id);
                        aux2->info=infor2;
                        aux2->urm=*lista_topic;
                        *lista_topic=aux2;
                    }
                    }
                }
            }
            topicele=topicele->urm;
        }
        sub=sub->urm;
    }
    return 1;

}


int main(int argc, char *argv[])
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  TLG lista_topic=NULL;          
  TLG lista_de_subscriberi=NULL; 
  TLG ofline_sub=NULL;

  // PORTUL PE CARE SE PRIMESTE CONEXIUNEA
  uint16_t port;
  int rc = sscanf(argv[1], "%hu", &port);

  struct packet *mesaj_primit;
  int lungime_mesaj;

  // SOCKETUL PENTRU STDIN
  // mai jos

  // SOCKETUL PENTRU TCP
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);

  if (listenfd < 0)
  {
    fprintf(stderr, "TCP conect port problem\n");
  }

  struct sockaddr_in serv_addr;
  socklen_t socket_len = sizeof(struct sockaddr_in);

  memset(&serv_addr, 0, socket_len);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


  rc = bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (rc < 0)
  {
    fprintf(stderr, "bind problem\n");
    return 0;
  }

  // SOCKETUL PENTRU UDP

  int sockUDP = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockUDP < 0)
  {
    fprintf(stderr, "socket UDP problem\n");
  }

  struct sockaddr_in servaddr, cliaddr;
  socklen_t socket_len_UDP = sizeof(struct sockaddr_in);

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  servaddr.sin_family = AF_INET;         // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY = 0.0.0.0
  servaddr.sin_port = htons(port);
  rc = bind(sockUDP, (const struct sockaddr *)&servaddr, sizeof(servaddr));

  socklen_t len_client = sizeof(cliaddr);

  // TREBUIE DOAR SA FAC CEVA CU CONEXIUNILE PRIMITE

  //struct pollfd poll_fds[MAX_CONNECTIONS];
  struct pollfd* poll_fds = (struct pollfd*) malloc(MAX_CONNECTIONS * sizeof(struct pollfd));
  int num_clients = 3;

  rc = listen(listenfd, MAX_CONNECTIONS);

  poll_fds[0].fd = listenfd;
  poll_fds[0].events = POLLIN;

  poll_fds[1].fd = sockUDP;
  poll_fds[1].events = POLLIN;

  // aici
  poll_fds[2].fd = STDIN_FILENO;
  poll_fds[2].events = POLLIN;

  struct sockaddr_in adresa_clientului;
  socklen_t lg_client = sizeof(adresa_clientului);
  memset(&adresa_clientului, 0, sizeof(adresa_clientului));

  while (1)
  {

    rc = poll(poll_fds, num_clients, -1);
    if (rc < 0)
    {
      fprintf(stderr, "poll problem\n");
    }
    for (int i = 0; i < num_clients; i++)
    {
      if (poll_fds[i].revents & POLLIN)
      {
        if (poll_fds[i].fd == listenfd)
        {
          // a venit o cerere de conexiune pe socketul inactiv (cel cu listen),
          // pe care serverul o accepta
          struct sockaddr_in cli_addr;
          socklen_t cli_len = sizeof(cli_addr);
          memset(&cli_addr, 0, sizeof(cli_addr));

          int newsockfd =
              accept(listenfd, (struct sockaddr *)&cli_addr, &cli_len);
          if (newsockfd < 0)
          {
            fprintf(stderr, "accept problem\n");
          }
          adresa_clientului = cli_addr;
          lg_client = cli_len;

          // se adauga noul socket intors de accept() la multimea descriptorilor
          // de citire
            if(num_clients == 32){
                struct pollfd* new_poll_fds = (struct pollfd*) realloc(poll_fds, (2*num_clients) * sizeof(struct pollfd));
                poll_fds=new_poll_fds;
            }
          poll_fds[num_clients].fd = newsockfd;
          poll_fds[num_clients].events = POLLIN;
          num_clients++;
        }
        else if (poll_fds[i].fd == sockUDP)
        {
          // s-au primit date pe unul din socketii de UDP,
          // asa ca serverul trebuie sa le receptioneze
          // mesaj_primit=calloc(1,sizeof(struct packet));
          // lungime_mesaj = recv_all1(poll_fds[i].fd, mesaj_primit,MAXLINE);
          // mesaj_primit->len=lungime_mesaj;
          // ins_IncLG(&lista_topic,mesaj_primit);
             mesaj_primit=calloc(1,sizeof(struct packet));
             mesaj_primit->len=recvfrom(sockUDP,mesaj_primit->message , MAXLINE,0, (struct sockaddr *) &cliaddr, &len_client);
             mesaj_primit->message[mesaj_primit->len]='\0';
             //printf("%s\n",mesaj_primit->message);
             struct topic *mama=malloc(1*sizeof(struct topic));
             mama->cliaddr=malloc(1*sizeof(struct sockaddr_in));
             mama->cliaddr->sin_addr.s_addr=cliaddr.sin_addr.s_addr;
             mama->cliaddr->sin_port=cliaddr.sin_port;
             memset(&cliaddr, 0, sizeof(cliaddr));
             strncpy(mama->topic,mesaj_primit->message,50);
             mama->topic[50]='\0';
             //mama->type=mesaj_primit->message[50];
             char tipul=mesaj_primit->message[50];


             if(mesaj_primit->message[50]==0){
               strcpy(mama->type,"INT");
                if(mesaj_primit->message[51]==0){
                  //strncpy(mama->continut,'+',1);
                  uint32_t flori=*((uint32_t*)(mesaj_primit->message + 52));
                  flori=ntohl(flori);
                  char h[100];
                  //memcpy(h,&flori,sizeof(flori));
                  //h[sizeof(flori)]='\0';
                  sprintf(h, "%" PRIu32, flori);
                  //printf("aici--------------%s",h);
                  strcpy(mama->continut,h);
                }else{
                  if(mesaj_primit->message[51]==1){
                  mama->continut[0]='-';
                  uint32_t flori=*((uint32_t*)(mesaj_primit->message + 52));
                  flori=ntohl(flori);
                  char h[100];
                  // memcpy(h,&flori,sizeof(flori));
                  // h[sizeof(flori)]='\0';
                  sprintf(h, "%" PRIu32, flori);
                  strcat(mama->continut,h);
                }
                }
              }else if(mesaj_primit->message[50]==3){
              strcpy(mama->type,"STRING");
              strncpy(mama->continut,mesaj_primit->message + 51,mesaj_primit->len - 51);
              mama->continut[mesaj_primit->len - 51]='\0';}
              else if(mesaj_primit->message[50]==1){
                strcpy(mama->type,"SHORT_REAL");
                
                  //strncpy(mama->continut,'+',1);
                  uint16_t flori=*((uint16_t*)(mesaj_primit->message + 51));
                  flori=ntohs(flori);
                  char h[1000];
                  sprintf(h,"%d.%02d",(int)(flori/100),(int)(flori % 100));
                  strcpy(mama->continut,h);
              }else if(mesaj_primit->message[50]==2){
                strcpy(mama->type,"FLOAT");
                if(mesaj_primit->message[51]==0){
                  //strncpy(mama->continut,'+',1);
                  uint32_t flori=*((uint32_t*)(mesaj_primit->message + 52));
                  flori=ntohl(flori);
                  //printf("%d\n",flori);
                  uint8_t power = *((uint8_t*)(mesaj_primit->message + 56));
                  //printf("%d\n",power);
                  char h[100];
                  char h1[100];
                  //uint32_t abs = flori/(uint32_t)pow(10,power);
                  //printf("%d\n",abs);
                  uint32_t pi=flori/(uint32_t)pow(10,power);
                  uint32_t pd=flori%(uint32_t)pow(10,power);
                  snprintf(h,sizeof(h),"%d",pi);
                  snprintf(h1,sizeof(h1),"%0*d",power,pd);
                  
                  //char h2[100];
                  if(power != 0)
                    snprintf(mama->continut,sizeof(mama->continut),"%s.%s",h,h1);
                    else{
                      snprintf(mama->continut,sizeof(mama->continut),"%s",h);
                    }

                }else{
                  if(mesaj_primit->message[51]==1){
                  //mama->continut[0]='-';
                  uint32_t flori=*((uint32_t*)(mesaj_primit->message + 52));
                  flori=ntohl(flori);
                  uint8_t power =*( (uint8_t*)(mesaj_primit->message + 56));
                  char h[100];
                  char h1[100];
                 
                  uint32_t pi=flori/(uint32_t)pow(10,power);
                  uint32_t pd=flori%(uint32_t)pow(10,power);
                  snprintf(h,sizeof(h),"%d",pi);
                  snprintf(h1,sizeof(h1),"%0*d",power,pd);
                  
                  //char h2[100];
                  if(power != 0){
                    snprintf(mama->continut,sizeof(mama->continut),"-%s.",h);
                    strcat(mama->continut,h1);
                    
                    }
                    else{
                      snprintf(mama->continut,sizeof(mama->continut),"-%s",h);
                    }
                }
                }}

           //ins_IncLG(&lista_topic,mama);
           inserare_interesanta(ofline_sub,mama,&lista_topic);

           TLG bulina=lista_de_subscriberi;
           if(lista_de_subscriberi != NULL){
           for(;bulina!=NULL;bulina=bulina->urm){
             struct subscriber*teo=(struct subscriber*)(bulina->info);
             char el[100];
             in_port_t portul=ntohs(mama->cliaddr->sin_port);
             sprintf(el,"%hu",portul);
             TLG geo = teo->lista_topice;
             if(teo->lista_topice != NULL){
             for(;geo != NULL;geo=geo->urm){
                 struct tp*carutas=(struct tp*)(geo->info);
               if(strncmp(carutas->topicul,mama->topic,strlen(mama->topic))==0){
                struct packet*cetransmit=malloc(1*sizeof(struct packet));
                strcpy(cetransmit->message,"UDP");
                strcat(cetransmit->message,"  ");
                strcat(cetransmit->message,inet_ntoa(mama->cliaddr->sin_addr));
                strcat(cetransmit->message,":");
                strcat(cetransmit->message,el);
                strcat(cetransmit->message," - ");
                strcat(cetransmit->message,mama->topic);
                strcat(cetransmit->message," - ");
                // int leno= 3+2+strlen(inet_ntoa(mama->cliaddr->sin_addr))+1+strlen(el)+3+strlen(mama->topic)+3;
                // cetransmit->message[leno]=mama->type;
                strcat(cetransmit->message,mama->type);
                strcat(cetransmit->message," - ");
                strcat(cetransmit->message,mama->continut);
                cetransmit->len=3+2+strlen(inet_ntoa(mama->cliaddr->sin_addr))+1+strlen(el)+3+strlen(mama->topic)+3+strlen(mama->type)+3+strlen(mama->continut);
                send_all1(teo->fd,cetransmit);
                //printf("%s\n",cetransmit->message);

               }
             }}
           }}
        }   //vezi ca trebuie practic sa trimit doua mesaje ca tu ai acel buffer de 15001 maxim sau mareste-l
        else if (poll_fds[i].fd == STDIN_FILENO)
        {
          // aici trebuie sa vada daca am primit de la tastatura date
          mesaj_primit = calloc(1, sizeof(struct packet));
          fgets(mesaj_primit->message, 5, stdin);
          for (int i = 3; i < num_clients; i++)
          {
            struct packet *ionut = malloc(1 * sizeof(struct packet));
            ionut->len = 8;
            strncpy(ionut->message, "Inchide", 7);
            send_all1(poll_fds[i].fd, ionut);
            close(poll_fds[i].fd);
          }
          close(poll_fds[0].fd);
          close(poll_fds[1].fd);
          return 0;
         
        }
        else
        {
          // aici am primit pe unul din clienti si trebuie sa receptionez
          struct packet *adina = malloc(1 * sizeof(struct packet));
          rc = recv_all1(poll_fds[i].fd, adina);
          char buf[1501];
          strncpy(buf, adina->message, adina->len);
          char *p = strtok(buf, "  ");
          while (p != NULL)
          {
            if (strncmp(p, "Init", 4) == 0)
            {
              struct subscriber *sub = malloc(1 * sizeof(struct subscriber));
              sub->cli_addr = adresa_clientului;
              p = strtok(NULL, "  ");
              strncpy(sub->id, p, strlen(p));
              sub->fd = poll_fds[i].fd;
              sub->lista_topice = NULL;
              if (Cuatre_Lista_Sub(lista_de_subscriberi, sub) == 0)
              {
                
                adina = malloc(1 * sizeof(struct packet));
                strncpy(adina->message, "Existi!", 7);
                adina->len = 8;
                send_all1(poll_fds[i].fd, adina);
                
                printf("Client %s already connected.\n", p);
                
                close(poll_fds[i].fd);

                // se scoate din multimea de citire socketul inchis
                for (int j = i; j < num_clients - 1; j++)
                {
                  poll_fds[j] = poll_fds[j + 1];
                }

                num_clients--;
                break;
              } else
              {
              char tata[100];
              sprintf(tata, "%hu", ntohs(adresa_clientului.sin_port));
              printf("New client %s connected from %s:%s.\n", sub->id, inet_ntoa(sub->cli_addr.sin_addr), tata);
              ins_IncLG(&lista_de_subscriberi, sub);
              if(ofline_sub != NULL){
              for(TLG date=ofline_sub;date != NULL;date=date->urm){
                    struct subscriber *informatie=(struct subscriber*)(date->info);
                    if(strcmp(sub->id,informatie->id)==0){
                        sub->lista_topice=informatie->lista_topice;
                        //isi si trimit ce avea pe 1 si eu am primit
                        TLG hohoho=sub->lista_topice;
                        while(hohoho != NULL){
                            struct tp*gica=(struct tp*)(hohoho->info);
                            if(gica->sf == '1'){
                            TLG gica2=lista_topic;
                            while(gica2 != NULL){
                                struct topicnou*cont_lista_mare=(struct topicnou*)(gica2->info);
                                if((strcmp(gica->topicul,cont_lista_mare->topic_trimis_offline->topic)==0)&&(strcmp(cont_lista_mare->idul,informatie->id)==0)){
                                    int de_cate_ori_sa_trimit=cont_lista_mare->de_cate_ori;
                                    //printf("%d\n",cont_lista_mare->de_cate_ori);
                                    while(de_cate_ori_sa_trimit != 0){
                                    struct packet*cetransmit=malloc(1*sizeof(struct packet));
                                    strcpy(cetransmit->message,"UDP");
                                    strcat(cetransmit->message,"  ");
                                    strcat(cetransmit->message,inet_ntoa(cont_lista_mare->topic_trimis_offline->cliaddr->sin_addr));
                                    strcat(cetransmit->message,":");
                                    char el[100];
                                    in_port_t portul=ntohs(cont_lista_mare->topic_trimis_offline->cliaddr->sin_port);
                                    sprintf(el,"%hu",portul);
                                    strcat(cetransmit->message,el);
                                    strcat(cetransmit->message," - ");
                                    strcat(cetransmit->message,cont_lista_mare->topic_trimis_offline->topic);
                                    strcat(cetransmit->message," - ");
                                    strcat(cetransmit->message,cont_lista_mare->topic_trimis_offline->type);
                                    strcat(cetransmit->message," - ");
                                    strcat(cetransmit->message,cont_lista_mare->topic_trimis_offline->continut);
                                    cetransmit->len=3+2+strlen(inet_ntoa(cont_lista_mare->topic_trimis_offline->cliaddr->sin_addr))+1+strlen(el)+3+strlen(cont_lista_mare->topic_trimis_offline->topic)+3+strlen(cont_lista_mare->topic_trimis_offline->type)+3+strlen(cont_lista_mare->topic_trimis_offline->continut);
                                    send_all1(sub->fd,cetransmit);
                                    //break;
                                    de_cate_ori_sa_trimit=de_cate_ori_sa_trimit-1;
                                }
                                cont_lista_mare->de_cate_ori=0;
                                }
                                gica2=gica2->urm;
                            }}
                            hohoho=hohoho->urm;
                        }


                        EliminareSub(&ofline_sub,informatie->id);
                        break;
                    }
              }

              }
            }
            }
            else if (strncmp(p, "Inchis", 6) == 0)
            {
              p = strtok(NULL, "  ");
              printf("Client %s disconnected.\n", p);
              ins_offline(&ofline_sub,lista_de_subscriberi,p);
              EliminareSub(&lista_de_subscriberi, p);

              close(poll_fds[i].fd);

              for (int j = i; j < num_clients - 1; j++)
              {
                poll_fds[j] = poll_fds[j + 1];
              }

              num_clients--;
            }
            else if (strcmp(p, "Sub") == 0)
            {
              
              p = strtok(NULL, "  ");
              TLG aux = lista_de_subscriberi;
              for (; aux != NULL; aux = aux->urm)
              {
                struct subscriber *ana = (struct subscriber *)(aux->info);
                if (strncmp(ana->id, p, strlen(p)) == 0)
                {
                  p = strtok(NULL, "  ");
                  struct tp*fata=malloc(1*sizeof(struct tp));
                
                  char *ajut = malloc(100 * 1);
                  memcpy(ajut, p, strlen(p) + 1);
                  fata->topicul=ajut;
                  p=strtok(NULL,"  ");
                  //printf("%s\n",p);
                  fata->sf=p[0];

                  if (ana->lista_topice == NULL)
                  {
                    TLG der = malloc(1 * sizeof(TCelulaG));
                    der->info = fata;
                    der->urm = NULL;
                    ana->lista_topice = der;
                  }
                  else if (ana->lista_topice != NULL)
                  {
                    TLG ber = malloc(1 * sizeof(TCelulaG));
                    ber->info = fata;
                    ber->urm = ana->lista_topice;
                    ana->lista_topice = ber;
                  }
                  break;
                }
              }
            }
            else if (strcmp(p, "Unsub") == 0)
            {
              p = strtok(NULL, "  ");
              TLG bulina = lista_de_subscriberi;
              for (; bulina != NULL; bulina = bulina->urm)
              {
                struct subscriber *teo = (struct subscriber *)(bulina->info);
                if (strncmp(teo->id, p, strlen(p)) == 0)
                {
                  p = strtok(NULL, "  ");
                  TLG sda = teo->lista_topice;
                  TLG sd = NULL;
                  for (; sda != NULL; sd = sda, sda = sda->urm)
                  {
                      struct tp*caruta=(struct tp*)(sda->info);
                    if (strncmp(caruta->topicul, p, strlen(p)) == 0)
                    {
                      break;
                    }
                  }
                  if (sd == NULL)
                  {
                    teo->lista_topice = sda->urm;
                  }
                  else
                  {
                    sd->urm = sda->urm;
                  }
                  break;
                }
              }
            }
            // for(TLG er=lista_de_subscriberi;er!=NULL;er=er->urm){
            //   struct subscriber*ana=(struct subscriber*)(er->info);
            //   printf("%s ",ana->id);
            //   TLG v=ana->lista_topice;
            //   while(v != NULL){
            //       struct tp*prolog=(struct tp*)(v->info);
            //     printf("%s---%c",prolog->topicul,prolog->sf);
            //     v=v->urm;
            //   }}

            // printf("\n");

            p = NULL;
          }
          // afis(lista_de_subscriberi);
        }
      }
    }
    // if(sw==1)
    // break;
  }
  // struct subscriber*ion=(struct subscriber*)lista_de_subscriberi->info;

  // printf("%s\n",ion->id);

  // run_chat_multi_server(listenfd);
  close(poll_fds[0].fd);
  close(poll_fds[1].fd);
  // close(poll_fds[2].fd);

  return 0;
}