#include <nusys.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <float.h>

#include "scene/scene.h"
#include "config/config.h"
#include "debug/debug.h"
#include "time/time.h"

#include "physics/physics.h"

#include "camera/camera.h"
#include "camera/camera_states.h"
#include "camera/camera_control.h"

#include "camera/light.h"

#include "actor/actor.h"
#include "actor/actor_states.h"
#include "actor/actor_control.h"
#include "actor/collision/actor_collision_detection.h"
#include "actor/collision/actor_collision_response.h"

#include "scene/scenery.h"

#include "assets/axis.h"
#include "assets/textures.h"
#include "assets/assets.h"

//#include "notepad.h"


// Display list
Gfx glist[GLIST_LENGTH];
Gfx* glistp;


// globals

TimeData time_data;

Camera camera = {
    distance_from_barycenter: 700,
    target_distance: 700,
    
    angle_around_barycenter: 0,
    pitch: 15,

    offset_angle: 22,
    offset_height: 180,

    field_of_view: 65,

    near_clipping: 100,
    far_clipping: 10000,
    
    settings: {
        orbitational_acceleration_rate: 15,
        orbitational_max_velocity: {120, 100},
        zoom_acceleration_rate: 60,
        zoom_deceleration_rate: 20,
        zoom_max_speed: 300,
        distance_from_baricenter: 700,
        field_of_view: 63,
	    field_of_view_aim: 45,
        offset_acceleration_rate: 25,
        offset_deceleration_rate: 45,
        offset_max_speed: 160,
        offset_angle: 23,
        offset_angle_aim: 30,
        max_pitch: 70,
    },
};

LightData light = {
    rotation: {57, 57, 57,},
};

Scenery axis = {
    scale: {1, 1, 1},
    position: {0, 0, 0},
    rotation: {0, 0, 0},
    mesh: gfx_axis,
};

Actor player = {
    mesh: gfx_capsule_mesh,
    scale: {1, 1, 1},
    grounding_height: 0,
    body: {
        position: {0, 0, 0,},
        velocity: {0, 0, 0,},
        rotation: {0, 0, 0,},
    },
    settings: {
        idle_acceleration_rate: 9,
        walk_acceleration_rate: 4,
        run_acceleration_rate: 6,
        roll_acceleration_rate: 20,
        roll_acceleration_grip_rate: 2,
        jump_acceleration_rate: 50,
        aerial_control_rate: 2.5,
        walk_target_speed: 200,
        run_target_speed: 650,
        sprint_target_speed: 900,
        idle_to_roll_target_speed: 300,
        idle_to_roll_grip_target_speed: 50,
        walk_to_roll_target_speed: 400,
        run_to_roll_target_speed: 780,
        sprint_to_roll_target_speed: 980,
        jump_target_speed: 800, 
        jump_timer_max: 0.13,
        fall_max_speed: -2650,
    },
};
ActorCollider player_collider = {
    settings: {
        body_radius: 35,
        body_height: 180,
    }
};
ActorContactData player_contact;
ActorContactData ground_contact;

Scenery ground = {
    scale: {1, 1, 1},
    position: {0, 0, 0},
    rotation: {0, 0, 0},
    mesh: gfx_ground_mesh,
};

Scenery laser = {
    scale: {1, 384, 1},
    position: {1920, -1200, 90},
    rotation: {0, 0, 86},
    mesh: gfx_laser,
};
Ray ray;

Scenery blue_box = {
    scale: {3.0f, 3.0f, 3.0f,},
    position: {650, 1150, 370},
    rotation: {0.0f, 0.0f, 0.0f},
    mesh: gfx_blue_cube_mesh,
};
AABB blue_box_collider;

Scenery green_box = {
    scale: {5.0f, 5.0f, 5.0f,},
    position: {1150, 650, -50},
    rotation: {0.0f, 0.0f, 0.0f},
    mesh: gfx_green_cube_mesh,
};
AABB green_box_collider;

Scenery black_ball = {
    scale: {3.0f, 3.0f, 3.0f,},
    position: {1150, -650, 90},
    rotation: {0, 0, 0},
    mesh: gfx_black_sphere_mesh,
};
Sphere black_ball_collider = {
    radius: 150,
    center: {1150, -650, 90},
};

Scenery blue_ball = {
    scale: {5.0f, 5.0f, 5.0f,},
    position: {650, -1150, 0},
    rotation: {270, 0, 0},
    mesh: gfx_light_blue_sphere_mesh,
};
Sphere blue_ball_collider = {
    radius: 250,
    center: {650, -1150, 0},
};

Scenery grey_box = {
    scale: {3.0f, 3.0f, 3.0f,},
    position: {-1150, 650, 435},
    rotation: {45.0f, 45.0f, 60.0f},
    mesh: gfx_grey_cube_mesh,
};
Box grey_box_collider = {
    size: {300.0f, 300.0f, 300.0f,},
    center: {-1150, 650, 435},
    rotation: {45.0f, 45.0f, 60.0f},
};

