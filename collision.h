#ifndef COLLISION_H
#define COLLISION_H

/* COLLISION.H
here are all the structures and functions prototypes that involve the collision detection */


// structures
typedef struct {
    float equation[4];
    float origin[3];
    float normal[3];
} Plane;


// function prototypes

void set_point(float* dest, float* src);
void set_vector(float *vector, float *a, float *b);
void set_plane(Plane* plane, float origin[3], float normal[3]);

float distance_2d(float* a, float* b);
float distance(float* a, float* b);
float distance_squared(float *a, float *b);
float distance_point_and_line(float *a, float *b, float *c);

void init_plane(Plane* plane, float* origin, float* normal);

void init_plane_from_triangle(Plane* plane, float a[3], float b[3], float c[3]);


/* set_point
sets values from a source point to a destinatary*/
void set_point(float* dest, float* src) 
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}


/* set_vector
sets a vector given 2 points*/
void set_vector(float *vector, float *a, float *b) 
{
    vector[0] = b[0] - a[0];
    vector[1] = b[1] - a[1];
    vector[1] = b[2] - a[2];
}


/* set_plane_from_vector
sets a plane given an origin point and a vector*/
void set_plane_from_vector(Plane* plane, float origin[3], float normal[3])
{
    set_point(plane->normal, normal);
    set_point(plane->origin, origin);
    set_point(plane->equation, normal);
    plane->equation[3] = -dot(normal, origin);
}


void set_plane_from_triangle(Plane *plane, float *a, float *b, float *c)
{
    float *vec1;
    float *vec2;

    set_vector(vec1, a, b);
    set_vector(vec2, a, c);

    plane->normal[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    plane->normal[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    plane->normal[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

    // Normalize the normal float

    float length = qi_sqrt(dot(plane->normal, plane->normal));

    plane->normal[0] /= length;
    plane->normal[1] /= length;
    plane->normal[2] /= length;

    set_point(plane->origin, a);

    set_point(plane->equation, plane->normal);
    plane->equation[3] = -dot(plane->normal, plane->origin);
}
/*
*/


float distance_2d(float* a, float* b) 
{
    return 1 / qi_sqrt((a[0] - b[0]) * (a[0] - b[0])+ (a[1] - b[1]) * (a[1] - b[1]) );
}


float distance(float* a, float* b) 
{
    return 1 / qi_sqrt((a[0] - b[0]) * (a[0] - b[0])+ (a[1] - b[1]) * (a[1] - b[1])+ (a[2] - b[2]) * (a[2] - b[2]));
}


float distance_squared(float *a, float *b) 
{
    return (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]);
}


// Return minimum distance between line segment ab and point c

float distance_point_and_line(float *a, float *b, float *c) 
{
 float l2 = distance_squared(a, b);

    float c_a[3];
    float b_a[3];
    float projection[3];
    
    if (l2 == 0.0) return distance_2d(c, a);
    
    c_a[0] = c[0] - a[0];
    c_a[1] = c[1] - a[1];
    b_a[0] = b[0] - a[0];
    b_a[1] = b[1] - a[1];

 float t = max(0, min(1, dot(c_a, b_a) / l2));

    projection[0] = a[0] + t * (b[0] - a[0]);
    projection[1] = a[1] + t * (b[1] - a[1]);
    
    return distance_2d(c, projection);
}


float distance_to_plane (Plane *plane, float* point) 
{
    return (point[0] * plane->normal[0] + point[1] * plane->normal[1] + point[2] * plane->normal[2]) + plane->equation[3];
}


int is_plane_front_facing_to (Plane *plane, float *direction)
{
    return (0 >= dot(plane->normal, direction));
}


int collision_point_and_circle(float point_position[3], float circle_center[3], float radius) 
{

    return radius >= distance(point_position, circle_center);
}


// https://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not/37865332#37865332
// currently only looks at x and y, despite passing in 3d points

int collision_point_and_rectangle(float point_position[3], float rectangle_position[3], float width, float depth) 
{
    float A[3], B[3], C[3], D[3], AB[3], AM[3], BC[3], BM[3];

    A[0] = rectangle_position[0] + width / 2;
    A[1] = rectangle_position[1] + depth / 2;
    
    B[0] = rectangle_position[0] - width / 2;
    B[1] = rectangle_position[1] + depth / 2;
    
    C[0] = rectangle_position[0] - width / 2;
    C[1] = rectangle_position[1] - depth / 2;
    
    D[0] = rectangle_position[0] + width / 2;
    D[1] = rectangle_position[1] - depth / 2;

    set_vector(AB, A, B);
    set_vector(AM, A, point_position);
    set_vector(BC, B, C);
    set_vector(BM, B, point_position);

    float dotABAM = dot(AB, AM);
    float dotABAB = dot(AB, AB);
    float dotBCBM = dot(BC, BM);
    float dotBCBC = dot(BC, BC);

    return 0 <= dotABAM && dotABAM <= dotABAB && 0 <= dotBCBM && dotBCBM <= dotBCBC;
}



/*
void detect_collisions(Entity *entity, Scenery object, TimeData timedata) 
{
    if ( collision_point_and_rectangle(entity->position, object.position, 100, 100)) {

        float A[3]; float B[3]; float C[3]; float D[3];

        int width = 100;
        int depth = 100;

        A[0] = entity->position[0] + width / 2;
        A[1] = entity->position[1] + depth / 2;
        
        B[0] = entity->position[0] - width / 2;
        B[1] = entity->position[1] + depth / 2;
        
        C[0] = entity->position[0] - width / 2;
        C[1] = entity->position[1] - depth / 2;
        
        D[0] = entity->position[0] + width / 2;
        D[1] = entity->position[1] - depth / 2;

        min_dist_to_wall_old =

        min(minimum_distance(A, D, entity->position),
            min(minimum_distance(C, D, entity->position),
                min(minimum_distance(A, B, entity->position), 
                    minimum_distance(B, C, entity->position)
                )
            )
        );

        min_dist_to_wall_new = 
        
        min(minimum_distance(A, D, new_pos),
            min(minimum_distance(C, D, new_pos),
                min(minimum_distance(A, B, new_pos), 
                    minimum_distance(B, C, new_pos)
                )
            )
        );

        if (collision_point_and_rectangle(new_pos, object.position, 500, 500))
        {
            if (min_dist_to_wall_new != min_dist_to_wall_old) {
                
                entity.speed[0] = 0;
                entity.speed[1] = 0;
            }
        }
    }
}
*/



#endif