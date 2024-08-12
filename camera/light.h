#ifndef LIGHT_H
#define LIGHT_H

// structures

typedef struct{
    Light amb;
    Light dir;
	Vector3 rotation;
} LightData;


// function protoypes

void light_set_dl(LightData *light);


// function implementations

// temporary function until i learn how the lights work
void light_set_dl(LightData *light)
{
    int i;

    //color
    for (i=0; i<3; i++) {

        // Ambient color
        light->amb.l.col[i]  = 130;
        light->amb.l.colc[i] = 130;

        //directional light color
        light->dir.l.col[i]  = 255;
        light->dir.l.colc[i] = 255;
    }

    // Direction
    light->dir.l.dir[0] = -127 * sinf(light->rotation.x * 0.0174532925);
    light->dir.l.dir[1] =  127 * sinf(light->rotation.z * 0.0174532925) * cosf(light->rotation.x * 0.0174532925);
    light->dir.l.dir[2] =  127 * cosf(light->rotation.z * 0.0174532925) * cosf(light->rotation.x * 0.0174532925);

    // Send the light struct to the RSP
    gSPNumLights(glistp++, NUMLIGHTS_1);
    gSPLight(glistp++, &light->dir, 1);
    gSPLight(glistp++, &light->amb, 2);
    gDPPipeSync(glistp++); 
}

#endif