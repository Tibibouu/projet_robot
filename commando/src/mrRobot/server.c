#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "server.h"
#include "pilot.h"
#include "config.h"

#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


/*
 * Values initialization
 */
int socket_listen, socket_instance;
struct sockaddr_in server_adress;
Connected connectedDevice;


/* 
 * Private prototypes
 */
static void Server_connect();
static void Server_setConnection();
static void Server_readMsg();
static void Server_sendMsg(PilotState pilotState);


/* 
 * Public functions
 */
void Server_start()
{
    connectedDevice.number = 0;
    Server_connect();
    Server_setConnection();
    Server_readMsg();
}

void Server_stop()
{
    if(connectedDevice.number)
    {
        close(socket_instance);
        close(socket_listen);
    }
}


/* 
 * Private functions
 */
static void Server_connect()
{
    /* Creation du socket : AF_INET = IP, SOCK_STREAM = TCP */
    socket_listen = socket (AF_INET, SOCK_STREAM, 0);
    server_adress.sin_family = AF_INET; /* Type d'adresse = IP */
    server_adress.sin_port = htons (SERVER_PORT); /* Port TCP ou le service est accessible */
    server_adress.sin_addr.s_addr = htonl (INADDR_ANY); /* On s'attache a toutes les interfaces */
    /* On attache le socket a l'adresse indiquee */
    bind (socket_listen, (struct sockaddr *)&server_adress, sizeof (server_adress));
    /* Mise en ecoute du socket */
    listen (socket_listen, MAX_PENDING_CONNECTIONS);

    /* Acceptation de la connexion*/
    socket_instance = accept(socket_listen, NULL, 0);
}

static void Server_setConnection()
{
    if(socket_instance != -1)
        connectedDevice.number++;
}

static void Server_sendMsg(PilotState pilotState)
{
    DataSend dataSend;
    dataSend.speed = htonl (pilotState.speed);
    dataSend.collision = htonl (pilotState.collision);
    dataSend.luminosity = htonl (pilotState.luminosity);
    write(socket_instance, &dataSend, sizeof(dataSend));
}

static void Server_readMsg()
{
    ServerState state = INIT;
    DataRecive dataRecive;
    VelocityVector velocityVector;
    PilotState pilotState;
    while(state != END)
    {
        switch(state)
        {
        case INIT:
            state = LISTEN;
            break;
        case LISTEN:
            if(read(socket_instance, &dataRecive, sizeof(dataRecive)))
            {
                dataRecive.ask = ntohl (dataRecive.ask);
                velocityVector.direction = ntohl (dataRecive.direction);
                velocityVector.power = ntohl (dataRecive.power);
                state = RUNNING;
            }
            break;
        case RUNNING:
            switch (dataRecive.ask)
            {
            case ASKED_MVT:
                Pilot_setVelocity(velocityVector);
                break;
            case ASKED_PILOT_START:
                Pilot_start();
                break;
            case ASKED_PILOT_CHECK:
                Pilot_check();
                break;
            case ASKED_PILOT_STATE:
                pilotState = Pilot_getState();
                Server_sendMsg(pilotState);
                break;
            case ASKED_PILOT_STOP:
                Pilot_stop();
                state = END;
                break;
            default:
                break;
            }
            if(state != END)
                state = LISTEN;
            break;
        default:
            break;
        }
    }
}