#ifndef MATH_UTIL_H
#define MATH_UTIL_H



float max(float a, float b);
float min(float a, float b);

float rad(float angle);
float deg(float rad);

float dot(float *a, float *b);
float qi_sqrt(float number);

float calculate_diagonal(float a, float b);



/* max
return higher value*/
float max(float a, float b) 
{
    if (a > b) return a; else return b;
}


/* min
return lower value*/
float min(float a, float b) 
{
    if (a < b) return a; else return b;
}

  
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


/*dot
returns dot product*/
float dot(float *a, float *b) 
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; 
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

float calculate_diagonal(float a, float b){
    float diagonal = 1 / qi_sqrt(a * a + b * b);
    return diagonal;
}

#endif