#ifndef CONTROLS_H
#define CONTROLS_H

/* CONTROLS.H
here are all the input related functions */


// function prototypes

void move_viewport_stick(Viewport *viewport, NUContData contdata[1]);
void move_entity_stick(Entity *entity, Viewport viewport, NUContData contdata[1]);



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/* move_viewport_stick
changes the viewport variables depending on controller input*/

void move_viewport_stick(Viewport *viewport, NUContData *contdata)
{
    if (fabs(contdata->stick_x) < 7) contdata->stick_x = 0;
    if (fabs(contdata->stick_y) < 7) contdata->stick_y = 0;

    viewport->angle_around_target += contdata->stick_x / 20;
    viewport->pitch += contdata->stick_y / 20;

    if (viewport->angle_around_target > 360) {viewport->angle_around_target  = viewport->angle_around_target - 360;}
    if (viewport->angle_around_target < 0) {viewport->angle_around_target  = viewport->angle_around_target + 360;}

    if (viewport->pitch > 85) viewport->pitch = 85;
    if (viewport->pitch < -85) viewport->pitch = -85;
}


/*move_entity_stick
changes the entity position variables depending on controller input */

void move_entity_stick(Entity *entity, Viewport viewport, NUContData *contdata)
{
    float input_amount; 
    float directional_speed = 0;

	if (fabs(contdata->stick_x) < 9) {contdata->stick_x = 0;}
	if (fabs(contdata->stick_y) < 9) {contdata->stick_y = 0;}


    if (fabs(contdata->stick_x) > 0 || fabs(contdata->stick_y) > 0) {
        
        input_amount = calculate_hypotenuse(contdata->stick_x ,contdata->stick_y);
        entity->target_yaw = deg(atan2(contdata->stick_x, -contdata->stick_y) - rad(viewport.angle_around_target));
    }

    if (fabs(entity->speed[0]) > 0 || fabs(entity->speed[1]) > 0) directional_speed = calculate_hypotenuse(entity->speed[0], entity->speed[1]);

    //debug data collecting
    entity->input_amount = input_amount;
    entity->directional_speed = directional_speed;
    
    if (input_amount == 0){

        if  (fabs(entity->speed[0]) < 1 && fabs(entity->speed[1]) < 1){
            entity->speed[0] = 0;
            entity->speed[1] = 0;
        }
        
        entity->acceleration[0] = 9 * (0 - entity->speed[0]);
        entity->acceleration[1] = 9 * (0 - entity->speed[1]);

        if (entity->state =! IDLE && fabs(directional_speed) < 5) set_entity_state(entity, IDLE);
    }

    else if (input_amount > 0 && input_amount <= 60){

        set_entity_state(entity, WALKING);
    }

    else if (input_amount > 60){

        set_entity_state(entity, RUNNING);
    }
    
    if (entity->speed[0] != 0 || entity->speed[1] != 0) entity->yaw = deg(atan2(entity->speed[0], -entity->speed[1]));
}


#endif