#ifndef CONTROLS_H
#define CONTROLS_H

/* CONTROLS.H
here are all the input related functions */


// function prototypes

int input(u32 input);
void move_viewport_stick(Viewport *viewport, NUContData *contdata);
void move_viewport_c_buttons(Viewport *viewport, NUContData *contdata, TimeData timedata);
void move_entity_stick(Entity *entity, Viewport viewport, NUContData *contdata);
void set_entity_actions(Entity *entity, NUContData *contdata);



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/* input
 auxiliary function for 8 directional movement*/

int input(u32 input){
    if (input == 0) {return 0;}
    else {return 1;}
}


/* move_viewport_stick
changes the viewport variables depending on controller input*/

void move_viewport_stick(Viewport *viewport, NUContData *contdata)
{
    int deadzone = 8;
    float input_x = 0;
    float input_y = 0;

    if (fabs(contdata->stick_x) >= deadzone || fabs(contdata->stick_y) >= deadzone) {
        input_x = contdata->stick_x;
        input_y = contdata->stick_y;
    }

    if (input_x == 0 && input_y == 0) {
        viewport->rotational_target_speed[0] = 0;
        viewport->rotational_target_speed[1] = 0;
    }
    
    else if (input_x != 0 || input_y != 0) {
        viewport->rotational_target_speed[0] = input_x;
        viewport->rotational_target_speed[1] = input_y;
    }
    
    if (contdata->trigger & L_TRIG) set_viewport_state (viewport, THIRD_PERSON_SHOOTER_AIMING);
    else set_viewport_state (viewport, THIRD_PERSON_SHOOTER_GAMEPLAY);
}


void move_viewport_c_buttons(Viewport *viewport, NUContData *contdata, TimeData timedata)
{
    float input_x = 0;
    float input_y = 0;

    if ((contdata->button & R_CBUTTONS) || (contdata->button & L_CBUTTONS) || (contdata->button & U_CBUTTONS) || (contdata->button & D_CBUTTONS)){
        
        input_x = input(contdata->button & R_CBUTTONS) - input(contdata->button & L_CBUTTONS);
        input_y = input(contdata->button & U_CBUTTONS) - input(contdata->button & D_CBUTTONS);
    }

    if (input_x == 0) viewport->rotational_target_speed[0] = 0; 
    else viewport->rotational_target_speed[0] = input_x * viewport->speed_settings[0];

	if (input_y == 0) viewport->rotational_target_speed[1] = 0; 
    else viewport->rotational_target_speed[1] = input_y * viewport->speed_settings[1];

    if (contdata->trigger & L_TRIG) set_viewport_state (viewport, THIRD_PERSON_SHOOTER_AIMING);
    else set_viewport_state (viewport, THIRD_PERSON_SHOOTER_GAMEPLAY);
}


void set_entity_actions(Entity *entity, NUContData *contdata)
{
    //if (contdata->trigger & D_CBUTTONS);
    //if (contdata->trigger & R_TRIG); 
    
    if (contdata->trigger & B_BUTTON) set_entity_state(entity, ROLL);
    
    if (contdata->trigger & A_BUTTON) set_entity_state(entity, JUMP);
}


/*move_entity_stick
changes the entity state and target yaw depending on controller input */

void move_entity_stick(Entity *entity, Viewport viewport, NUContData *contdata)
{
    int deadzone = 8;
    float input_amount = 0; 

    if (fabs(contdata->stick_x) >= deadzone || fabs(contdata->stick_y) >= deadzone) {
        input_amount = calculate_diagonal(contdata->stick_x ,contdata->stick_y);
        entity->target_yaw = deg(atan2(contdata->stick_x, -contdata->stick_y) - rad(viewport.angle_around_entity - (viewport.offset_angle / 2)));
    }

    //debug data collecting
    entity->input_amount = input_amount;
    
    if (input_amount == 0 && entity->state != ROLL){
        set_entity_state(entity, IDLE);
    }

    else if (input_amount > 0 && input_amount <= 64 && entity->state != ROLL){
        set_entity_state(entity, WALKING);
    }

    else if (input_amount > 64 && entity->state != ROLL){
        set_entity_state(entity, RUNNING);
    }
}


#endif