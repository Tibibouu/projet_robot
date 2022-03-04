/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
/**
 * @file  remoteui.h
 *
 * @brief  This component describe the remote user interface. It captures the user's controls and send them to the pilot module.
 *
 * @author Jerome Delatour
 * @date 17-04-2016
 * @version 1
 * @section License
 *
 * The MIT License
 *
 * Copyright (c) 2016, Jerome Delatour
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */


#ifndef REMOTEUI_H
#define REMOTEUI_H


#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"


typedef struct Remote_s
{
    char input;
}Remote;

typedef enum
{
    INIT,
    WAIT_CHAR,
    RUNNING,
    END
}RemoteState;

typedef enum
{
    LEFT = 1,
    RIGHT,
    FORWARD,
    BACKWARD,
    NO
}Direction;
    
typedef struct VelocityVector_s
{
    Direction direction;
    int power;
}VelocityVector;

typedef struct PilotState_s
{
    int speed;
    int collision;
    float luminosity;
}PilotState;

typedef enum
{
    ASK_PILOT_START,
    ASK_MVT,
    ASK_PILOT_CHECK,
    ASK_PILOT_STATE,
    ASK_PILOT_STOP
}Ask;


/**
 * initialize in memory RemoteUI
 * 
 */
extern void RemoteUI_new();

/**
 * Start RemoteUI and waits for the user's input until the user ask to quit
 *
 */
extern void RemoteUI_start();

/**
 * Stop RemoteUI
 *
 */
extern void RemoteUI_stop();

/**
 * destruct the RemoteUI from memory 
 *
 */
extern void RemoteUI_free();


#endif /* REMOTEUI_H */

