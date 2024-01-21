#ifndef VIEWPORTSTATES_H
#define VIEWPORTSTATES_H

/* VIEWPORTSTATES.H
here are all the viewport state machine related functions */

// function prototypes

void set_viewport_state(Viewport *viewport, ViewportState new_state);

void set_third_person_shooter_gameplay_viewport(Viewport *viewport);

void set_third_person_shooter_aiming_viewport(Viewport *viewport);

void set_third_person_shooter_looking_viewport(Viewport *viewport);


void set_viewport_state(Viewport *viewport, ViewportState new_state) 
{
    switch(new_state) {

        case THIRD_PERSON_SHOOTER_GAMEPLAY: {
            set_third_person_shooter_gameplay_viewport(viewport);
            break;
        }
        case THIRD_PERSON_SHOOTER_LOOKING: {
            set_third_person_shooter_aiming_viewport(viewport);
            break;
        }
        case THIRD_PERSON_SHOOTER_AIMING: {
            set_third_person_shooter_looking_viewport(viewport);
            break;
        }
    }
}

void set_third_person_shooter_gameplay_viewport(Viewport *viewport)
{
    viewport->rotational_acceleration[0] = viewport->acceleration_settings[0] * (viewport->rotational_target_speed[0] - viewport->rotational_speed[0]);
    viewport->rotational_acceleration[1] = viewport->acceleration_settings[0] * (viewport->rotational_target_speed[1] - viewport->rotational_speed[1]);
}


void set_third_person_shooter_aiming_viewport(Viewport *viewport){}


void set_third_person_shooter_looking_viewport(Viewport *viewport){}


#endif