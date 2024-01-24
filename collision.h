#ifndef COLLISION_H
#define COLLISION_H

/* COLLISION.H
here are all the structures and functions prototypes that involve the collision detection */


// the following code, at this point is jaltekruse's
// i plan to reshape it so i can understand it


void set_pt(float* dest, float* src) 
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}


float max(float a, float b) 
{
    if (a > b) return a;
    else return b;
}


float min(float a, float b) 
{
    if (a < b) return a;
    else return b;
}


float dot(float *u, float *v) 
{
    return u[0] * v[0] + u[1] * v[1]; 
}


void vector(float *dest, float *p1, float *p2) 
{
    dest[0] = p2[0] - p1[0];
    dest[1] = p2[1] - p1[1];
}


float distance_2d(float* pos1, float* pos2) 
{
    return 1 / Q_rsqrt( 
          (pos1[0] - pos2[0]) * (pos1[0] - pos2[0])
        + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1])
    );
}


float distance(float* pos1, float* pos2) 
{
    return 1 / Q_rsqrt( 
          (pos1[0] - pos2[0]) * (pos1[0] - pos2[0])
        + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1])
        + (pos1[2] - pos2[2]) * (pos1[2] - pos2[2])
    );
}


// length of a line segment in x and y directions
// this is only 2D for now, but takes 3d pts
float length_squared(float *pos1, float *pos2) 
{
    return
          (pos1[0] - pos2[0]) * (pos1[0] - pos2[0])
        + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]);
}



// https://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not/37865332#37865332
// currently only looks at x and y, despite passing in 3d points

int pt_in_rect(float* pos1, Entity *entity, int width, int depth) 
{
    float A[3]; float B[3]; float C[3]; float D[3];

    float AB[3];float AM[3];float BC[3];float BM[3];

    A[0] = entity->pos[0] + width / 2;
    A[1] = entity->pos[1] + depth / 2;
    
    B[0] = entity->pos[0] - width / 2;
    B[1] = entity->pos[1] + depth / 2;
    
    C[0] = entity->pos[0] - width / 2;
    C[1] = entity->pos[1] - depth / 2;
    
    D[0] = entity->pos[0] + width / 2;
    D[1] = entity->pos[1] - depth / 2;

    vector(AB, A, B);
    vector(AM, A, pos1);
    vector(BC, B, C);
    vector(BM, B, pos1);

    float dotABAM = dot(AB, AM);
    float dotABAB = dot(AB, AB);
    float dotBCBM = dot(BC, BM);
    float dotBCBC = dot(BC, BC);

    return 0 <= dotABAM && dotABAM <= dotABAB && 0 <= dotBCBM && dotBCBM <= dotBCBC;

    // javascript for reference from the link above
    /*
        function pointInRectangle(m, r) {
            var AB = vector(r.A, r.B);
            var AM = vector(r.A, m);
            var BC = vector(r.B, r.C);
            var BM = vector(r.B, m);
            var dotABAM = dot(AB, AM);
            var dotABAB = dot(AB, AB);
            var dotBCBM = dot(BC, BM);
            var dotBCBC = dot(BC, BC);
            return 0 <= dotABAM && dotABAM <= dotABAB && 0 <= dotBCBM && dotBCBM <= dotBCBC;
        }

        function vector(p1, p2) {
            return {
                    x: (p2.x - p1.x),
                    y: (p2.y - p1.y)
            };
        }

        function dot(u, v) {
            return u.x * v.x + u.y * v.y; 
        }

        var r = {
            A: {x: 50, y: 0},
            B: {x: 0, y: 20},
            C: {x: 10, y: 50},
            D: {x: 60, y: 30}
        };

        var m = {x: 40, y: 20};

        pointInRectangle(m, r); // returns true.
    */
}