Scenery purple_box = {
    scale: {5.0f, 5.0f, 5.0f,},
    position: {-650, 1150, 0},
    rotation: {45.0f, 20.0f, 20.0f},
    mesh: gfx_purple_cube_mesh,
};
Box purple_box_collider = {
    size: {500.0f, 500.0f, 500.0f,},
    center: {-650, 1150, 0},
    rotation: {45.0f, 20.0f, 20.0f},
};

Scenery platform = {
    scale: {1.0f, 1.0f, 0.1f,},
    position: {-1100, -1100, 871},
    rotation: {0, 60, 45},
    mesh: gfx_platform_mesh,
};
Plane platform_collider;

// small cubes for debugging purposes
Scenery red_dots[10];
Scenery green_dots[10];

// functions

void scene_set_dl()
{   
    // debug data    
    for (int i = 0; i < 10; i++) {
        scenery_set_dl(&red_dots[i]);
        scenery_set_dl(&green_dots[i]);
    }
    scenery_set_dl(&axis);
    scenery_set_dl(&laser);
    // actor
    actor_set_dl(&player);
    // scenery
    scenery_set_dl(&blue_box);
    scenery_set_dl(&green_box);
    scenery_set_dl(&grey_box);
    scenery_set_dl(&purple_box);
    scenery_set_dl(&blue_ball);
    scenery_set_dl(&black_ball);
    scenery_set_dl(&platform);
    scenery_set_dl(&ground);

}

void actorCollision_solve()
{
    actorContactData_clear(&player_contact);
    actorCollider_setVertical(&player_collider, &player.body.position);
    
    if (player.body.position.z != 0 
        && player.state != JUMP) {

        player.grounding_height = 0.0f;
        actor_setState(&player, FALLING);
    }

    // this part of the code is the prototype for the stick to ground algorithm
    actorCollision_setGroundDistance(&ground_contact, &player.body.position);
    
    float actor_to_ground = (player_collider.body.radius / cosf(rad(ground_contact.slope))) - player_collider.body.radius;
    
    if (actor_to_ground != 0
        && player.state != JUMP
        && ground_contact.slope > 0 && ground_contact.slope < 50
        && player.body.position.z > player.grounding_height
        && ground_contact.ground_distance > (actor_to_ground)
        && ground_contact.ground_distance < (actor_to_ground + 20)) {

        player.body.position.z -= ground_contact.ground_distance - (actor_to_ground);
        player.grounded = true;
        player.grounding_height = player.body.position.z;
        if (vector3_squaredMagnitude(&player.body.velocity) < 100) actor_setState(&player, STAND_IDLE);
        else actor_setState(&player, player.previous_state);
    }
    else actorContactData_clear(&ground_contact);
    // ends here
    
    if (actorCollision_contactSphere(&player_collider, &black_ball_collider)) {
        actorCollision_contactSphereSetData(&player_contact, &player_collider, &black_ball_collider);
        actorCollision_setResponse(&player, &player_contact, &player_collider);
    }
    
    if (actorCollision_contactSphere(&player_collider, &blue_ball_collider)) {
        actorCollision_contactSphereSetData(&player_contact, &player_collider, &blue_ball_collider);
        actorCollision_setResponse(&player, &player_contact, &player_collider);
    }
    
    if (actorCollision_contactAABB(&player_collider, &green_box_collider)) {
        actorCollision_contactAABBsetData(&player_contact, &player_collider, &green_box_collider);
        actorCollision_setResponse(&player, &player_contact, &player_collider);
    }
    if (actorCollision_contactAABB(&player_collider, &blue_box_collider)) {
        actorCollision_contactAABBsetData(&player_contact, &player_collider, &blue_box_collider);
        actorCollision_setResponse(&player, &player_contact, &player_collider);
    }

    if (actorCollision_contactBox(&player_collider, &purple_box_collider)) {
        actorCollision_contactBoxSetData(&player_contact, &player_collider, &purple_box_collider);
        actorCollision_contactBoxSetData(&ground_contact, &player_collider, &purple_box_collider);
        actorCollision_setResponse(&player, &player_contact, &player_collider);
    }

    if (actorCollision_contactBox(&player_collider, &grey_box_collider)) {
        actorCollision_contactBoxSetData(&player_contact, &player_collider, &grey_box_collider);
        actorCollision_setResponse(&player, &player_contact, &player_collider);
    }

    if (actorCollision_contactPlane(&player_collider, &platform_collider)) {
        actorCollision_contactPlaneSetData(&player_contact, &player_collider, &platform_collider);
        actorCollision_setResponse(&player, &player_contact, &player_collider);
    }

    actorCollision_collideWithPlayground(&player);
}

