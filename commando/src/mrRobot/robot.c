#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "robot.h"
#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


Robot *robot;

/* 
 * Private prototypes
 */
static void Robot_init();
static void Robot_deinit();

/* 
 * Public functions
 */
void Robot_new(void)
{
    if (!(robot = (Robot*) calloc(1, sizeof(Robot))))               // Allocate memory for the Robot structure
    {
        printf("ERROR NEW ROBOT : Robot memory not allocated.\n");  // If the program could not allocate memory, it print an error message
        exit(EXIT_SUCCESS);                                         // It exit 
    }
}

void Robot_start(void)
{
    Robot_new();
    Robot_init();
}

void Robot_stop(void)
{
    Robot_deinit();
    Robot_free();
}

void Robot_free(void)
{
    if(robot)
        free(robot);
}

void Robot_setWheelsVelocity(int mr, int ml)
{
    if(robot)
    {
        if
        (
            (Motor_setCmd(robot->motor_right, mr) == -1)
            ||
            (Motor_setCmd(robot->motor_left, ml) == -1)
        )
        {
            printf("ERROR ROBOT SET WHEELS VELOCITY\n");
        }
    }
    else
    {
        printf("ERROR ROBOT SET WHEELS VELOCITY : robot is null\n");
        exit(EXIT_SUCCESS);
    }
}

int Robot_getRobotSpeed(void)
{
    if(robot)
    {
        int ml = 0;
        int mr = 0;
        if
        (
            ((mr = Motor_getCmd(robot->motor_right)) == E_CMD) ||
            ((mr = Motor_getCmd(robot->motor_right)) == E_CMD)
        )
        {
            printf("ERROR ROBOT GET SPEED : error motor get command\n"); 
            return 0;
        }
        else
            return abs(mr + ml)/2;
    }
    else
    {
        printf("ERROR ROBOT GET SPEED : robot is null\n");
        return 0;
    }

}

SensorState Robot_getSensorState()
{
    SensorState state = {0};
    ContactStatus contact_status = ERROR;
    if(robot)
    {
        contact_status = ContactSensor_getStatus(robot->contact_bumper);
        switch (contact_status)
        {
        case ERROR:
                printf("ERROR ROBOT GET SENSOR STATE : error with contact bumper\n");
            break;
        case RELEASED:
                state.collision = NO_BUMP;
            break;
        case PRESSED:
                state.collision = BUMPED;
            break;
        default:
            break;
        }        
        if((state.luminosity = LightSensor_getStatus(robot->light_sensor)) == -1)
            printf("ERROR ROBOT GET SENSOR STATE : error with light sensor value\n");
    }
    else
    {
        state.collision = 0;
        state.luminosity = 0;
    }
    return state;
}


/* 
 * Private functions
 */
static void Robot_init()
{
    if(ProSE_Intox_init(INTOX_IP, INTOX_PORT) == 0)
    {
        if(robot)
        {
            if
            (
                ((robot->motor_right = Motor_open(RIGHT_MOTOR)) == NULL) 
                ||
                ((robot->motor_left = Motor_open(LEFT_MOTOR)) == NULL) 
                ||
                ((robot->contact_bumper = ContactSensor_open(FRONT_BUMPER)) == NULL) 
                ||
                ((robot->contact_under = ContactSensor_open(FLOOR_SENSOR)) == NULL) 
                ||
                ((robot->light_sensor = LightSensor_open(LIGHT_SENSOR)) == NULL)
            )
            {
                printf("ERROR ROBOT INIT : can't open motors and sensors\n");
                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            printf("ERROR ROBOT INIT : robot is NULL\n");
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        printf("ERROR ROBOT INIT : intox connection can't be initialized\n");
        exit(EXIT_SUCCESS);
    }
}

static void Robot_deinit()
{
    if(robot)
    {
        if
        ( 
            (Motor_close(robot->motor_right) != 0) && 
            (Motor_close(robot->motor_left) != 0) &&
            (ContactSensor_close(robot->contact_bumper) != 0) &&
            (ContactSensor_close(robot->contact_under) != 0) &&
            (LightSensor_close(robot->light_sensor) != 0)
        )
        {
            printf("ERROR ROBOT DEINIT : close motors and sensors\n");
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        printf("ERROR ROBOT DEINIT : robot is NULL\n");
        exit(EXIT_SUCCESS);
    }
}