// show the bounding rectangle for an object (that is not rotated)
void debug_entity_collision_rect(StaticEntity* static_entity) 
{
    scenery[0].entity.pos[0] =  static_entity->entity.pos[0] + get_static_entity_width(static_entity) / 2;
    scenery[0].entity.pos[1] =  static_entity->entity.pos[1] + get_static_entity_depth(static_entity) / 2;
    scenery[0].entity.pos[2] =  static_entity->entity.pos[2];

    scenery[1].entity.pos[0] =  static_entity->entity.pos[0] - get_static_entity_width(static_entity) / 2;
    scenery[1].entity.pos[1] =  static_entity->entity.pos[1] + get_static_entity_depth(static_entity) / 2;
    scenery[1].entity.pos[2] =  static_entity->entity.pos[2];

    scenery[2].entity.pos[0] =  static_entity->entity.pos[0] - get_static_entity_width(static_entity) / 2;
    scenery[2].entity.pos[1] =  static_entity->entity.pos[1] - get_static_entity_depth(static_entity) / 2;
    scenery[2].entity.pos[2] =  static_entity->entity.pos[2];

    scenery[3].entity.pos[0] =  static_entity->entity.pos[0] + get_static_entity_width(static_entity) / 2;
    scenery[3].entity.pos[1] =  static_entity->entity.pos[1] - get_static_entity_depth(static_entity) / 2;
    scenery[3].entity.pos[2] =  static_entity->entity.pos[2];
}



// Return minimum distance between line segment vw and point p

float minimum_distance(float *v, float *w, float *p) 
{
    const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) return distance_2d(p, v);   // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    float p_v[3] = {};
    p_v[0] = p[0] - v[0];
    p_v[1] = p[1] - v[1];
    float w_v[3] = {};
    w_v[0] = w[0] - v[0];
    w_v[1] = w[1] - v[1];
    const float t = max(0, min(1, dot(p_v, w_v) / l2));
    float projection[3];
    projection[0] = v[0] + t * (w[0] - v[0]);
    projection[1] = v[1] + t * (w[1] - v[1]);
    //const vec2 projection = v + t * (w - v);  // Projection falls on the segment
    return distance_2d(p, projection);
}

float min_dist_to_wall_old;
float min_dist_to_wall_new;

void detect_collisions() 
{
    StaticEntity *shack = &scenery[SCENERY_COUNT - 1];

    if ( pt_in_rect(nick.entity.pos, &scenery[SCENERY_COUNT - 1].entity, get_static_entity_width(shack), get_static_entity_depth(shack))) {
        // just do the oposite of what we did to move this frame to get out of the wall
        Entity *entity = &nick.entity;
        // calculate if the next frame will move us out of the box
        float new_pos[3] = {};
        float frame_distance = time_data.frame_duration * entity->speed;
        new_pos[0] = entity->pos[0] + frame_distance * sin(rad(entity->yaw));
        new_pos[1] = entity->pos[1] - frame_distance * cos(rad(entity->yaw));


        float A[3]; float B[3]; float C[3]; float D[3];

        int width = get_static_entity_width(shack);
        int depth = get_static_entity_depth(shack);

        A[0] = entity->pos[0] + width / 2;
        A[1] = entity->pos[1] + depth / 2;
        
        B[0] = entity->pos[0] - width / 2;
        B[1] = entity->pos[1] + depth / 2;
        
        C[0] = entity->pos[0] - width / 2;
        C[1] = entity->pos[1] - depth / 2;
        
        D[0] = entity->pos[0] + width / 2;
        D[1] = entity->pos[1] - depth / 2;

        min_dist_to_wall_old =

        min(minimum_distance(A, D, nick.entity.pos),
            min(minimum_distance(C, D, nick.entity.pos),
                min(minimum_distance(A, B, nick.entity.pos), 
                    minimum_distance(B, C, nick.entity.pos)
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

        if (pt_in_rect(new_pos, &scenery[SCENERY_COUNT - 1].entity, get_static_entity_width(shack), get_static_entity_depth(shack))
        ) {
            if (min_dist_to_wall_new != min_dist_to_wall_old) {
                nick.entity.speed = 0;
            }
        }
    }
    
    if ( distance(nick.entity.pos, willy.entity.pos) < 150) {
        nick.entity.speed = -800;
        set_entity_state(&nick, FALLBACK);
    }

    if ( distance(candy.entity.pos, willy.entity.pos) < 150) {
        //willy.entity.vertical_speed = 4000;
        willy.entity.speed = 800;
        set_entity_state(&willy, FALLBACK);
    }
}



#endif