void print_debug_data()
{   
    ContactData contact;
    /*
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 1);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "point: %d %d %d", (int)(contact.point.x), (int)(contact.point.y), (int)(contact.point.z));
    */
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 1);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "%d", (int)(ground_contact.ground_distance));

    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 2);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "%d", (int)((ground_contact.ground_distance - ((player_collider.body.radius / cosf(rad(ground_contact.slope))) - player_collider.body.radius))));
     
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 4);
    if(player.state == STAND_IDLE) nuDebConPrintf(NU_DEB_CON_WINDOW0, "STAND IDLE");
    if(player.state == WALKING) nuDebConPrintf(NU_DEB_CON_WINDOW0, "WALKING");
    if(player.state == RUNNING) nuDebConPrintf(NU_DEB_CON_WINDOW0, "RUNNING");
    if(player.state == ROLL) nuDebConPrintf(NU_DEB_CON_WINDOW0, "ROLL");
    if(player.state == JUMP) nuDebConPrintf(NU_DEB_CON_WINDOW0, "JUMP");
    if(player.state == FALLING) nuDebConPrintf(NU_DEB_CON_WINDOW0, "FALLING");
    /*
    actorCollision_contactBoxSetData(&contact, &player_collider, &purple_box_collider);

    red_dots[0].position = contact.data.point;
    
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 1);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "normal: %d %d %d", (int)(100 * player_contact.data.normal.x), (int)(100 * player_contact.data.normal.y), (int)(100 * player_contact.data.normal.z));
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 2);
    nuDebConPrintf(NU_DEB_CON_WINDOW0, "slope %d", (int)player_contact.slope);


    if (actorCollision_contactSphere(&player_collider, &black_ball_collider)
        //|| actorCollision_contactSphere(&player_collider, &blue_ball_collider)
        //|| actorCollision_collisionAABB(&player_contact, &player_collider, &blue_box_collider)
        || actorCollision_contactAABB(&player_collider, &green_box_collider)
        || actorCollision_contactPlane(&player_collider, &platform_collider)
        || actorCollision_contactBox(&player_collider, &purple_box_collider)
        //|| actorCollision_collisionBox(&player_collider, &grey_box_collider)
        //|| actorCollision_intersectionRay(&player_collider, &ray)
        ) {

        nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 3);
        nuDebConPrintf(NU_DEB_CON_WINDOW0, "contact");
    }
    */
}

void scene_init(void)
{
    time_init(&time_data);

    actor_setState(&player, STAND_IDLE);
    actorCollider_init(&player_collider);
    actorContactData_clear(&ground_contact);

    Vector3 blue_box_size = vector3_returnScaled(&blue_box.scale, 100);
    Vector3 green_box_size = vector3_returnScaled(&green_box.scale, 100);

    aabb_setFromCenterAndSize(&blue_box_collider, &blue_box.position, &blue_box_size);
    aabb_setFromCenterAndSize(&green_box_collider, &green_box.position, &green_box_size);

    ray_setFromRotationAndPoint(&ray, &laser.position, &laser.rotation);

    plane_setFromRotationAndPoint(&platform_collider, &platform.rotation, &platform.position);

        for (int i = 0; i < 10; i++) {
            red_dots[i].position = (Vector3){0, 0, 0};
            red_dots[i].scale = (Vector3){4, 4, 4};
            red_dots[i].rotation = (Vector3){0, 0, 0};
            red_dots[i].mesh = gfx_red_dot;
        }

        for (int i = 0; i < 10; i++) {
            green_dots[i].position = (Vector3){0, 0, 0};
            green_dots[i].scale = (Vector3){4, 4, 4};
            green_dots[i].rotation = (Vector3){0, 0, 0};
            green_dots[i].mesh = gfx_green_dot;
        }
}

void scene_update()
{
    time_setData(&time_data);
    
    // Read the controllers
    nuContDataGetEx(&contdata[0], 0);
    nuContDataGetEx(&contdata[1], 1);

    // Camera update
    cameraControl_setOrbitalMovement(&camera, &contdata[0], &contdata[1]);
    camera_getOrbitalPosition(&camera, player.body.position, time_data.frame_time);
    
    // Actor update
    actorControl_setMotion(&player, &contdata[0], time_data.frame_time, camera.angle_around_barycenter, camera.offset_angle);
    actor_integrate(&player, time_data.frame_time);
    actorCollision_solve();
    actor_setState(&player, player.state);

}

void scene_draw(void)
{
    // Assign our glist pointer to our glist array for ease of access
    glistp = glist;

    // Initialize the RCP and framebuffer
    rcp_init();
    fb_clear(154, 181, 198);
    shading_set_options();
    
    light_set_dl(&light);
    camera_set_dl(&camera);
    scene_set_dl();  

    // Syncronize the RCP and CPU and specify that our display list has ended
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    
    // Ensure the chache lines are valid
    osWritebackDCache(&camera.projection, sizeof(camera.projection));
    osWritebackDCache(&camera.modeling, sizeof(camera.modeling));
    
    // Ensure we haven't gone over the display list size and start the graphics task
    debug_assert((glistp-glist) < GLIST_LENGTH);

    #if TV_TYPE != PAL
        // is this the command that actually draws stuff?
        nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
    #else
        nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
    #endif
    
    // Draw the debug text (doesn't work on PAL)
    #if TV_TYPE != PAL
        nuDebConClear(NU_DEB_CON_WINDOW0);
        print_debug_data();
        nuDebConDisp(NU_SC_SWAPBUFFER);
    #endif
}
