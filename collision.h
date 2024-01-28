#ifndef COLLISION_H
#define COLLISION_H

/* COLLISION.H
here are all the structures and functions prototypes that involve the collision detection */


// structures
typedef struct {
    float origin[3];
    float normal[3];
    float equation[4];
} Plane;

typedef struct {
    
    float e_radius[3]; // ellipsoid radius
    float r3_velocity[3];
    float r3_position[3];
    float velocity[3];
    float normalized_velocity[3];
    float base_point[3];
    int found_collision;
    double nearest_distance;
    float intersection_point[3];

} CollisionPackage;


// function prototypes

void set_point(float* dest, float* src);
void set_vector(float *vector, float *a, float *b);
void set_plane_from_normal(Plane* plane, float origin[3], float normal[3]);
void set_plane_from_triangle(Plane *plane, float *a, float *b, float *c);
float signed_distance_point_to_plane (Plane *plane, float* point);
int is_plane_facing_velocity(Plane *plane, float *direction);






float distance(float *a, float *b);
float distance_squared(float *a, float *b);
float distance_point_and_line(float *a, float *b, float *c);


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
#include <math.h>

float squared_length(const float* vector) {
    return vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
}

float length(const float* vector) {
    return 1 / qi_sqrt(squared_length(vector));
}



/* set_plane_from_normal
sets a plane given an origin point and a vector*/
void set_plane_from_normal(Plane* plane, float origin[3], float normal[3])
{
    set_point(plane->normal, normal);
    set_point(plane->origin, origin);
    set_point(plane->equation, normal);
    plane->equation[3] = -dot_product(normal, origin);
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

    float length = 1 / qi_sqrt(dot_product (plane->normal, plane->normal));

    plane->normal[0] /= length;
    plane->normal[1] /= length;
    plane->normal[2] /= length;

    set_point(plane->origin, a);

    set_point(plane->equation, plane->normal);
    plane->equation[3] = -dot_product(plane->normal, plane->origin);
}


float signed_distance_point_to_plane (Plane *plane, float* point) 
{
    return (point[0] * plane->normal[0] + point[1] * plane->normal[1] + point[2] * plane->normal[2]) + plane->equation[3];
}


int is_plane_facing_velocity(Plane *plane, float *direction)
{
    return (0 >= dot_product(plane->normal, direction));
}


typedef unsigned int uint32;

#define in(a) (*((uint32*)&a))

/* collision_point_and_triangle
Check if a point is inside a triangle in 3D space */
float collision_point_and_triangle(const float point[3], const float pa[3], const float pb[3], const float pc[3]) 
{
    float e10[3] = {pb[0] - pa[0], pb[1] - pa[1], pb[2] - pa[2]};
    float e20[3] = {pc[0] - pa[0], pc[1] - pa[1], pc[2] - pa[2]};
    float a = e10[0] * e10[0] + e10[1] * e10[1] + e10[2] * e10[2];
    float b = e10[0] * e20[0] + e10[1] * e20[1] + e10[2] * e20[2];
    float c = e20[0] * e20[0] + e20[1] * e20[1] + e20[2] * e20[2];
    float ac_bb = (a * c) - (b * b);
    float vp[3] = {point[0] - pa[0], point[1] - pa[1], point[2] - pa[2]};
    float d = vp[0] * e10[0] + vp[1] * e10[1] + vp[2] * e10[2];
    float e = vp[0] * e20[0] + vp[1] * e20[1] + vp[2] * e20[2];
    float x = (d * c) - (e * b);
    float y = (e * a) - (d * b);
    float z = x + y - ac_bb;
    return ((in(z) & ~(in(x) | in(y))) & 0x80000000);
}



// Check if a solution exists
int get_lowest_root(float *root, float max_r, float a, float b, float c) 
{

    float determinant = b * b - 4.0f * a * c;

    // If determinant is negative, it means no solutions.
    if (determinant < 0.0f) return 0; 
    

    // Calculate the two roots: (if determinant == 0 then
    // x1==x2, but let’s disregard that slight optimization)
    float sqrt_d = 1 / qi_sqrt(determinant);
    float r1 = (-b - sqrt_d) / (2 * a);
    float r2 = (-b + sqrt_d) / (2 * a);

    // Sort so x1 <= x2
    if (r1 > r2) {
        float temp = r2;
        r2 = r1;
        r1 = temp;
    }

    // Get lowest root:
    if (r1 > 0 && r1 < max_r) {
        *root = r1;
        return 1; // True, a valid solution was found.
    }

    // It is possible that we want x2 - this can happen
    // if x1 < 0
    if (r2 > 0 && r2 < max_r) {
        *root = r2;
        return 1; // True, a valid solution was found.
    }

    // No (valid) solutions
    return 0; // False, no valid solutions.
}


