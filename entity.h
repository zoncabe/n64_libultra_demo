#ifndef ENTITY_H
#define ENTITY_H


typedef struct {

	Mtx	pos_mtx;
	Mtx	rot_mtx[3];
	Mtx scale_mtx;

    float scale;
	float position[3];
	float pitch;

	float target_yaw;
	float yaw;
    
	float speed[3];

	s64ModelHelper model;

    float framerate;

} Entity;


void set_entity_position(Entity *entity, TimeData time_data);


void set_entity_position(Entity *entity, TimeData time_data){


    entity->position[0] += entity->speed[0] * time_data.frame_duration;
    entity->position[1] += entity->speed[1] * time_data.frame_duration;
    entity->position[2] += entity->speed[2] * time_data.frame_duration;

}

#endif
