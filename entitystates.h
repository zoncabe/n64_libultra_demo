#ifndef ENTITYSTATE_H
#define ENTITYSTATE_H

/* ENTITYSTATE.H
here are all the state machine related functions */

//variables




// function prototypes

void set_entity_state(Entity *entity, EntityState new_state);

void set_idle_state(Entity *entity);

void set_walking_state(Entity *entity);
            
void set_running_state(Entity *entity);



void set_entity_state(Entity * entity, EntityState new_state) 
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
    }
}


void set_idle_state(Entity *entity)
{
    if (entity->previous_state == IDLE) return;
    
    sausage64_set_anim(&entity->model, ANIMATION_nick_look_around_left);
    entity->previous_state = IDLE;
}


void set_walking_state(Entity *entity)
{
    entity->target_speed[0] = 80 * sinf(rad(entity->target_yaw));
    entity->target_speed[1] = 80 * -cosf(rad(entity->target_yaw));

    entity->acceleration[0] = 4 * (entity->target_speed[0] - entity->speed[0]);
    entity->acceleration[1] = 4 * (entity->target_speed[1] - entity->speed[1]);

    if (entity->previous_state == WALKING) return;
    
    sausage64_set_anim(&entity->model, ANIMATION_nick_walk_left);

    entity->previous_state = WALKING;
}


void set_running_state(Entity *entity)
{
    entity->target_speed[0] = 300 * sinf(rad(entity->target_yaw));
    entity->target_speed[1] = 300 * -cosf(rad(entity->target_yaw));

    entity->acceleration[0] = 4 * (entity->target_speed[0] - entity->speed[0]);
    entity->acceleration[1] = 4 * (entity->target_speed[1] - entity->speed[1]);

    if (entity->previous_state == RUNNING) return;
    
    sausage64_set_anim(&entity->model, ANIMATION_nick_run_left);

    entity->previous_state = RUNNING;
}

#endif