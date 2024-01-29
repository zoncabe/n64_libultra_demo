#ifndef COLLISION_H
#define COLLISION_H

/* COLLISION.H
here are all the structures and functions prototypes that involve the collision detection */


// structures

typedef unsigned int uint32;
#define in(a) (*((uint32*)&a))

/*
typedef enum {

    SPHERE,
    AABB,
    OBB,
    CAPSULE

} BoundingShapeType;
*/


typedef struct {

    float center[3];
    float radius;

} Sphere;


typedef struct {
    
    float min[3];
    float max[3];
    float center[3];

} AABB;


typedef struct {

    float startPoint[3];
    float endPoint[3];
    float radius;

} Capsule;


typedef struct {

    float center[3];
    float size[3];
    float rotation[3];

} OBB;



// function prototypes

void set_point(float* dest, float* src);
float squared_distance(float *a, float *b);
float distance(float *a, float *b);

void set_vector(float *vector, float *a, float *b);
float squared_length(const float* vector);
float length(const float* vector);



/* set_point
sets values from a source point to a destinatary */
void set_point(float* dest, float* src) 
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}


/* distance
returns the squared distance between 2 points */
float squared_distance(float *a, float *b) 
{
    return (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2]);
}


/* distance
returns the distance between 2 points*/
float distance(float* a, float* b) 
{
    return 1 / qi_sqrt(squared_distance(a, b));
}


/* set_vector
sets a vector given 2 points*/
void set_vector(float *vector, float *a, float *b) 
{
    vector[0] = b[0] - a[0];
    vector[1] = b[1] - a[1];
    vector[1] = b[2] - a[2];
}


/* squared_lenght
returns the squared lenght of a vector */
float squared_length(const float* vector) {
    return vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
}


/* lenght
returns the lenght of a vector */
float length(const float* vector) {
    return 1 / qi_sqrt(squared_length(vector));
}


/* collision_sphere_sphere
detects collision between 2 shperes */
int collision_sphere_sphere(Sphere sphere1, Sphere sphere2) 
{
    float distance_squared = squared_distance(sphere1.center, sphere2.center);
    float radius_sum = sphere1.radius + sphere2.radius;
    return distance_squared <= radius_sum * radius_sum ? 1 : 0;
}


/* collision_aabb_aabb
detects collision between 2 boxes */
int collision_aabb_aabb(AABB aabb1, AABB aabb2) 
{
    if (aabb1.max[0] < aabb2.min[0] || aabb1.min[0] > aabb2.max[0])
        return 0;

    if (aabb1.max[1] < aabb2.min[1] || aabb1.min[1] > aabb2.max[1])
        return 0;

    if (aabb1.max[2] < aabb2.min[2] || aabb1.min[2] > aabb2.max[2])
        return 0;

    return 1;
}


/* collision_aabb_aabb
detects collision between a shpere and a box */
int collision_sphere_aabb(Sphere sphere, AABB aabb) 
{
    // Find the closest point on the AABB to the sphere's center
    float closest_point[3];

    for (int i = 0; i < 3; i++) {
        closest_point[i] = max(aabb.min[i], min(sphere.center[i], aabb.max[i]));
    }

    // Calculate the squared distance from this point to the sphere's center
    float distance_squared = squared_distance(sphere.center, closest_point);

    // Check if the distance is less than or equal to the sphere's radius squared
    return distance_squared <= sphere.radius * sphere.radius ? 1 : 0;
}


/* collision_capsule_sphere
detects collision between a capsule and a sphere */
int collision_capsule_sphere(Capsule capsule, Sphere sphere) 
{
    float capsule_vector[3];
    set_vector(capsule_vector, capsule.startPoint, capsule.endPoint);

    float sphere_to_start[3];
    set_vector(sphere_to_start, capsule.startPoint, sphere.center);

    float t = dot_product(sphere_to_start, capsule_vector) / squared_distance(capsule.startPoint, capsule.endPoint);
    t = max(0, min(1, t));

    float closest_point[3] = {
        capsule.startPoint[0] + t * capsule_vector[0],
        capsule.startPoint[1] + t * capsule_vector[1],
        capsule.startPoint[2] + t * capsule_vector[2]
    };

    float distance_squared = squared_distance(sphere.center, closest_point);
    float radius_sum = capsule.radius + sphere.radius;
    return distance_squared <= radius_sum * radius_sum ? 1 : 0;
}


#endif