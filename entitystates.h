#ifndef ENTITYSTATES_H
#define ENTITYSTATES_H

/* ENTITYSTATES.H
here are all the entity state machine related functions */

// entity properties



// variables


// function prototypes

void set_entity_state(Entity *entity, EntityState new_state);

void set_idle_state(Entity *entity);

void set_walking_state(Entity *entity);
            
void set_running_state(Entity *entity);

void set_roll_state(Entity *entity);

void set_grounded_foot(Entity *entity);



void set_entity_state(Entity *entity, EntityState new_state) 
{
    switch(new_state) {

        case STAND_IDLE: {
            set_idle_state(entity);
            break;
        }
        case WALKING: {
            set_walking_state(entity);
            break;
        }
        case RUNNING: {
            set_running_state(entity);
            break;
        }
        case ROLL: {
            set_roll_state(entity);
            break;
        }
    }
}


void set_idle_state(Entity *entity)
{
    if  (fabs(entity->speed[0]) < 1 && fabs(entity->speed[1]) < 1){
        entity->speed[0] = 0;
        entity->speed[1] = 0;
    }
    
    entity->acceleration[0] = 9 * (0 - entity->speed[0]);
    entity->acceleration[1] = 9 * (0 - entity->speed[1]);

    if (entity->state == STAND_IDLE) return;
    
    entity->target_yaw = entity->yaw;
    entity->state = STAND_IDLE;

    if (entity->grounded_foot == LEFT){
    sausage64_set_anim(&entity->model, ANIMATION_nick_look_around_left);
    }

    if (entity->grounded_foot == RIGHT){
    sausage64_set_anim(&entity->model, ANIMATION_nick_look_around_right);
    }

    entity->previous_state = STAND_IDLE;
}


void set_walking_state(Entity *entity)
{
    entity->target_speed[0] = entity->settings.walk_target_speed * sinf(rad(entity->target_yaw));
    entity->target_speed[1] = entity->settings.walk_target_speed * -cosf(rad(entity->target_yaw));

    entity->acceleration[0] = 4 * (entity->target_speed[0] - entity->speed[0]);
    entity->acceleration[1] = 4 * (entity->target_speed[1] - entity->speed[1]);

    if (entity->state == WALKING) return;
    
    entity->state = WALKING;
    sausage64_set_anim(&entity->model, ANIMATION_nick_walk_right);
    entity->previous_state = WALKING;
}


void set_running_state(Entity *entity)
{
    entity->target_speed[0] = entity->settings.run_target_speed * sinf(rad(entity->target_yaw));
    entity->target_speed[1] = entity->settings.run_target_speed * -cosf(rad(entity->target_yaw));

    entity->acceleration[0] = 6 * (entity->target_speed[0] - entity->speed[0]);
    entity->acceleration[1] = 6 * (entity->target_speed[1] - entity->speed[1]);

    if (entity->state == RUNNING) return;
    
    entity->state = RUNNING;
    sausage64_set_anim(&entity->model, ANIMATION_nick_run_right);
    entity->previous_state = RUNNING;
}


void set_roll_state(Entity *entity)
{
    switch(entity->previous_state) {

        case STAND_IDLE: {

            if(entity->model.animtick < entity->settings.idle_to_roll_change_grip_tick){

            entity->target_speed[0] = entity->settings.idle_to_roll_target_speed * sinf(rad(entity->yaw));
            entity->target_speed[1] = entity->settings.idle_to_roll_target_speed * -cosf(rad(entity->yaw));

            entity->acceleration[0] = 20 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 20 * (entity->target_speed[1] - entity->speed[1]);
            } 

            if(entity->model.animtick >= entity->settings.idle_to_roll_change_grip_tick){

            entity->target_speed[0] = entity->settings.walk_target_speed * sinf(rad(entity->target_yaw));
            entity->target_speed[1] = entity->settings.walk_target_speed * -cosf(rad(entity->target_yaw));

            entity->acceleration[0] = 2 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 2 * (entity->target_speed[1] - entity->speed[1]);
            }
        
            if (entity->state == ROLL) return;

            entity->state = ROLL;
            
            sausage64_set_anim_tick(&entity->model, ANIMATION_nick_stand_to_roll_left, 30);           

            break;
        }

        case WALKING: {

            if(entity->model.animtick < entity->settings.walk_to_roll_change_grip_tick){
                
            entity->target_speed[0] = entity->settings.walk_to_roll_target_speed * sinf(rad(entity->yaw));
            entity->target_speed[1] = entity->settings.walk_to_roll_target_speed * -cosf(rad(entity->yaw));

            entity->acceleration[0] = 20 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 20 * (entity->target_speed[1] - entity->speed[1]);
            }

            if(entity->model.animtick >= entity->settings.walk_to_roll_change_grip_tick){
                
            entity->target_speed[0] = entity->settings.walk_target_speed * sinf(rad(entity->target_yaw));
            entity->target_speed[1] = entity->settings.walk_target_speed * -cosf(rad(entity->target_yaw));

            entity->acceleration[0] = 3 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 3 * (entity->target_speed[1] - entity->speed[1]);
            }
        
            if (entity->state == ROLL) return;
        
            entity->state = ROLL;
            sausage64_set_anim(&entity->model, ANIMATION_nick_run_to_roll_left);

            break;
        }

        case RUNNING: {
            

            if(entity->model.animtick < entity->settings.run_to_roll_change_grip_tick){
                
            entity->target_speed[0] = entity->settings.run_to_roll_target_speed * sinf(rad(entity->yaw));
            entity->target_speed[1] = entity->settings.run_to_roll_target_speed * -cosf(rad(entity->yaw));

            entity->acceleration[0] = 20 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 20 * (entity->target_speed[1] - entity->speed[1]);
            }

            if(entity->model.animtick >= entity->settings.run_to_roll_change_grip_tick){
                
            entity->target_speed[0] = entity->settings.run_target_speed * sinf(rad(entity->target_yaw));
            entity->target_speed[1] = entity->settings.run_target_speed * -cosf(rad(entity->target_yaw));

            entity->acceleration[0] = 2 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 2 * (entity->target_speed[1] - entity->speed[1]);
            }

            if (entity->state == ROLL) return;
        
            entity->state = ROLL;
            sausage64_set_anim(&entity->model, ANIMATION_nick_run_to_roll_left);

            break;
        }
    }

    void set_grounded_foot(Entity *entity)
    {
        // TO DO 
    }
}

#endif