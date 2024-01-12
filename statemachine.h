#ifndef STATEMACHINE_H
#define STATEMACHINE_H

/* STATEMACHINE.H
here are all the state machine related functions */


// function prototypes


void set_animation(Entity *entity)
{
    if (entity->state == JUMP) sausage64_set_anim(entity->helper, ANIMATION_nick_jump);
    if (entity->state == ROLL) sausage64_set_anim(entity->helper, ANIMATION_nick_roll);
    if (entity->state == FALLBACK) sausage64_set_anim(entity->helper, ANIMATION_nick_fallback);
    if (entity->state == FALL) sausage64_set_anim(entity->helper, ANIMATION_nick_fall);
    if (entity->state == MIDAIR) sausage64_set_anim(entity->helper, ANIMATION_nick_midair);
    if (entity->state == IDLE) sausage64_set_anim(entity->helper, ANIMATION_nick_idle);
    if (entity->state == WALK) sausage64_set_anim(entity->helper, ANIMATION_nick_walk);
    if (entity->state == RUN) sausage64_set_anim(entity->helper, ANIMATION_nick_run);
}




void set_entity_state(Entity * entity, entity_state new_state) 
{
    int curr_state = entity->state;

    if (curr_state == new_state) return;

    entity->state = new_state;

    set_animation(entity);

}





void handle_controller_input(NUContData cont[1], AnimatedEntity* entity){

    //cont[0].trigger & D_CBUTTONS, U_CBUTTONS
    if (cont[0].trigger & R_TRIG) {
    }
    if (cont[0].trigger & A_BUTTON) set_entity_state(entity, JUMP);
    if (cont[0].trigger & B_BUTTON) set_entity_state(entity, ROLL);
    if (entity->entity.speed > 900) {
        set_entity_state(entity, RUN);
    } else if (cont->stick_x != 0 || cont->stick_y != 0) {
        set_entity_state(entity, WALK);
    }

    if (cont->stick_x == 0 && cont->stick_y == 0
        && entity->entity.state != JUMP 
        && entity->entity.state != ROLL
        && entity->entity.state != FALLBACK
        && entity->entity.state != FALL 
        && entity->entity.state != MIDAIR 
        ) {
        set_entity_state(entity, IDLE);
    }

}