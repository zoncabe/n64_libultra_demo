#ifndef SHAPES_H
#define SHAPES_H

/* SHAPES.H
here are all the structures and functions prototypes that involve the terrain generation */


/* gfx_axis                             
An axis model which was useful for getting calculations right.
Red represents X, Green represents Y, Blue represents Z. */

Vtx vtx_axis[] = {
  {-2, -2, -2, 0, 0, 0, -73, -73, -73, 255},
  { 50, -2, -2, 0, 0, 0,  73, -73, -73, 255},
  { 50,  2, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  2, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2, -2,  2, 0, 0, 0, -73, -73,  73, 255},
  { 50, -2,  2, 0, 0, 0,  73, -73,  73, 255},
  { 50,  2,  2, 0, 0, 0,  73,  73,  73, 255},
  {-2,  2,  2, 0, 0, 0, -73,  73,  73, 255},
  
  {-2, -2, -2, 0, 0, 0, -73, -73, -73, 255},
  { 2, -2, -2, 0, 0, 0,  73, -73, -73, 255},
  { 2,  50, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  50, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2, -2,  2, 0, 0, 0, -73, -73,  73, 255},
  { 2, -2,  2, 0, 0, 0,  73, -73,  73, 255},
  { 2,  50,  2, 0, 0, 0,  73,  73,  73, 255},
  {-2,  50,  2, 0, 0, 0, -73,  73,  73, 255},

  {-2, -2, -2, 0, 0, 0, -73, -73, -73, 255},
  { 2, -2, -2, 0, 0, 0,  73, -73, -73, 255},
  { 2,  2, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  2, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2, -2,  50, 0, 0, 0, -73, -73,  73, 255},
  { 2, -2,  50, 0, 0, 0,  73, -73,  73, 255},
  { 2,  2,  50, 0, 0, 0,  73,  73,  73, 255},
  {-2,  2,  50, 0, 0, 0, -73,  73,  73, 255},
};

Gfx gfx_axis[] = {

    gsDPSetPrimColor(0, 0, 255, 0, 0, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_axis, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),

    gsDPSetPrimColor(0, 0, 0, 255, 0, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_axis+8, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),

    gsDPSetPrimColor(0, 0, 0, 0, 255, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_axis+16, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),
    gsSPEndDisplayList(),
};

Vtx vtx_laser[] = {
  {-2,  0, -2, 0, 0, 0, -73, -73, -73, 255},
  { 2,  0, -2, 0, 0, 0,  73, -73, -73, 255},
  { 2,  10, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  10, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2,  0,  2, 0, 0, 0, -73, -73,  73, 255},
  { 2,  0,  2, 0, 0, 0,  73, -73,  73, 255},
  { 2,  10,  2, 0, 0, 0,  73,  73,  73, 255},
  {-2,  10,  2, 0, 0, 0, -73,  73,  73, 255},

};

Gfx gfx_laser[] = {
  
    gsDPSetPrimColor(0, 0, 255, 0, 0, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_laser, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),
    gsSPEndDisplayList(),
};

Vtx vtx_dot[] = {
  {-2,  -2, -2, 0, 0, 0, -73, -73, -73, 255},
  { 2,  -2, -2, 0, 0, 0,  73, -73, -73, 255},
  { 2,  2, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  2, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2,  -2,  2, 0, 0, 0, -73, -73,  73, 255},
  { 2,  -2,  2, 0, 0, 0,  73, -73,  73, 255},
  { 2,  2,  2, 0, 0, 0,  73,  73,  73, 255},
  {-2,  2,  2, 0, 0, 0, -73,  73,  73, 255},

};

Gfx gfx_red_dot[] = {
  
    gsDPSetPrimColor(0, 0, 255, 0, 0, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_dot, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),
    gsSPEndDisplayList(),
};

Gfx gfx_green_dot[] = {
  
    gsDPSetPrimColor(0, 0, 0, 255, 0, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_dot, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),
    gsSPEndDisplayList(),
};
Gfx gfx_blue_dot[] = {
  
    gsDPSetPrimColor(0, 0, 0, 0, 255, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_dot, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),
    gsSPEndDisplayList(),
};

/* gfx_mirror_axis                              
mirrored axis */

Vtx vtx_mirror_axis[] = {
  {-2, -2, -2, 0, 0, 0, -73, -73, -73, 255},
  { 50, -2, -2, 0, 0, 0,  73, -73, -73, 255},
  { 50,  2, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  2, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2, -2,  2, 0, 0, 0, -73, -73,  73, 255},
  { 50, -2,  2, 0, 0, 0,  73, -73,  73, 255},
  { 50,  2,  2, 0, 0, 0,  73,  73,  73, 255},
  {-2,  2,  2, 0, 0, 0, -73,  73,  73, 255},
  {-2, -2, -2, 0, 0, 0, -73, -73, -73, 255},
  { 2, -2, -2, 0, 0, 0,  73, -73, -73, 255},
  { 2,  50, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  50, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2, -2,  2, 0, 0, 0, -73, -73,  73, 255},
  { 2, -2,  2, 0, 0, 0,  73, -73,  73, 255},
  { 2,  50,  2, 0, 0, 0,  73,  73,  73, 255},
  {-2,  50,  2, 0, 0, 0, -73,  73,  73, 255},
  {-2, -2, -2, 0, 0, 0, -73, -73, -73, 255},
  { 2, -2, -2, 0, 0, 0,  73, -73, -73, 255},
  { 2,  2, -2, 0, 0, 0,  73,  73, -73, 255},
  {-2,  2, -2, 0, 0, 0, -73,  73, -73, 255},
  {-2, -2,  50, 0, 0, 0, -73, -73,  73, 255},
  { 2, -2,  50, 0, 0, 0,  73, -73,  73, 255},
  { 2,  2,  50, 0, 0, 0,  73,  73,  73, 255},
  {-2,  2,  50, 0, 0, 0, -73,  73,  73, 255},
};

Gfx gfx_mirror_axis[] = {
    gsDPSetPrimColor(0, 0, 0, 255, 0, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_mirror_axis, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),

    gsDPSetPrimColor(0, 0, 255, 0, 0, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_mirror_axis+8, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),

    gsDPSetPrimColor(0, 0, 0, 0, 255, 255),
    gsDPPipeSync(),
    gsSPVertex(vtx_mirror_axis+16, 8, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 5, 6, 0),
    gsSP2Triangles(6, 5, 2, 1, 2, 5, 1, 1),
    gsSP2Triangles(1, 3, 2, 0, 0, 3, 1, 2),
    gsSP2Triangles(3, 0, 7, 2, 0, 4, 7, 2),
    gsSP2Triangles(2, 7, 6, 0, 2, 3, 7, 0),
    gsSP2Triangles(0, 5, 4, 0, 0, 1, 5, 0),
    
    gsSPEndDisplayList(),
};


#endif
