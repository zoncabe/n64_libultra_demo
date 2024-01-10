#ifndef VIEWPORT_H
#define VIEWPORT_H

/* VIEWPORT.H
here are all the structures and functions prototypes that involve the setting up of the viewport */


// structures

typedef struct {
	
	Mtx modeling;

	u16 normal;
	Mtx projection;
	Mtx position_mtx;

	float distance_from_target;
	float horizontal_distance_from_target;
	float vertical_distance_from_target;
	float angle_around_target;

	float position[3];
	float target[3];
	float pitch;
	float yaw;
	float roll;
	
} Viewport;

typedef struct{

    Light amb;
    Light dir;
	float angle[3];
	
}LightData;


// functions prototypes

void move_viewport_stick(Viewport *viewport, NUContData cont[1]);

void set_viewport_position(Viewport *viewport, Entity entity);



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/* move_viewport_stick
changes the viewport variables depending on controller input*/

void move_viewport_stick(Viewport *viewport, NUContData cont[1])
{
    if (fabs(cont->stick_x) < 7) cont->stick_x = 0;
    if (fabs(cont->stick_y) < 7) cont->stick_y = 0;

    viewport->angle_around_target += cont->stick_x / 20;
    viewport->pitch += cont->stick_y / 20;

    if (viewport->angle_around_target > 360) {viewport->angle_around_target  = viewport->angle_around_target - 360;}
    if (viewport->angle_around_target < 0) {viewport->angle_around_target  = viewport->angle_around_target + 360;}

    if (viewport->pitch > 85) viewport->pitch = 85;
    if (viewport->pitch < -85) viewport->pitch = -85;
}

/*set_viewport_position
calculates the viewport position given the input controlled variables*/

void set_viewport_position(Viewport *viewport, Entity target)
{
    viewport->horizontal_distance_from_target = viewport->distance_from_target * cosf(rad(viewport->pitch));
	viewport->vertical_distance_from_target = viewport->distance_from_target * sinf(rad(viewport->pitch));

    viewport->position[0] = target.position[0] - viewport->horizontal_distance_from_target * sinf(rad(viewport->angle_around_target));
    viewport->position[1] = target.position[1] - viewport->horizontal_distance_from_target * cosf(rad(viewport->angle_around_target));
    viewport->position[2] = viewport->vertical_distance_from_target + target.position[2];
	
	viewport->target[0] = target.position[0];
	viewport->target[1] = target.position[1];
	viewport->target[2] = target.position[2] + 70;

    //this makes the camera collide with an horizontal plane at height 0 simulating the floor
    //will be removed when camera collision happens
	if (viewport->position[2] < 0)  viewport->position[2] = 0;
}


#endif
