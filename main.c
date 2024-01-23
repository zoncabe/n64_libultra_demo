/***************************************************************
                            main.c
                               
Program entrypoint.
***************************************************************/

#include <nusys.h>
#include "config.h"
#include "main.h"
#include "debug.h"


/*********************************
        Function Prototypes
*********************************/

static void callback_prenmi();
static void callback_vsync(int tasksleft);

// Controller data
NUContData contdata[2];


/*==============================
    mainproc
    Initializes the game
==============================*/

void mainproc(void)
{
    // Start by selecting the proper television
    if (TV_TYPE == PAL)
    {
        osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);
        osViSetYScale(0.833);
        osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_ON);
    }
    else if (TV_TYPE == MPAL)
    {
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
        osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_ON);
    }
    
    // Initialize and activate the graphics thread and Graphics Task Manager.
    nuGfxInit();
    
    // Initialize the controller
    nuContInit();
        
    // Initialize the scene
    init_scene();
        
    // Set callback functions for reset and graphics
    nuGfxFuncSet((NUGfxFunc)callback_vsync);
    
    // Turn on the screen and loop forever to keep the idle thread busy
    nuGfxDisplayOn();
    while(1)
        ;
}


/*==============================
    callback_vsync
    Code that runs on on the graphics
    thread
    @param The number of tasks left to execute
==============================*/

static void callback_vsync(int tasksleft)
{
    // Update the stage, then draw it when the RDP is ready
    update_scene();
    if (tasksleft < 1)
        render_frame();
}


/*==============================
    callback_prenmi
    Code that runs when the reset button
    is pressed. Required to prevent crashes
==============================*/

static void callback_prenmi()
{
    nuGfxDisplayOff();
    osViSetYScale(1);
}