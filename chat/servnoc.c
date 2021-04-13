#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>



int main(){
    /*descriptor de socket*/
    int udp_socket,lbind,tam,lrecv;
    
    /* estructura de la familia AF_INET */
    struct sockaddr_in SERVIDOR, CLIENTE;
    
    /*puedo usar 100  bytes, de los que uso 19 bytes*/
    unsigned char msj[512]="";
    unsigned char paqRec[512]="";
    
    /*el descriptor de socket udp_socket*/
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    /*como tcp es unico, protocolo 0*/
    
    /*si socket() regresa -1 => hubo un error*/
    if(udp_socket==-1){
        perror("\nError al abrir el socket");
        exit(1); /*termino el programa*/
    }
    
    else{
        perror("\nExito al abrir el socket");
        
        /*Hago la estructura de SERVIDOR*/
        SERVIDOR.sin_family=AF_INET; /* address family: AF_INET */
        SERVIDOR.sin_port=htons(8080);   /* pto especifico para CLIENTE */
        SERVIDOR.sin_addr.s_addr=INADDR_ANY;   /* 0.0.0.0 es INADDR_ANY */
        
        /*Hago bind entre el descriptor de socket y la estructura SERVIDOR*/
        lbind=bind(udp_socket, (struct sockaddr *)&SERVIDOR, sizeof(SERVIDOR));
        if(lbind==-1){
            perror("\nError en bind");
            exit(1);
        }
        else{
            perror("\nExito en bind");
            
            do{
            lrecv = sizeof(CLIENTE);
            tam=recvfrom(udp_socket,paqRec,512,0,(struct sockaddr *)&CLIENTE, &lrecv);
            
            if(tam==-1){
                perror("\nError al recibir");
                exit(1);
            }
            else{
            	if(strcmp(paqRec,"salir") == 0){
            	printf("\nSaliendo..");
            	close(udp_socket);
            	exit(0);
            	}
                printf("\nRespuesta: %s", paqRec);
                strcpy(paqRec,"");
                fflush( stdin );
                printf("\nEnvia:");
            	fgets(msj,30,stdin);
            	tam=sendto(udp_socket,msj,strlen(msj)+1,0,(struct sockaddr *)&CLIENTE, sizeof(CLIENTE));
            	strcpy(msj,"");
            	if(tam==-1){
            		perror("\nError al enviar");
            	}
            	else{
            		perror("\nExito al enviar");
            	}
            }
            }while(strcmp(paqRec,"salir") != 0);
        }
        
        
    }
    
    close(udp_socket);
    return 0;
}
