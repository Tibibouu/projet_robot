#ifndef SERVER_H
#define SERVER_H


#include "pilot.h"
#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


typedef enum
{
    ASKED_PILOT_START,
    ASKED_MVT,
    ASKED_PILOT_CHECK,
    ASKED_PILOT_STATE,
    ASKED_PILOT_STOP
}Asked;

typedef struct Connected_s
{
    int number;
}Connected;

typedef struct DataSend_s
{
    int speed;
    int collision;
    float luminosity;
}DataSend;

typedef struct DataRecive_s
{
    Asked ask;
    Direction direction;
    int power;
}DataRecive;

typedef enum
{
    INIT,
    LISTEN,
    RUNNING,
    END
}ServerState;


/**
 * Start Server 
 *
 */
extern void Server_start();

/**
 * Stop Server
 *
 */
extern void Server_stop();


#endif /* SERVER_H */