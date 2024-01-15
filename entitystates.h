#ifndef ENTITYSTATES_H
#define ENTITYSTATES_H

/* ENTITYSTATES.H
here are all the state machine related functions */

//variables


// function prototypes

void set_entity_state(Entity *entity, EntityState new_state);

void set_idle_state(Entity *entity);

void set_walking_state(Entity *entity);
            
void set_running_state(Entity *entity);

void set_roll_state(Entity *entity);



void set_entity_state(Entity *entity, EntityState new_state) 
{
    
    switch(new_state) {
        case IDLE: {
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
    if (entity->previous_state == IDLE) return;
    
    entity->state = IDLE;
    sausage64_set_anim(&entity->model, ANIMATION_nick_look_around_left);
    entity->previous_state = IDLE;
}


void set_walking_state(Entity *entity)
{

    entity->target_speed[0] = 80 * sinf(rad(entity->target_yaw));
    entity->target_speed[1] = 80 * -cosf(rad(entity->target_yaw));

    entity->acceleration[0] = 4 * (entity->target_speed[0] - entity->speed[0]);
    entity->acceleration[1] = 4 * (entity->target_speed[1] - entity->speed[1]);

    if (entity->state == WALKING) return;
    
    entity->state = WALKING;
    sausage64_set_anim(&entity->model, ANIMATION_nick_walk_left);
    entity->previous_state = WALKING;
}


void set_running_state(Entity *entity)
{
    entity->target_speed[0] = 260 * sinf(rad(entity->target_yaw));
    entity->target_speed[1] = 260 * -cosf(rad(entity->target_yaw));

    entity->acceleration[0] = 6 * (entity->target_speed[0] - entity->speed[0]);
    entity->acceleration[1] = 6 * (entity->target_speed[1] - entity->speed[1]);

    if (entity->state == RUNNING) return;
    
    entity->state = RUNNING;
    sausage64_set_anim(&entity->model, ANIMATION_nick_run_left);
    entity->previous_state = RUNNING;
}


void set_roll_state(Entity *entity)
{
    switch(entity->previous_state) {

        case IDLE: {
       
            entity->target_speed[0] = 120 * sinf(rad(entity->yaw));
            entity->target_speed[1] = 120 * -cosf(rad(entity->yaw));

            entity->acceleration[0] = 10 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 10 * (entity->target_speed[1] - entity->speed[1]);
        
            if (entity->state == ROLL) return;
            
            entity->state = ROLL;
            sausage64_set_anim(&entity->model, ANIMATION_nick_stand_to_roll_left);

            break;
        }

        case WALKING: {

            entity->target_speed[0] = 140 * sinf(rad(entity->yaw));
            entity->target_speed[1] = 140 * -cosf(rad(entity->yaw));

            entity->acceleration[0] = 6 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 6 * (entity->target_speed[1] - entity->speed[1]);
        
            if (entity->state == ROLL) return;
            
            entity->state = ROLL;
            sausage64_set_anim(&entity->model, ANIMATION_nick_run_to_roll_left);

            break;
        }

        case RUNNING: {

            entity->target_speed[0] = 280 * sinf(rad(entity->yaw));
            entity->target_speed[1] = 280 * -cosf(rad(entity->yaw));

            entity->acceleration[0] = 16 * (entity->target_speed[0] - entity->speed[0]);
            entity->acceleration[1] = 16 * (entity->target_speed[1] - entity->speed[1]);

            if (entity->state == ROLL) return;
            
            entity->state = ROLL;
            sausage64_set_anim(&entity->model, ANIMATION_nick_run_to_roll_left);
            break;
        }
    }
}

#endif