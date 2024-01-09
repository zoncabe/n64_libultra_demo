#ifndef VIEWPORT_H
#define VIEWPORT_H



// scene structs

typedef struct {
	
	Mtx modeling;

	u16 normal;
	Mtx projection;
	Mtx pos_mtx;

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
	
}LightData;

void move_viewport_stick(Viewport *viewport, NUContData cont[1]);

void set_viewport_position(Viewport *viewport, Entity entity);


void move_viewport_stick(Viewport *viewport, NUContData cont[1]){

    if (fabs(cont->stick_x) < 7) cont->stick_x = 0;
    if (fabs(cont->stick_y) < 7) cont->stick_y = 0;

    viewport->angle_around_target += cont->stick_x / 20;
    viewport->pitch += cont->stick_y / 20;

    if (viewport->angle_around_target > 360) {viewport->angle_around_target  = viewport->angle_around_target - 360;}
    if (viewport->angle_around_target < 0) {viewport->angle_around_target  = viewport->angle_around_target + 360;}

    if (viewport->pitch > 85) viewport->pitch = 85;
    if (viewport->pitch < -85) viewport->pitch = -85;
}


void set_viewport_position(Viewport *viewport, Entity entity){

    viewport->horizontal_distance_from_target = viewport->distance_from_target * cosf(rad(viewport->pitch));
	viewport->vertical_distance_from_target = viewport->distance_from_target * sinf(rad(viewport->pitch));

    viewport->position[0] = entity.position[0] - viewport->horizontal_distance_from_target * sinf(rad(viewport->angle_around_target));
    viewport->position[1] = entity.position[1] - viewport->horizontal_distance_from_target * cosf(rad(viewport->angle_around_target));
    viewport->position[2] = viewport->vertical_distance_from_target + entity.position[2];
	
	viewport->target[0] = entity.position[0];
	viewport->target[1] = entity.position[1];
	viewport->target[2] = entity.position[2] + 70;

}

#endif
