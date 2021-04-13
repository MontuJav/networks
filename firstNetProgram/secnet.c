#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(){
    
    int udp_socket,lbind,tam;
    struct sockaddr_in remota,local;
    unsigned char msj[100]="hola red soy Javie";
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(udp_socket==-1){
        perror("\nError al abrir el socket");
        exit(1); /*termino el programa*/
    }
    
    else{
        perror("\nExito al abrir el socket");
        local.sin_family=AF_INET;
        local.sin_port=htons(0); //puerto en 0 cuando queremos que la API asigne el pto.
        local.sin_addr.s_addr=INADDR_ANY;
        lbind=bind(udp_socket, (struct sockaddr *)&local, sizeof(local));
        if(lbind==-1){
            perror("\nError en bind");
            exit(1);
        }
        else{
            perror("\nExito en bind");
            remota.sin_family=AF_INET;
            remota.sin_port=htons(53);
            remota.sin_addr.s_addr=inet_addr("8.8.8.8");
            
            tam=sendto(udp_socket,msj,strlen(msj)+1,0,(struct sockaddr *)&remota, sizeof(remota));
            if(tam==-1){
                perror("\nError al enviar");
                exit(1);
            }
            else{
                perror("\nExito al enviar");
            }
        }  
    }
    
    close(udp_socket);
    return 0;
     
}
