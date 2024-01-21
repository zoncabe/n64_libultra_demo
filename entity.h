#ifndef ENTITY_H
#define ENTITY_H

/* ENTITY.H
here are all the structures and functions prototypes that involve the setting up of an entity */


// structures

typedef enum {
	EMPTY,
    IDLE,
    WALKING,
    RUNNING,
	ROLL,
	JUMP,

} EntityState;

typedef enum {
	LEFT,
    RIGHT,

} Foot;

typedef struct {

	Mtx	position_mtx;
	Mtx	rotation_mtx[3];
	Mtx scale_mtx;

	EntityState previous_state;
	EntityState state;

	int A_press;
	int B_press;

	float walk_target_speed;
	float run_target_speed;
	float idle_to_roll_target_speed;
	float walk_to_roll_target_speed;
	float run_to_roll_target_speed;
	
	int idle_to_roll_change_grip_tick;
	int walk_to_roll_change_grip_tick;
	int run_to_roll_change_grip_tick;
	int walk_grounded_foot_change_tick;
	int run_grounded_foot_change_tick;

	Foot grounded_foot;

	float scale;
	float position[3];
	float pitch;

	float target_yaw;
	float yaw;
    
	float acceleration[3];
	float target_speed[3];
	float speed[3];

	s64ModelHelper model;

    float framerate;

	float input_amount;
	float input_x;
	float input_y;

	float directional_speed;

	Mtx model_mtx[];

} Entity;



// functions prototypes

//void init_entity(Entity *entity, int idle, Mtx *entityMtx, void (*animcallback)(u16));
void set_entity_position(Entity *entity, TimeData timedata);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/* init_entity
this is a not working pototype function that i left behind because its driving me crazy

void init_entity(Entity *entity, int idle, Mtx *entityMtx, void (*animcallback)(u16))
{
    sausage64_initmodel(&entity->model, entity->model.mdldata, entityMtx);
    sausage64_set_anim(&entity->model, idle); 
    sausage64_set_animcallback(&entity->model, animcallback);
}
*/


/* set_entity_position
calculates the entity position given the speed and the available frame duration*/

void set_entity_position(Entity *entity, TimeData timedata)
{
    entity->speed[0] += (entity->acceleration[0] * timedata.frame_duration);
    entity->speed[1] += (entity->acceleration[1] * timedata.frame_duration);
    entity->speed[2] += (entity->acceleration[2] * timedata.frame_duration);

    if (entity->speed[0] != 0 || entity->speed[1] != 0) {

		// set yaw based on speed
		entity->yaw = deg(atan2(entity->speed[0], -entity->speed[1]));
		// debug data collecting
		//entity->directional_speed = calculate_diagonal(entity->speed[0], entity->speed[1]);
	}

    entity->position[0] += entity->speed[0] * timedata.frame_duration;
    entity->position[1] += entity->speed[1] * timedata.frame_duration;
    entity->position[2] += entity->speed[2] * timedata.frame_duration;

}


#endif
