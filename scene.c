/*SCENE.C
handles the demo scene */

//testing git

#include <nusys.h>
#include <string.h> // Needed for CrashSDK compatibility
#include <math.h>

#include "config.h"
#include "helper.h"
#include "sausage64.h"
#include "debug.h"
#include "scene.h"
#include "n64_logo.h"

#include "nickTex.h"
#include "nickMdl.h"
#include "shapesMdl.h"
#include "terrain.h"

#include "math_util.h"
#include "time.h"
#include "entity.h"
#include "viewport.h"
#include "entitystates.h"
#include "viewportstates.h"
#include "controls.h"
#include "collision.h"


// macros

#define USB_BUFFER_SIZE 256


// function prototypes

void entity_animcallback(u16 anim);

void set_viewport(Viewport *viewport);

void set_light();

void set_entity (Entity *entity);

void set_scenery (Scenery *scenery);


// globals

TimeData timedata;

Viewport viewport = {

    distance_from_entity: 250,
    target_distance: 600,
    angle_around_entity: 30,
    offset_angle: 20,
    pitch: 10,
    height: 90,
    
    settings: {

        rotational_acceleration_rate: 8,
        rotational_max_speed: {120, 100},

        zoom_acceleration_rate: 30,
        zoom_deceleration_rate: 10,
        zoom_max_speed: 600,

        target_zoom: 230,
        target_zoom_aim: 130,

	    offset_acceleration_rate: 6,
        offset_deceleration_rate: 30,
	    offset_max_speed: 40,

        target_offset: 23,
        target_offset_aim: 30,

	    max_pitch: 70,
    },
};


LightData light = {

    angle: {60, 60, 60,},
};


Entity player = {

    scale: 1,

    position: {-300, -500, 0,},
    
    yaw: 150,

    framerate: 0.5,

    settings: {

        idle_acceleration_rate: 9,
        walk_acceleration_rate: 4,
        run_acceleration_rate: 6,
        roll_acceleration_rate: 20,
        roll_acceleration_grip_rate: 2,
        jump_acceleration_rate: 40,
        aerial_control_rate: 0.5,

        walk_target_speed: 120,
        run_target_speed: 260,
        idle_to_roll_target_speed: 140,
        walk_to_roll_target_speed: 160,
        run_to_roll_target_speed: 280,
        jump_target_speed: 350, 

        idle_to_roll_change_grip_tick: 20,
        walk_to_roll_change_grip_tick: 23,
        run_to_roll_change_grip_tick: 20,

        walk_grounded_foot_change_tick: 15,
        run_grounded_foot_change_tick: 10,
        jump_timer_max: 0.20,
    },
};

Mtx playerMtx[MESHCOUNT_nick];

#define AXIS_COUNT 8

Scenery axis_cube[AXIS_COUNT] = {

    {model:        gfx_axis, scale: {1 ,1 ,1}, position: { -100, -100,   0}, yaw:   0, pitch: 0, roll:   0,},
    {model: gfx_mirror_axis, scale: {1 ,1 ,1}, position: {  100, -100,   0}, yaw:  90, pitch: 0, roll:   0,},
    {model:        gfx_axis, scale: {1 ,1 ,1}, position: {  100,  100,   0}, yaw: 180, pitch: 0, roll:   0,},
    {model: gfx_mirror_axis, scale: {1 ,1 ,1}, position: { -100,  100,   0}, yaw: 270, pitch: 0, roll:   0,},
    
    {model: gfx_mirror_axis, scale: {1 ,1 ,1}, position: { -100, -100, 50}, yaw: 270, pitch: 0, roll: 180,},
    {model:        gfx_axis, scale: {1 ,1 ,1}, position: {  100, -100, 50}, yaw:   0, pitch: 0, roll: 180,},
    {model: gfx_mirror_axis, scale: {1 ,1 ,1}, position: {  100,  100, 50}, yaw:  90, pitch: 0, roll: 180,},
    {model:        gfx_axis, scale: {1 ,1 ,1}, position: { -100,  100, 50}, yaw: 180, pitch: 0, roll: 180,},
    
    
};

Scenery cube = {

    yaw: 10,
    pitch: 72,
    roll: 36,
    scale: {1 ,1 ,1},
    position: {0, 0, 100},

    rotational_speed: {270, 50, 190,},

    model: gfx_cube,
};


AABB cube_bounding_box = {

    min: {-100, -100, 0},
    max: {100, 100, 50},
};

Sphere player_bounding_box = {

    radius: 20,
};


