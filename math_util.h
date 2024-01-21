#ifndef MATH_UTIL_H
#define MATH_UTIL_H


float get_time();
float rad(float angle);
float deg(float rad);
float qi_sqrt(float number);
float calculate_diagonal(float x, float y);
int lim(u32 input);


  
/* rad
angle to radians*/

float rad(float angle){
	osSyncPrintf("1 rad\n");
	float radian = M_PI / 180 * angle;
	osSyncPrintf("2 rad\n");
	return radian;
}

/* deg
radians to angle*/

float deg(float rad){
	float angle = 180 / M_PI * rad;
	return angle;
}


/*qi_sqrt
quick inverse square root*/

float qi_sqrt(float number){

	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 ); 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}



/* calculate diagonal
 calculates diagonal of a rect triangle given its sides */

float calculate_diagonal(float x, float y){
    float diagonal = 1 / qi_sqrt(x * x +  y * y);
    return diagonal;
}


/* lim
 auxiliary function for 8 directional movement*/

int lim(u32 input){
    if (input == 0) {return 0;}
    else {return 1;}
}

#endif