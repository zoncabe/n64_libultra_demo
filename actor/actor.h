#ifndef ACTOR_H
#define ACTOR_H

/* ACTOR.H
here are all the structures and functions prototypes that involve the setting up of an actor */


// structures

typedef enum {

	EMPTY,
    STAND_IDLE,
    WALKING,
    RUNNING,
	SPRINTING,
	ROLL,
	JUMP,
	FALLING,

} ActorState;


typedef struct {

	float idle_acceleration_rate;
	float walk_acceleration_rate;
	float run_acceleration_rate;
	float roll_acceleration_rate;
	float roll_acceleration_grip_rate;
	float jump_acceleration_rate;
	float aerial_control_rate;

	float walk_target_speed;
	float run_target_speed;
	float sprint_target_speed;
	float idle_to_roll_target_speed;
	float idle_to_roll_grip_target_speed;
	float walk_to_roll_target_speed;
	float run_to_roll_target_speed;
	float sprint_to_roll_target_speed;
	float jump_target_speed;
	float fall_max_speed;
	
	float jump_timer_max;

} ActorSettings;

typedef struct {

	float stick_magnitude;
	float stick_x;
	float stick_y;
	float time_held;
	bool jump_hold;
	bool jump_released;

} Actorinput;


typedef struct {

	Mtx	position_mtx;
	Mtx	rotation_mtx[3];
	Mtx scale_mtx;
	
	Gfx *mesh;
	Vector3 scale;
	
	RigidBody body;
	
	Vector3 target_rotation;
	Vector3 target_velocity;

	float horizontal_speed;
	float jumping_speed;

	bool grounded;
	float grounding_height;

	ActorState previous_state;
	ActorState state;

	ActorSettings settings;
	Actorinput input;

} Actor;


void actor_set_dl(Actor *actor);
void actor_animate(Actor *actor);


//small script to lean the capsule towards the velocity vector
void actor_animate(Actor *actor)
{
	if (actor->horizontal_speed > 1) actor->body.rotation.x = actor->horizontal_speed * 0.01f;
	else actor->body.rotation.x = 0;
}


/* actor_det_dl
handles the system functions that enters an actor's position and rotation values */

void actor_set_dl(Actor* actor)
{	
    guTranslate(&actor->position_mtx, actor->body.position.x, actor->body.position.y, actor->body.position.z);
    guRotate(&actor->rotation_mtx[1], actor->body.rotation.y, 0, 1, 0);
    guRotate(&actor->rotation_mtx[2], actor->body.rotation.z, 0, 0, 1);
    guRotate(&actor->rotation_mtx[0], actor->body.rotation.x, 1, 0, 0);
    guScale(&actor->scale_mtx, actor->scale.x, actor->scale.y, actor->scale.z);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&actor->position_mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&actor->rotation_mtx[1]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&actor->rotation_mtx[2]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&actor->rotation_mtx[0]), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&actor->scale_mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

    gSPDisplayList(glistp++, actor->mesh);

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
}

#endif