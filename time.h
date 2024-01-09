#ifndef TIME_H
#define TIME_H

typedef struct{
	OSTime cur_frame;
	OSTime last_frame;
	float frame_duration;
	f32 FPS;
}TimeData;

float get_time();
float cycles_to_sec(OSTime cycles);
void time_management(TimeData *time);


/*==============================================================
    get_time
    returns time in seconds
==============================================================*/

float get_time(){

    float time = (s32)OS_CYCLES_TO_USEC(osGetTime()) / 1000000.0f;
    return time;
}


/*==============================================================
    cycles_to_sec
    converts cycles to seconds
==============================================================*/

float cycles_to_sec(OSTime cycles){

    float time = (s32)OS_CYCLES_TO_USEC(cycles) / 1000000.0f;
    return time;
}


/*==============================================================
    time_management
    calculates FPS and frame_duration variable    
==============================================================*/

void time_management(TimeData *time){

    time->cur_frame = osGetTime();

    time->frame_duration = cycles_to_sec(time->cur_frame - time->last_frame);

    time->FPS = 1 / time->frame_duration;

    time->last_frame = time->cur_frame;

}


#endif