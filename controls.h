#ifndef CONTROLS_H
#define CONTROLS_H

/* CONTROLS.H
here are all the input related functions */


// function prototypes

void move_viewport_stick(Viewport *viewport, NUContData contdata[1]);
void set_entity_actions(Entity *entity, NUContData contdata[1]);
void move_entity_stick(Entity *entity, Viewport viewport, NUContData contdata[1]);



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/* move_viewport_stick
changes the viewport variables depending on controller input*/

void move_viewport_stick(Viewport *viewport, NUContData *contdata)
{
    if (fabs(contdata->stick_x) < 7) contdata->stick_x = 0;
    if (fabs(contdata->stick_y) < 7) contdata->stick_y = 0;

    viewport->angle_around_target += contdata->stick_x / 30;
    viewport->pitch += contdata->stick_y / 20;

    if (viewport->angle_around_target > 360) {viewport->angle_around_target  = viewport->angle_around_target - 360;}
    if (viewport->angle_around_target < 0) {viewport->angle_around_target  = viewport->angle_around_target + 360;}

    if (viewport->pitch > 85) viewport->pitch = 85;
    if (viewport->pitch < -85) viewport->pitch = -85;
}


void set_entity_actions(Entity *entity, NUContData contdata[1])
{
    //if (contdata[0].trigger & D_CBUTTONS);
    //if (contdata[0].trigger & R_TRIG); 
    
    if (contdata[0].trigger & B_BUTTON) set_entity_state(entity, ROLL);
    
    if (contdata[0].trigger & A_BUTTON) set_entity_state(entity, JUMP);
}


/*move_entity_stick
changes the entity position variables depending on controller input */

void move_entity_stick(Entity *entity, Viewport viewport, NUContData *contdata)
{
    int deadzone = 8;
    float input_amount = 0; 
    float directional_speed = 0;

    if (fabs(contdata->stick_x) > deadzone || fabs(contdata->stick_y) > deadzone) {
        input_amount = calculate_hypotenuse(contdata->stick_x ,contdata->stick_y);
        entity->target_yaw = deg(atan2(contdata->stick_x, -contdata->stick_y) - rad(viewport.angle_around_target));
    }

    if (fabs(entity->speed[0]) > 0 || fabs(entity->speed[1]) > 0) directional_speed = calculate_hypotenuse(entity->speed[0], entity->speed[1]);

    //debug data collecting
    entity->input_amount = input_amount;
    entity->directional_speed = directional_speed;
    
    if (input_amount == 0 && entity->state != ROLL){
        set_entity_state(entity, IDLE);
    }

    else if (input_amount > 0 && input_amount <= 64 && entity->state != ROLL){
        set_entity_state(entity, WALKING);
    }

    else if (input_amount > 64 && entity->state != ROLL){
        set_entity_state(entity, RUNNING);
    }
    
    // set yaw based on speed
    if (entity->speed[0] != 0 || entity->speed[1] != 0) entity->yaw = deg(atan2(entity->speed[0], -entity->speed[1]));
}


#endif