void rotate_cube(){

    if ( cube.rotational_speed[0] > 270){

        cube.rotational_speed[0] -= 50;
        cube.rotational_speed[1] -= 50;
        cube.rotational_speed[2] -= 50;
    }

    cube.yaw += cube.rotational_speed[0] * timedata.frame_duration;
    cube.pitch -= cube.rotational_speed[1] * timedata.frame_duration;
    cube.roll += cube.rotational_speed[2] * timedata.frame_duration;
}

/*
int swap_cube_index = 0;

void swap_cube(){

    cube.scale_speed = 0;

    if ( cube.rotational_speed[0] < 1270){

        cube.rotational_speed[0] += 50;
        cube.rotational_speed[1] += 50;
        cube.rotational_speed[2] += 50;
    }

    cube.yaw += cube.rotational_speed[0] * timedata.frame_duration;
    cube.pitch -= cube.rotational_speed[1] * timedata.frame_duration;
    cube.roll += cube.rotational_speed[2] * timedata.frame_duration;

    swap_cube_index = 0;

    cube.model = gfx_n64_logo;
}
*/


/* entity_animcallback
Called before an animation finishes
@param The animation that is finishing */

void entity_animcallback(u16 anim)
{
    switch(anim)
    {
        case ANIMATION_nick_stand_to_roll_left:
            
            set_entity_state(&player, STAND_IDLE);
            break;

        case ANIMATION_nick_run_to_roll_left:
        
            set_entity_state(&player, STAND_IDLE);
            break;
            
        case ANIMATION_nick_jump_left:
        
            sausage64_set_anim(&player.model, ANIMATION_nick_fall_idle_left);
            break;
    }
}


/* set viewport
handles the system functions that enters the viewport position and rotation values */

void set_viewport(Viewport *viewport)
{
    guPerspective(
    	&viewport->projection, &viewport->normal, 
        45, (float)SCREEN_WD / (float)SCREEN_HT, 
    	10.0, 10000.0, 0.01);
    
    guLookAt(
    	&viewport->position_mtx,
    	viewport->position[0], viewport->position[1], viewport->position[2],
    	viewport->target[0], viewport->target[1], viewport->target[2],
    	0, 0, 1
  	);

    gSPMatrix(glistp++, &viewport->projection, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &viewport->position_mtx, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPPerspNormalize(glistp++, &viewport->normal);

    guMtxIdent(&viewport->modeling);
    gSPMatrix(glistp++, &viewport->modeling, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);  
}


/* set light
temporary function until i learn how the lights work  */

void set_light(LightData *light)
{
    int i;

    //color
    for (i=0; i<3; i++) {

        // Ambient color
        light->amb.l.col[i] = 140;
        light->amb.l.colc[i] = 140;

        //directional light color
        light->dir.l.col[i] = 255;
        light->dir.l.colc[i] = 255;
    }

    // Direction
    light->dir.l.dir[0] = -127*sinf(light->angle[0]*0.0174532925);
    light->dir.l.dir[1] = 127*sinf(light->angle[2]*0.0174532925)*cosf(light->angle[0]*0.0174532925);
    light->dir.l.dir[2] = 127*cosf(light->angle[2]*0.0174532925)*cosf(light->angle[0]*0.0174532925);

    // Send the light struct to the RSP
    gSPNumLights(glistp++, NUMLIGHTS_1);
    gSPLight(glistp++, &light->dir, 1);
    gSPLight(glistp++, &light->amb, 2);
    gDPPipeSync(glistp++); 
}


/* set entity
handles the system functions that enters an entity's position and rotation values */

void set_entity (Entity *entity)
{
    guTranslate(&entity->position_mtx, entity->position[0], entity->position[1], entity->position[2]);
    guRotate(&entity->rotation_mtx[0], entity->pitch, 1, 0, 0);
    guRotate(&entity->rotation_mtx[1], entity->yaw, 0, 0, 1);
    guScale(&entity->scale_mtx, entity->scale, entity->scale, entity->scale);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&entity->position_mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&entity->rotation_mtx[0]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&entity->rotation_mtx[1]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&entity->scale_mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

    sausage64_drawmodel(&glistp, &entity->model);

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
}


/* set scenery
handles the system functions that enters a scenery object's position and rotation values */

void set_scenery (Scenery *scenery)
{
    guTranslate(&scenery->position_mtx, scenery->position[0], scenery->position[1], scenery->position[2]);
    guRotate(&scenery->rotation_mtx[0], scenery->pitch, 1, 0, 0);
    guRotate(&scenery->rotation_mtx[1], scenery->yaw, 0, 0, 1);
    guRotate(&scenery->rotation_mtx[2], scenery->roll, 0, 1, 0);
    guScale(&scenery->scale_mtx, scenery->scale[0], scenery->scale[1], scenery->scale[2]);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&scenery->position_mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&scenery->rotation_mtx[0]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&scenery->rotation_mtx[1]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&scenery->rotation_mtx[2]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&scenery->scale_mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

    gSPDisplayList(glistp++, scenery->model);

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
     gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
}


void set_scene()
{        
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

    set_entity(&player);

    for (int i = 0; i < AXIS_COUNT; i++) {

        set_scenery(&axis_cube[i]);
    }

    set_scenery (&cube);

      // Syncronize the RCP and CPU and specify that our display list has ended
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    
    // Ensure the chache lines are valid
    osWritebackDCache(&viewport.projection, sizeof(viewport.projection));
    osWritebackDCache(&viewport.modeling, sizeof(viewport.modeling));

}

/* print_debug_data      
sets debug information to be printed on screen */

void print_debug_data()
{
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 1);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "time  %d", (int) get_time());
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 2);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "FPS  %d", (int) timedata.FPS);

    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 3);
    if(player.state == STAND_IDLE) nuDebConPrintf(NU_DEB_CON_WINDOW0, "STAND IDLE");
    if(player.state == WALKING) nuDebConPrintf(NU_DEB_CON_WINDOW0, "WALKING");
    if(player.state == RUNNING) nuDebConPrintf(NU_DEB_CON_WINDOW0, "RUNNING");
    if(player.state == ROLL) nuDebConPrintf(NU_DEB_CON_WINDOW0, "ROLL");
    if(player.state == JUMP) nuDebConPrintf(NU_DEB_CON_WINDOW0, "JUMP");

    if (collision_sphere_aabb(player_bounding_box, cube_bounding_box)) {
        nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 4);
        nuDebConPrintf(NU_DEB_CON_WINDOW0, "COLLISION");
    }
    /*

    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 5);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "time_held  %d", (int)(player.input.time_held * 1000));
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 6);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "z acceleration  %d", (int)player.acceleration[2]);
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 7);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "grounded  %d", (int)player.grounded);
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 8);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "hold time  %d", (int)(100 * player.hold_time));


    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 9);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "COLLISION");
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 10);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "target y  %d", (int)viewport.target[1]);
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 11);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "target z  %d", (int)viewport.target[2]);
    */
}

