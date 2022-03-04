#ifndef CLIENT_H
#define CLIENT_H


#include "remoteui.h"
#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


typedef enum
{
    DISCONNECTED = 0,
    CONNECTED = 1
}ConnectionState;

typedef struct DataSend_s
{
    Ask ask;
    Direction direction;
    int power;
}DataSend;

typedef struct DataRecive_s
{
    int speed;
    int collision;
    float luminosity;
}DataRecive;


/**
 * Start Client 
 *
 */
extern void Client_start();

/**
 * Stop Client
 *
 */
extern void Client_stop();

/**
 * Start Client 
 *
 */
extern void Client_send_msg(Ask ask, VelocityVector velocityVector);

/**
 * Start Client 
 *
 */
extern DataRecive Client_read_msg();


#endif /* CLIENT_H */