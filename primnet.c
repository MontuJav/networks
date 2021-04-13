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
    char opc;
    
    /* estructura de la familia AF_INET */
    struct sockaddr_in local, remota;
    
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
        
        /*Hago la estructura de local*/
        local.sin_family=AF_INET; /* address family: AF_INET */
        local.sin_port=htons(0);   /* 0 asigna valor aleatorio */
        local.sin_addr.s_addr=INADDR_ANY;   /* 0.0.0.0 es INADDR_ANY */
        
        /*Hago bind entre el descriptor de socket y la estructura local*/
        lbind=bind(udp_socket, (struct sockaddr *)&local, sizeof(local));
        if(lbind==-1){
            perror("\nError en bind");
            exit(1);
        }
        else{
            perror("\nExito en bind");
            /*Hago la estructura de remota*/
            remota.sin_family=AF_INET; /* address family: AF_INET */
            remota.sin_port=htons(8080);   /* los servers usan el pto 8080 */
            remota.sin_addr.s_addr=inet_addr("192.168.1.84");   /* inet_addr es una funcion que convierte una string a in_addr_t del manual inet_addr */
            
            
            do{
            fflush( stdin );
            printf("\nEnvia:");
            /* No usar scanf causa problemas */
            fgets(msj,30,stdin);
            tam=sendto(udp_socket,msj,strlen(msj)+1,0,(struct sockaddr *)&remota, sizeof(remota));
            strcpy(msj,"");
            if(tam==-1){
                perror("\nError al enviar");
            }
            else{
                perror("\nExito al enviar");
                lrecv = sizeof(remota);
		 tam=recvfrom(udp_socket,paqRec,512,0,(struct sockaddr *)&remota, &lrecv);
		 if(tam==-1){
		 	perror("\nError al recibir");
		        exit(1);
		 }
		 else{
		 	printf("\nRespuesta: %s", paqRec);
		 	strcpy(paqRec,"");
		 }
            }
            fflush( stdin );
            printf("\nSalir? (s/n):");
            scanf("%c%*c", &opc);
            }while(opc!='s');
        
            /*Señal de salida*/
            tam=sendto(udp_socket,"salir",strlen("salir")+1,0,(struct sockaddr *)&remota, sizeof(remota));
            
            if(tam==-1){
		 perror("\nError al eviar senal de salida");
		 exit(1);
	    }
	    else{
		 printf("\nSe envio el mensaje de salida");
	    }    
        }  
    }
    
    close(udp_socket);
    return 0;   
}
