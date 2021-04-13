#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>


void obtenerDatos(int ds){
    struct ifreq nic; //declaramos estructura
    unsigned char nombre[20];
    int s=0;
    printf("\nInserta el nombre de la interfaz:");
    gets(nombre);
    strcpy(nic.ifr_name,nombre);
    if(ioctl(ds,SIOCGIFINDEX,&nic)==-1){
        perror("\nError al obtener el index");
        exit(1);
    }
    else{
        printf("\nEl indice es: %d\n",nic.ifr_ifindex);
    }
    
    if(ioctl(ds,SIOCGIFADDR,&nic)==-1){
        perror("\nError al obtener la dirección IP");
        exit(1);
    }
    else{
        struct sockaddr_in* nicIpAddr = (struct sockaddr_in*)&nic.ifr_addr; //Casting de la direccion retornada en struct sockaddr a struct sockaddr_in, OJO se hace con pointer
        printf("\nLa direccion IP es: %s\n",inet_ntoa(nicIpAddr->sin_addr));
        //Se extrae el campo sin_addr tipo struct in_addr, de la estructura sockaddr_in
        
        //inet_ntoa convierte una ip en formato in_addr(formato binario en orden de bytes de red) a notacion estandar de números y puntos en una cadena.
    }
    
    if(ioctl(ds,SIOCGIFNETMASK,&nic)==-1){
        perror("\nError al obtener la mascara de subred");
    }
    else{
        struct sockaddr_in* nicNetMask = (struct sockaddr_in*)&nic.ifr_netmask;
        printf("\nLa direccion IP de la mascara de subred es:%s\n",inet_ntoa(nicNetMask->sin_addr));
    }
    if(ioctl(ds,SIOCGIFHWADDR,&nic)==-1){
        perror("\nError al obtener la MAC");
    }
    else{
        struct sockaddr_in* nicMAC = (struct sockaddr_in*)&nic.ifr_hwaddr;
        printf("\nLa MAC es:");
        //OJO como se debe imprimir la MAC
        for(s=0; s<6; s++){
            printf("%.2X ", (unsigned char)nic.ifr_hwaddr.sa_data[s]);
        }
        printf("\n");
    }
}
int main(){
    int packet_socket;
    packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    
    if(packet_socket==-1){
        perror("\nError al abrir el socket");
        exit(1);
    }
    else{
        perror("\nExito al abrir el socket");
        obtenerDatos(packet_socket);
    }
    close(packet_socket);
    return 0;
}
