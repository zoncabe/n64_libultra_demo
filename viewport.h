#ifndef VIEWPORT_H
#define VIEWPORT_H

/* VIEWPORT.H
here are all the structures and functions prototypes that involve the setting up of the viewport */


// structures

typedef enum {
	INTRO,
	MENU,
	PAUSE,
    THIRD_PERSON_SHOOTER_GAMEPLAY,
	THIRD_PERSON_SHOOTER_LOOKING,
	THIRD_PERSON_SHOOTER_AIMING,

} ViewportState;

typedef struct {
	
	Mtx modeling;

	u16 normal;
	Mtx projection;
	Mtx position_mtx;

	float distance_from_entity;
	float horizontal_distance_from_entity;
	float vertical_distance_from_entity;

	float target_distance;
	float horizontal_target_distance;
	float vertical_target_distance;

	float angle_around_entity;
	float offset_angle;

	float rotational_acceleration [3];
	float acceleration_settings[2];

	float rotational_speed[3];
	float rotational_target_speed[3];
	
	float speed_settings[2];

	float position[3];
	float target[3];
	float pitch;
	float yaw;
	float roll;

	float height;
	
} Viewport;

typedef struct{

    Light amb;
    Light dir;
	float angle[3];
	
}LightData;


// functions prototypes

void set_viewport_position(Viewport *viewport, Entity entity, TimeData timedata);



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*set_viewport_position
calculates the viewport position given the input controlled variables*/

void set_viewport_position(Viewport *viewport, Entity entity, TimeData timedata)
{
	viewport->rotational_speed[0] += viewport->rotational_acceleration[0] * timedata.frame_duration;
    viewport->rotational_speed[1] += viewport->rotational_acceleration[1] * timedata.frame_duration;

	viewport->angle_around_entity += (viewport->rotational_speed[0] * timedata.frame_duration);
    viewport->pitch += (viewport->rotational_speed[1] * timedata.frame_duration);

	if (viewport->angle_around_entity > 360) viewport->angle_around_entity -= 360;
    if (viewport->angle_around_entity < 0) viewport->angle_around_entity  += 360;

    if (viewport->pitch > 70) viewport->pitch = 70;
    if (viewport->pitch < -70) viewport->pitch = -70;

	viewport->distance_from_entity = 250;

    viewport->horizontal_distance_from_entity = viewport->distance_from_entity * cosf(rad(viewport->pitch));
	viewport->vertical_distance_from_entity = viewport->distance_from_entity * sinf(rad(viewport->pitch));

	viewport-> horizontal_target_distance = viewport->target_distance * cosf(rad(viewport->pitch));
	viewport->vertical_target_distance = viewport->target_distance * sinf(rad(viewport->pitch + 180));

    viewport->position[0] = entity.position[0] - (viewport->horizontal_distance_from_entity * sinf(rad(viewport->angle_around_entity - viewport->offset_angle)));
    viewport->position[1] = entity.position[1] - (viewport->horizontal_distance_from_entity * cosf(rad(viewport->angle_around_entity - viewport->offset_angle)));
    viewport->position[2] = entity.position[2] + viewport->height + viewport->vertical_distance_from_entity;
	
    /* this makes the camera collide with an horizontal plane at height 0 simulating the floor
    will be modyfied when camera collision happens */
	while (viewport->position[2] < 0)  {

		viewport->distance_from_entity--; 
		viewport->horizontal_distance_from_entity = viewport->distance_from_entity * cosf(rad(viewport->pitch));
		viewport->vertical_distance_from_entity = viewport->distance_from_entity * sinf(rad(viewport->pitch));

		viewport->position[0] = entity.position[0] - viewport->horizontal_distance_from_entity * sinf(rad(viewport->angle_around_entity - viewport->offset_angle));
		viewport->position[1] = entity.position[1] - viewport->horizontal_distance_from_entity * cosf(rad(viewport->angle_around_entity - viewport->offset_angle));
		viewport->position[2] = entity.position[2] + viewport->height + viewport->vertical_distance_from_entity;
	}
	
	viewport->target[0] = entity.position[0] - viewport-> horizontal_target_distance * sinf(rad(viewport->angle_around_entity + 180));
	viewport->target[1] = entity.position[1] - viewport-> horizontal_target_distance * cosf(rad(viewport->angle_around_entity + 180));
	viewport->target[2] = entity.position[2] + viewport->height + viewport->vertical_target_distance;
}


#endif
