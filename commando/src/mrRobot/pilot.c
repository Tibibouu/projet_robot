#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "pilot.h"
#include "robot.h"
#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


Pilot *pilot;


/* 
 * Private prototypes
 */
static void Pilot_run(event_e event, VelocityVector velocityVector);
static void Pilot_sendMvt(VelocityVector velocityVector);
static bool_e Pilot_hasBumped();


/* 
 * Public functions
 */
void Pilot_new()
{
    if (!(pilot = (Pilot*) calloc(1, sizeof(Pilot))))               // Allocate memory for the Pilot structure
    {
        printf("ERROR NEW PILOT : Pilot memory not allocated.\n");  // If the program could not allocate memory, it print an error message
        exit(EXIT_SUCCESS);                                         // It exit 
    }
}

void Pilot_start()
{
    Pilot_new();
    Robot_start();
}

void Pilot_stop()
{
    VelocityVector velocityVector = {NO,0};
    Pilot_run(SET_VELOCITY, velocityVector);

    Pilot_free();
    Robot_stop();
}

void Pilot_free()
{
    if(pilot)
        free(pilot);
}

void Pilot_setVelocity(VelocityVector velocity)
{
    Pilot_run(SET_VELOCITY, velocity);
}

PilotState Pilot_getState()
{
    SensorState sensorState = Robot_getSensorState();
    pilot->pilot_state.luminosity = sensorState.luminosity;
    pilot->pilot_state.speed = Robot_getRobotSpeed();
    return pilot->pilot_state;
}

void Pilot_check()
{
    VelocityVector velocityVector = {NO,0};
    Pilot_run(CHECK, velocityVector);
}


/* 
 * Private functions
 */
static void Pilot_run(event_e event, VelocityVector velocityVector)
{
    switch(event)
    {
    case SET_VELOCITY:
        Pilot_sendMvt(velocityVector);
        break;
    case CHECK:
        if(Pilot_hasBumped())
            Pilot_sendMvt(velocityVector);
        break;
    default:
        break;
    }
}

static void Pilot_sendMvt(VelocityVector velocityVector)
{
    if(velocityVector.direction)
    {
        switch (velocityVector.direction)
        {
            case LEFT:
                Robot_setWheelsVelocity(-velocityVector.power, velocityVector.power);
                break;
            case RIGHT:
                Robot_setWheelsVelocity(velocityVector.power, -velocityVector.power);
                break;
            case FORWARD:
                Robot_setWheelsVelocity(velocityVector.power, velocityVector.power);
                break;
            case BACKWARD:
                Robot_setWheelsVelocity(-velocityVector.power, -velocityVector.power);
                break;
            case NO:
                Robot_setWheelsVelocity(DEFAULT_POWER, DEFAULT_POWER);
                break;
            default:
                Robot_setWheelsVelocity(DEFAULT_POWER, DEFAULT_POWER);     
                break;
        }
    }
}

static bool_e Pilot_hasBumped()
{
    SensorState local = Robot_getSensorState();
    pilot->pilot_state.collision = local.collision == BUMPED ? BUMPED : NO_BUMP;
    return local.collision == BUMPED;
}