void check_triangle(CollisionPackage *collision_package, float* p1, float* p2, float* p3)
{
    // Make the plane containing this triangle.
    Plane triangle_plane;
    set_plane_from_triangle(&triangle_plane, p1, p2, p3);

    // Is the triangle front-facing to the velocity vector?
    if (is_plane_facing_velocity(&triangle_plane, collision_package->normalized_velocity)) {
        // Get interval of plane intersection:
        double t0, t1;
        int embedded_in_plane = 0;

        // Calculate the signed distance from the ellipsoid position to the triangle plane
        double signed_dist_to_triangle_plane = signed_distance_point_to_plane(&triangle_plane, collision_package->base_point);

        // Cache this as we’re going to use it a few times below:
        float normal_dot_velocity = dot_product(triangle_plane.normal, collision_package->velocity);

        // If the ellipsoid is traveling parallel to the plane:
        if (normal_dot_velocity == 0.0f) {
            if (fabs(signed_dist_to_triangle_plane) >= 1.0f) {
                // The ellipsoid is not embedded in the plane.
                // No collision possible.
                return;
            } else {
                // The ellipsoid is embedded in the plane.
                // It intersects in the whole range [0..1]
                embedded_in_plane = 1;
                t0 = 0.0;
                t1 = 1.0;
            }
        } else {
            // N dot D is not 0. Calculate the intersection interval:
            t0 = (-1.0 - signed_dist_to_triangle_plane) / normal_dot_velocity;
            t1 = (1.0 - signed_dist_to_triangle_plane) / normal_dot_velocity;

            // Swap so t0 < t1
            if (t0 > t1) {
                double temp = t1;
                t1 = t0;
                t0 = temp;
            }

            // Check that at least one result is within range:
            if (t0 > 1.0f || t1 < 0.0f) {
                // Both t values are outside values [0,1].
                // No collision possible.
                return;
            }

            // Clamp to [0,1]
            t0 = (t0 < 0.0) ? 0.0 : t0;
            t1 = (t1 < 0.0) ? 0.0 : t1;
            t0 = (t0 > 1.0) ? 1.0 : t0;
            t1 = (t1 > 1.0) ? 1.0 : t1;
        }

        // At this point, we have two time values t0 and t1
        // between which the swept ellipsoid intersects with the
        // triangle plane. If any collision is to occur, it must
        // happen within this interval.
        float collision_point[3];
        int found_collision = 0;
        float t = 1.0;

        // First, check for the easy case - collision inside
        // the triangle. If this happens, it must be at time t0
        // as this is when the ellipsoid rests on the front side
        // of the triangle plane. Note, this can only happen if
        // the ellipsoid is not embedded in the triangle plane.
        if (!embedded_in_plane) {

            float *plane_intersection_point;
            subtract_vectors(plane_intersection_point, collision_package->base_point, triangle_plane.normal);
            add_scaled_vector(plane_intersection_point, plane_intersection_point, collision_package->velocity, t0);

            if (collision_point_and_triangle(plane_intersection_point, p1, p2, p3)) {
                found_collision = 1;
                t = t0;
                set_point(collision_point, plane_intersection_point);
            }
        }

        // If we haven’t found a collision already, sweep the ellipsoid against points and edges of the triangle.
        // Note: A collision inside the triangle (the check above) will always happen before a vertex or edge collision!
        // This is why we can skip the swept test if the above gives a collision!
        if (!found_collision) {

            float *substracted_vector;
            float velocity_squared_length = squared_length(collision_package->velocity);
            float a, b, c; // Params for equation
            float new_t;

            // For each vertex or edge, a quadratic equation has to
            // be solved. We parameterize this equation as
            // a*t^2 + b*t + c = 0, and below we calculate the
            // parameters a, b, and c for each test.

            // Check against points:

            // P1
            a = velocity_squared_length;
            subtract_vectors(substracted_vector, collision_package->base_point, p1);
            b = 2.0 * dot_product(collision_package->velocity, substracted_vector);
            subtract_vectors(substracted_vector, p1, collision_package->base_point);
            c = squared_length(substracted_vector) - 1.0;
            if (get_lowest_root(&new_t, a, b, c, t)) {
                t = new_t;
                found_collision = 1;
                set_point(collision_point, p1);
            }

            // P2
            subtract_vectors(substracted_vector, collision_package->base_point, p2);
            b = 2.0 * dot_product(collision_package->velocity, substracted_vector);
            subtract_vectors(substracted_vector, p2, collision_package->base_point);
            c = squared_length(substracted_vector) - 1.0;

            if (get_lowest_root(&new_t, a, b, c, t)) {
                t = new_t;
                found_collision = 1;
                set_point(collision_point, p2);
            }

            // P3
            subtract_vectors(substracted_vector, collision_package->base_point, p3);
            b = 2.0 * dot_product(collision_package->velocity, substracted_vector);
            subtract_vectors(substracted_vector, p3, collision_package->base_point);
            c = squared_length(substracted_vector) - 1.0;
            if (get_lowest_root(&new_t, a, b, c, t)) {
                t = new_t;
                found_collision = 1;
                set_point(collision_point, p3);
            }

            // Check against edges:

            // p1 -> p2:
            float edge[3];
            subtract_vectors(edge, p2, p1);
            float base_to_vertex[3];
            subtract_vectors(base_to_vertex, p1, collision_package->base_point);
            float edge_squared_length = squared_length(edge);
            float edge_dot_velocity = dot_product(edge, collision_package->velocity);
            float edge_dot_base_to_vertex = dot_product(edge, base_to_vertex);

            // Calculate parameters for equation
            a = edge_squared_length * -velocity_squared_length + edge_dot_velocity * edge_dot_velocity;
            b = edge_squared_length * (2 * dot_product(collision_package->velocity, base_to_vertex)) -
                2.0 * edge_dot_velocity * edge_dot_base_to_vertex;
            c = edge_squared_length * (1 - squared_length(base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;

            // Does the swept ellipsoid collide against an infinite edge?
            if (get_lowest_root(&new_t, a, b, c, t)) {
                // Check if the intersection is within the line segment:
                float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_squared_length;
                if (f >= 0.0 && f <= 1.0) {
                    // Intersection took place within the segment.
                    t = new_t;
                    found_collision = 1;
                    add_scaled_vector(collision_point, p1, edge, f);
                }
            }

            // p2 -> p3:
            subtract_vectors(edge, p3, p2);
            subtract_vectors(base_to_vertex, p2, collision_package->base_point);
            edge_squared_length = squared_length(edge);
            edge_dot_velocity = dot_product(edge, collision_package->velocity);
            edge_dot_base_to_vertex = dot_product(edge, base_to_vertex);

            a = edge_squared_length * -velocity_squared_length + edge_dot_velocity * edge_dot_velocity;
            b = edge_squared_length * (2 * dot_product(collision_package->velocity, base_to_vertex)) -
                2.0 * edge_dot_velocity * edge_dot_base_to_vertex;
            c = edge_squared_length * (1 - squared_length(base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;

            if (get_lowest_root(&new_t, a, b, c, t)) {
                float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_squared_length;
                if (f >= 0.0 && f <= 1.0) {
                    t = new_t;
                    found_collision = 1;
                    add_scaled_vector(collision_point, p2, edge, f);
                }
            }

            // p3 -> p1:
            subtract_vectors(edge, p1, p3);
            subtract_vectors(base_to_vertex, p3, collision_package->base_point);
            edge_squared_length = squared_length(edge);
            edge_dot_velocity = dot_product(edge, collision_package->velocity);
            edge_dot_base_to_vertex = dot_product(edge, base_to_vertex);

            a = edge_squared_length * -velocity_squared_length + edge_dot_velocity * edge_dot_velocity;
            b = edge_squared_length * (2 * dot_product(collision_package->velocity, base_to_vertex)) -
                2.0 * edge_dot_velocity * edge_dot_base_to_vertex;
            c = edge_squared_length * (1 - squared_length(base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;

            if (get_lowest_root(&new_t, a, b, c, t)) {
                float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_squared_length;
                if (f >= 0.0 && f <= 1.0) {
                    t = new_t;
                    found_collision = 1;
                    add_scaled_vector(collision_point, p3, edge, f);
                }
            }
        }

        // Set result:
        if (found_collision) {
            // Distance to collision: ’t’ is the time of collision
            float dist_to_collision = t * length(collision_package->velocity);

            // Does this triangle qualify for the closest hit?
            // It does if it’s the first hit or the closest
            if (!collision_package->found_collision || dist_to_collision < collision_package->nearest_distance) {
                // Collision information necessary for sliding
                collision_package->nearest_distance = dist_to_collision;
                set_point(collision_package->intersection_point, collision_point);
                collision_package->found_collision = 1;
            }
        }
    } // if not backface
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
    
    if (l2 == 0.0) return distance(c, a);
    
    c_a[0] = c[0] - a[0];
    c_a[1] = c[1] - a[1];
    b_a[0] = b[0] - a[0];
    b_a[1] = b[1] - a[1];

 float t = max(0, min(1, dot_product(c_a, b_a) / l2));

    projection[0] = a[0] + t * (b[0] - a[0]);
    projection[1] = a[1] + t * (b[1] - a[1]);
    
    return distance(c, projection);
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

    float dotABAM = dot_product(AB, AM);
    float dotABAB = dot_product(AB, AB);
    float dotBCBM = dot_product(BC, BM);
    float dotBCBC = dot_product(BC, BC);

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