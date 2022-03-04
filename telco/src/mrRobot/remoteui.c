#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "remoteui.h"
#include "client.h"
#include "config.h"
#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


/*
 * Values initialization
 */
Remote *remote;
char characters[NB_CHAR] = {FORWARD_CHAR, BACKWARD_CHAR, LEFT_CHAR, RIGHT_CHAR, STOP_CHAR, DISPLAY_LOG_CHAR, CLEAR_LOG_CHAR, QUIT_CHAR}; // config.h


/* 
 * Private prototypes
 */
static void RemoteUI_run();
static void RemoteUI_display();
static bool_e RemoteUI_captureChoice();
static void RemoteUI_askMvt(Direction direction);
static VelocityVector RemoteUI_translate(Direction direction);
static void RemoteUI_ask4Log();
static void RemoteUI_askClearLog();
static void RemoteUI_eraseLog();
static void RemoteUI_quit();


/* 
 * Public functions
 */
void RemoteUI_new()
{
    if (!(remote = (Remote*) calloc(1, sizeof(Remote))))                // Allocate memory for the Remote structure
    {
        printf("ERROR NEW REMOTE : Remote memory not allocated.\n");    // If the program could not allocate memory, it print an error message
        exit(EXIT_SUCCESS);                                             // It exit 
    }
}

void RemoteUI_start()
{
    VelocityVector velocityVector;
    Client_start();
    Client_send_msg(ASK_PILOT_START, velocityVector);
    RemoteUI_run();
}

void RemoteUI_stop()
{
    printf("\n");               // Keeping last pilot state
    system("stty -cbreak");     // Ask the system to stop automatically pressing "enter" after each character typed. Now, the user must press it to validate his input
    system("stty echo");        // Ask the system to display character typed
}

void RemoteUI_free()
{
    if(remote)
        free(remote);
}


/* 
 * Private functions
 */
static void RemoteUI_run()
{
    RemoteState state = INIT;
    while(state != END)
    {
        switch(state)
        {
        case INIT:
            RemoteUI_display();
            state = WAIT_CHAR;
            break;
        case WAIT_CHAR:
            if(RemoteUI_captureChoice())
                state = RUNNING;
            break;
        case RUNNING:
            switch(remote->input)
            {
            case FORWARD_CHAR:
                RemoteUI_askMvt(FORWARD);
                break;
            case BACKWARD_CHAR:
                RemoteUI_askMvt(BACKWARD);
                break;
            case LEFT_CHAR:
                RemoteUI_askMvt(LEFT);
                break;
            case RIGHT_CHAR:
                RemoteUI_askMvt(RIGHT);
                break;
            case STOP_CHAR:
                RemoteUI_askMvt(NO);
                break;
            case DISPLAY_LOG_CHAR:
                RemoteUI_ask4Log();
                break;
            case CLEAR_LOG_CHAR:
                RemoteUI_askClearLog();
                break;
            case QUIT_CHAR:
                RemoteUI_quit();
                state = END;
                break;
            default:
                break;
            }
            if(state != END)
                state = WAIT_CHAR;
            break;
        default:
            break;
        }   
    }
}

static void RemoteUI_display()
{
    printf("Press :\n");
    printf("z : Forward\n");
    printf("s : Backward\n");
    printf("q : Left\n");
    printf("d : Right\n");
    printf("  : Stop\n");
    printf("r : Display log\n");
    printf("e : Clear log\n");
    printf("a : Quit\n");
    system("stty -echo");   // Ask the system to hide character typed
    system("stty cbreak");  // Ask the system to automatically press "enter" instead of the user after each character typed
}

static bool_e RemoteUI_captureChoice()
{
    if(remote)
        remote->input = getchar();   // The program suspends its operation, it waits for input from the keyboard

    for(int i = 0; i < NB_CHAR; i++)
    {
        if(remote->input == characters[i])
            return TRUE;
    }
    return FALSE;
}

static void RemoteUI_askMvt(Direction direction)
{
    VelocityVector velocityVector = RemoteUI_translate(direction);
    Client_send_msg(ASK_MVT, velocityVector);
}

static VelocityVector RemoteUI_translate(Direction direction)
{
    VelocityVector velocityVector = {direction, NOMINAL_POWER};
    if(direction == NO)
        velocityVector.power = DEFAULT_POWER;

    return velocityVector;
}

static void RemoteUI_ask4Log()
{
    VelocityVector velocityVector;
    Client_send_msg(ASK_PILOT_CHECK, velocityVector);
    Client_send_msg(ASK_PILOT_STATE, velocityVector);
    DataRecive dataRecive = Client_read_msg();
    PilotState pilotState = {dataRecive.speed, dataRecive.collision, dataRecive.luminosity};
    if(pilotState.collision == 0)
    {
        printf("\rCollision :");
        printf(GREEN);
        printf(" No ");
        printf(DEFAULT);
    }
    else
    {
        printf("\rCollision :");
        printf(RED);
        printf(" YES ");
        printf(DEFAULT);
    }
    printf("Luminosite : %f ", pilotState.luminosity);
    printf("Vitesse : %d ",pilotState.speed);
}

static void RemoteUI_askClearLog()
{
    RemoteUI_eraseLog();
}

static void RemoteUI_eraseLog()
{
    printf(ERASE);
}

static void RemoteUI_quit()
{
    VelocityVector velocityVector;
    Client_send_msg(ASK_PILOT_STOP, velocityVector);
    Client_stop();
}