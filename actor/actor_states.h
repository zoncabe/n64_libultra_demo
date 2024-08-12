#ifndef ACTORSTATES_H
#define ACTORSTATES_H

/* ACTORSTATES.H
here are all the actor state machine related functions */

#include "actor_movement.h"


// function prototypes


void set_idle (Actor *actor);

void set_walking (Actor *actor);
            
void set_running (Actor *actor);
            
void set_sprinting (Actor *actor);

void set_jump (Actor *actor);

void set_falling (Actor *actor);


void actor_setState (Actor *actor, ActorState state);



void set_idle(Actor *actor)
{
    if (actor->state == STAND_IDLE) return;

    actor_setStopingAcceleration(actor);
    
    if  (fabs(actor->body.velocity.x) < 1 && fabs(actor->body.velocity.y) < 1){

        vector3_init(&actor->body.velocity);
    
        actor->target_rotation.z = actor->body.rotation.z;
        actor->state = STAND_IDLE;

        actor->previous_state = STAND_IDLE;
    }
    
}


void set_walking(Actor *actor)
{
    actor_setHorizontalAcceleration (actor, actor->settings.walk_target_speed, actor->settings.walk_acceleration_rate);

    if (actor->state == WALKING) return;
    
    actor->state = WALKING;
    actor->previous_state = WALKING;
}


void set_running(Actor *actor)
{
    actor_setHorizontalAcceleration (actor, actor->settings.run_target_speed, actor->settings.run_acceleration_rate);

    if (actor->state == RUNNING) return;
    
    actor->state = RUNNING;
    actor->previous_state = RUNNING;
}


void set_sprinting(Actor *actor)
{
    actor_setHorizontalAcceleration (actor, actor->settings.sprint_target_speed, actor->settings.run_acceleration_rate);

    if (actor->state == SPRINTING) return;
    
    actor->state = SPRINTING;
    actor->previous_state = SPRINTING;
}


void set_jump(Actor *actor) 
{       
    if (actor->input.jump_hold && !actor->input.jump_released && actor->input.time_held < actor->settings.jump_timer_max){   
    
        actor_setJumpAcceleration (actor, actor->settings.jump_target_speed, actor->settings.jump_acceleration_rate);
        actor_setHorizontalAcceleration (actor, actor->jumping_speed, actor->settings.aerial_control_rate);
    } 
    
    else if (actor->body.velocity.z >= 0){

        actor_setHorizontalAcceleration (actor, actor->jumping_speed, actor->settings.aerial_control_rate);
        actor->body.acceleration.z = ACTOR_GRAVITY;
    }
    
    else {

        actor_setState(actor, FALLING);
        actor->input.time_held = 0;
        return;
    }

    if (actor->state == JUMP) return;
    
    actor->jumping_speed = actor->horizontal_speed;
    actor->body.velocity.z = actor->settings.jump_target_speed * 0.5f;
    actor->state = JUMP;
    actor->grounded = false;
    actor->grounding_height = 0.0f;
}


void set_falling (Actor *actor)
{   
    actor_setHorizontalAcceleration (actor, actor->jumping_speed, actor->settings.aerial_control_rate);
    actor->body.acceleration.z = ACTOR_GRAVITY;

    if (actor->body.position.z <= actor->grounding_height) {

        actor->grounded = true;
        actor->jumping_speed = 0.0f;
        actor->body.acceleration.z = 0.0f;
        actor->body.velocity.z = 0.0f;
        actor->body.position.z = actor->grounding_height;

        actor_setState (actor, actor->previous_state);

        return;
    }


    if (actor->state == FALLING ) return;

    actor->state = FALLING;
    actor->grounded = false;
    actor->body.velocity.z = 0.0f; 
    actor->grounding_height = 0.0f;
}



void actor_setState(Actor *actor, ActorState state) 
{
    switch(state) {

        case STAND_IDLE: {
            set_idle (actor);
            break;
        }
        case WALKING: {
            set_walking (actor);
            break;
        }
        case RUNNING: {
            set_running (actor);
            break;
        }
        case SPRINTING: {
            set_sprinting (actor);
            break;
        }
        case JUMP: {
            set_jump (actor);
            break;
        }
        case FALLING: {
            set_falling (actor);
            break;
        }   
    }
}

#endif