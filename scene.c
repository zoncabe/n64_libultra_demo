/***************************************************************
                           scene.c
                               
Handles the first level of the game.
***************************************************************/

#include <nusys.h>
#include <string.h> // Needed for CrashSDK compatibility
#include "config.h"
#include "helper.h"
#include "sausage64.h"
#include "debug.h"

#include "nickTex.h"
#include "nickMdl.h"


#include "math_util.h"
#include "time.h"
#include "entity.h"
#include "viewport.h"


/*********************************
              Macros
*********************************/

#define USB_BUFFER_SIZE 256


/*********************************
        Function Prototypes
*********************************/

void nick_animcallback(u16 anim);

void set_light();

void set_viewport(Viewport *viewport, Entity entity);


/*********************************
             Globals
*********************************/


Viewport viewport = {

    distance_from_target: 300,
    angle_around_target: 0,
    pitch: 10, 
};

Entity player = {

    position: { 0, 0, 0},
};

// Lights
static Light light_amb;
static Light light_dir;

// nick
Mtx nickMtx[MESHCOUNT_nick];
s64ModelHelper nick;
float nick_animspeed;


/*==============================
    scene_init
    Initialize the stage
==============================*/

void scene_init(void)
{
    // Initialize nick
    sausage64_initmodel(&nick, MODEL_nick, nickMtx);
    sausage64_set_anim(&nick, ANIMATION_nick_walk_left); 
    sausage64_set_animcallback(&nick, nick_animcallback);
    
    // Set nick's animation speed based on region
    #if TV_TYPE == PAL
        nick_animspeed = 0.66;
    #else
        nick_animspeed = 0.5;
    #endif
    
}


/*==============================
    scene_update
    Update stage variables every frame
==============================*/

void scene_update()
{
    int i; 
    
    // Advance nick's animation
    sausage64_advance_anim(&nick, nick_animspeed);
    
    
    /* -------- Controller -------- */
    
    // Read the controllers
    nuContDataGetEx(contdata, 0);
    nuContDataGetEx(contdata, 1);

    move_viewport_stick(&viewport, contdata);
    
}


void set_viewport(Viewport *viewport, Entity player){

    set_viewport_position(viewport, player);

    guPerspective(
    	&viewport->projection, &viewport->normal, 
        45, (float)SCREEN_WD / (float)SCREEN_HT, 
    	10.0, 10000.0, 0.01);
    
    guLookAt(
    	&viewport->pos_mtx,
    	viewport->position[0], viewport->position[1], viewport->position[2],
    	viewport->target[0], viewport->target[1], viewport->target[2],
    	0, 0, 1
  	);

    gSPMatrix(glistp++, &viewport->projection, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &viewport->pos_mtx, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPPerspNormalize(glistp++, &viewport->normal);

    guMtxIdent(&viewport->modeling);
    gSPMatrix(glistp++, &viewport->modeling, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    
}

void set_light(){

    // Setup the lights

    // Ambient color
    light_amb.l.col[0] = 140;
    light_amb.l.col[1] = 140;
    light_amb.l.col[2] = 140;

    light_amb.l.colc[0] = 140;
    light_amb.l.colc[1] = 140;
    light_amb.l.colc[2] = 140;

    //Light color
    light_dir.l.col[0] = 255;
    light_dir.l.col[1] = 255;
    light_dir.l.col[2] = 255;

    light_dir.l.colc[0] = 255;
    light_dir.l.colc[1] = 255;
    light_dir.l.colc[2] = 255;

    // Direction
    light_dir.l.dir[0] = 60;
    light_dir.l.dir[1] = 60;
    light_dir.l.dir[2] = 60;

    // Send the light struct to the RSP
    gSPNumLights(glistp++, NUMLIGHTS_1);
    gSPLight(glistp++, &light_dir, 1);
    gSPLight(glistp++, &light_amb, 2);
    gDPPipeSync(glistp++);
    
}


/*==============================
    draw_frame      
    Draw the stage
==============================*/

void draw_frame(void)
{
    
    // Assign our glist pointer to our glist array for ease of access
    glistp = glist;

    // Initialize the RCP and framebuffer
    rcp_init();
    fb_clear(180, 180, 180);
    
    set_viewport(&viewport, player);

    set_light();
    
    // Initialize the model matrix
    guMtxIdent(&viewport.modeling);
    gSPMatrix(glistp++, &viewport.modeling, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    // Initialize the RCP to draw stuff nicely
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetDepthSource(glistp++, G_ZS_PIXEL);
    gSPClearGeometryMode(glistp++,0xFFFFFFFF);
    gSPSetGeometryMode(glistp++, G_SHADE | G_ZBUFFER | G_CULL_BACK | G_SHADING_SMOOTH | G_LIGHTING);
    gSPTexture(glistp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF);
    gDPSetCombineMode(glistp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetTexturePersp(glistp++, G_TP_PERSP);
    gDPSetTextureFilter(glistp++, G_TF_BILERP);
    gDPSetTextureConvert(glistp++, G_TC_FILT);
    gDPSetTextureLOD(glistp++, G_TL_TILE);
    gDPSetTextureDetail(glistp++, G_TD_CLAMP);
    gDPSetTextureLUT(glistp++, G_TT_NONE);
    
    // Draw nick
    sausage64_drawmodel(&glistp, &nick);
    
    // Syncronize the RCP and CPU and specify that our display list has ended
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    
    // Ensure the chache lines are valid
    osWritebackDCache(&viewport.projection, sizeof(viewport.projection));
    osWritebackDCache(&viewport.modeling, sizeof(viewport.modeling));
    
    // Ensure we haven't gone over the display list size and start the graphics task
    debug_assert((glistp-glist) < GLIST_LENGTH);
    #if TV_TYPE != PAL
        nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
    #else
        nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
    #endif
    
    // Draw the menu (doesn't work on PAL)
    #if TV_TYPE != PAL
        nuDebConClear(NU_DEB_CON_WINDOW0);
        nuDebConDisp(NU_SC_SWAPBUFFER);
    #endif
}


/*********************************
     Model callback functions
*********************************/


/*==============================
    nick_animcallback
    Called before an animation finishes
    @param The animation that is finishing
==============================*/

void nick_animcallback(u16 anim)
{
    // Go to idle animation when we finished attacking
    switch(anim)
    {
        case ANIMATION_nick_run_to_roll_left:
            sausage64_set_anim(&nick, ANIMATION_nick_stand_idle_left);
            break;
    }
}

