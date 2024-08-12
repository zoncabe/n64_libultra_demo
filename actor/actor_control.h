#ifndef ACTOR_CONTROLS_H
#define ACTOR_CONTROLS_H

/* ACTOR_CONTROLS.H
here are all the actor control related functions */


// function prototypes


void jump(Actor* actor, NUContData *contdata, float frame_time);

void roll(Actor* actor, NUContData *contdata);

void move_withStick(Actor *actor, NUContData *contdata, float camera_angle_around, float camera_offset);


void actorControl_setMotion(Actor* actor, NUContData *contdata, float frame_time, float camera_angle_around, float camera_offset);




void jump(Actor* actor, NUContData *contdata, float frame_time)
{    
    if (contdata->trigger & A_BUTTON && actor->state != ROLL && actor->state != JUMP && actor->state != FALLING) {
        

        actor->input.jump_hold = true; 
        actor->input.jump_released = false;
        actor_setState(actor, JUMP);
    }

    if (contdata->button & A_BUTTON && actor->state == JUMP) {

        actor->input.jump_hold = true; 
        actor->input.time_held += frame_time;
    }
    else {

        actor->input.jump_released = true;
        actor->input.jump_hold = false;
        actor->input.time_held = 0;  
    }

}

void roll(Actor* actor, NUContData *contdata)
{

}


/*move_withStick
controls actor state and target yaw based on stick input */

void move_withStick(Actor *actor, NUContData *contdata, float camera_angle_around, float camera_offset)
{
    int deadzone = 8;
    float stick_magnitude = 0; 

    if (fabs(contdata->stick_x) >= deadzone || fabs(contdata->stick_y) >= deadzone) {

        Vector2 stick = {contdata->stick_x, contdata->stick_y};
        
        stick_magnitude = vector2_magnitude(&stick);
        actor->target_rotation.z = deg(atan2(contdata->stick_x, -contdata->stick_y) - rad(camera_angle_around - (0.5 * camera_offset)));
    }

    
    if (stick_magnitude == 0 && actor->state != ROLL && actor->state != JUMP && actor->state != FALLING){
        actor_setState(actor, STAND_IDLE);
    }

    else if (stick_magnitude > 0 && stick_magnitude <= 80 && actor->state != ROLL && actor->state != JUMP && actor->state != FALLING){
        actor_setState(actor, WALKING);
    }

    else if (contdata->button & R_TRIG && stick_magnitude > 80 && actor->state != ROLL && actor->state != JUMP && actor->state != FALLING){
        actor_setState(actor, SPRINTING);
    }

    else if (stick_magnitude > 80 && actor->state != ROLL && actor->state != JUMP && actor->state != FALLING){
        actor_setState(actor, RUNNING);
    }
}


void actorControl_setMotion(Actor* actor, NUContData *contdata, float frame_time, float camera_angle_around, float camera_offset)
{    
   
    jump(actor, contdata, frame_time);

    move_withStick(actor, contdata, camera_angle_around, camera_offset);

}



#endif