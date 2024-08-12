#ifndef TIME_H
#define TIME_H

/* TIME.H
here are all the structures and functions prototypes that involve the calculations for the timing */


// structures

typedef struct{

	OSTime current_frame;
	OSTime last_frame;
	float frame_time;
	f32 frame_rate;

}TimeData;


// functions prototypes

float time_getSeconds();
float cycles_to_sec(OSTime cycles);
void time_setData(TimeData *time);



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/* time_getSeconds
returns time in seconds */

float time_getSeconds()
{
    float time = (s32)OS_CYCLES_TO_USEC(osGetTime()) / 1000000.0f;
    return time;
}


/* cycles_to_sec
converts cycles to seconds */

float cycles_to_sec(OSTime cycles)
{
    float time = (s32)OS_CYCLES_TO_USEC(cycles) / 1000000.0f;
    return time;
}


/* time_init
sets all time data values to zero */
void time_init(TimeData *time)
{
    time->current_frame = 0.0f;

    time->frame_time = 0.0f;

    time->frame_rate = 0.0f;

    time->last_frame = 0.0f;
}


/* time_setData
calculates frame_rate and frame_time variable */

void time_setData(TimeData *time)
{
    time->current_frame = osGetTime();

    time->frame_time = cycles_to_sec(time->current_frame - time->last_frame);

    time->frame_rate = 1 / time->frame_time;

    time->last_frame = time->current_frame;
}


#endif