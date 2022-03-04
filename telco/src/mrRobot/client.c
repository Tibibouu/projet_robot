#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "client.h"
#include "config.h"

#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


/*
 * Values initialization
 */
int socket_instance;
struct sockaddr_in server_adress;
ConnectionState connectionState = DISCONNECTED;


/* 
 * Private prototypes
 */
static int Client_connect();
static void Client_setConnectionState(int request);


/* 
 * Public functions
 */
void Client_start()
{
    int request;
    printf("Waiting connection..\n");
    while(!connectionState)
    {
        request = Client_connect();
        Client_setConnectionState(request);
    }
    printf("Connect with commando !\n\n");
}

void Client_stop()
{
    if(connectionState)
        close(socket_instance);
}

void Client_send_msg(Ask ask, VelocityVector velocityVector)
{
    DataSend dataSend;
    dataSend.ask = htonl (ask);
    dataSend.direction = htonl (velocityVector.direction);
    dataSend.power = htonl (velocityVector.power);
    write(socket_instance, &dataSend, sizeof(dataSend));
}

DataRecive Client_read_msg()
{
    DataRecive dataRecive;
    bool_e valide = FALSE;
    while(!valide)
    {
        if(read(socket_instance, &dataRecive, sizeof(dataRecive)))
            return dataRecive;
    }
    return (DataRecive){0, 0, 0.0};
}


/* 
 * Private functions
 */
static int Client_connect()
{
    /* On choisit un socket TCP (SOCK_STREAM) sur IP (AF_INET) */
    socket_instance = socket(AF_INET, SOCK_STREAM, 0);
    /* AF_INET = famille TCP/IP */
    server_adress.sin_family = AF_INET;
    /* port du serveur auquel se connecter */
    server_adress.sin_port = htons(SERVER_PORT);
    /* adresse IP (ou nom de domaine) du serveur auquel se connecter */
    server_adress.sin_addr = *((struct in_addr *)gethostbyname(SERVER_IP)->h_addr_list[0]);
    /* On demande la connexion aupres du serveur */
    return connect(socket_instance, (struct sockaddr *)&server_adress, sizeof(server_adress));
}

static void Client_setConnectionState(int request)
{
    if(!request)
        connectionState = CONNECTED;
    else
        connectionState = DISCONNECTED;
}