/* scene_init
initializes the elements of the scene that require function calling */

void init_scene(void)
{
    // Initialize nick

    //init_entity(&player, 15, playerMtx, entity_animcallback);
    sausage64_initmodel(&player.model, MODEL_nick, playerMtx);

    player.model.transition_tick_count = 300;
    player.model.transition_tick_count = 300;

    set_entity_state(&player, STAND_IDLE);

    sausage64_set_animcallback(&player.model, entity_animcallback);
    
    /*// Set nick's animation speed based on region
    #if TV_TYPE == PAL 
    #else
    #endif */
}


/* update_scene
handles the elements that modify the scene state */

void update_scene()
{
    //updates the frame timings
    time_management(&timedata);

    // Advance entity animation
    sausage64_advance_anim(&player.model, player.framerate);
    
    // Read the controllers
    nuContDataGetEx(&contdata[0], 0);
    nuContDataGetEx(&contdata[1], 1);

    move_entity_stick(&player, viewport, &contdata[0]);

    set_entity_actions(&player, &contdata[0], timedata);

    set_entity_position(&player, timedata);

    set_entity_state(&player, player.state);  

    //move_viewport_stick(&viewport, &contdata[1]);
    move_viewport_c_buttons(&viewport, &contdata[0], timedata);

    set_viewport_position(&viewport, player, timedata);

    set_point(player_bounding_box.center, player.position);
    player_bounding_box.center[2] += player_bounding_box.radius;

    rotate_cube();
}


/* render_frame      
handles the system tasks given the setted scene */

void render_frame(void)
{
    // Assign our glist pointer to our glist array for ease of access
    glistp = glist;

    // Initialize the RCP and framebuffer
    rcp_init();
    fb_clear(180, 180, 180);
    
    set_viewport(&viewport);

    set_light(&light);

    set_scene();  
    
    // Ensure we haven't gone over the display list size and start the graphics task
    debug_assert((glistp-glist) < GLIST_LENGTH);

    #if TV_TYPE != PAL
        // is this the command that actually draws stuff?
        nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
    #else
        nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
    #endif
    
    // Draw the menu (doesn't work on PAL)
    #if TV_TYPE != PAL
        nuDebConClear(NU_DEB_CON_WINDOW0);
        print_debug_data();
        nuDebConDisp(NU_SC_SWAPBUFFER);
    #endif
}
