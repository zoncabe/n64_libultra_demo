#ifndef TEMPLATE_H
#define TEMPLATE_H


void entity_collide_and_slide(CollisionPackage collision_package, const float *vel[3], const float *gravity[3])
{
    // Do collision detection:
    set_point(collision_package->r3_position, position);
    set_point(collision_package->r3_velocity, vel);


    // calculate position and velocity in eSpace
    VECTOR eSpacePosition = collision_package->r3_position / collision_package->e_radius;
    VECTOR eSpaceVelocity = collision_package->r3_velocity / collision_package->e_radius;

    // Iterate until we have our final position.
    collisionRecursionDepth = 0;
    VECTOR finalPosition = collideWithWorld(eSpacePosition,
    eSpaceVelocity);
    // Add gravity pull:
    44
    // To remove gravity uncomment from here .....
    // Set the new R3 position (convert back from eSpace to R3
    collision_package->r3_position =
    finalPosition*collision_package->e_radius;
    collision_package->r3_velocity = gravity;
    eSpaceVelocity = gravity/collision_package->e_radius;
    collisionRecursionDepth = 0;
    finalPosition = collideWithWorld(finalPosition,
    eSpaceVelocity);
    // ... to here
    // Convert final result back to R3:
    finalPosition = finalPosition*collision_package->e_radius;
    // Move the entity (application specific function)
    MoveTo(finalPosition);
    }
    // Set this to match application scale..
    const float unitsPerMeter = 100.0f;
    VECTOR CharacterEntity::collideWithWorld(const VECTOR& pos,
    const VECTOR& vel)
    {
    // All hard-coded distances in this function is
    // scaled to fit the setting above..
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;
    // do we need to worry?
    if (collisionRecursionDepth>5)
    return pos;
    // Ok, we need to worry:
    collision_package->velocity = vel;
    45
    collision_package->normalizedVelocity = vel;
    collision_package->normalizedVelocity.normalize();
    collision_package->basePoint = pos;
    collision_package->foundCollision = false;
    // Check for collision (calls the collision routines)
    // Application specific!!
    world->checkCollision(collision_package);
    // If no collision we just move along the velocity
    if (collision_package->foundCollision == false) {
    return pos + vel;
    }
    // *** Collision occured ***
    // The original destination point
    VECTOR destinationPoint = pos + vel;
    VECTOR newBasePoint = pos;
    // only update if we are not already very close
    // and if so we only move very close to intersection..not
    // to the exact spot.
    if (collision_package->nearestDistance>=veryCloseDistance)
    {
    VECTOR V = vel;
    V.SetLength(collision_package->nearestDistanceveryCloseDistance);
    newBasePoint = collision_package->basePoint + V;
    // Adjust polygon intersection point (so sliding
    // plane will be unaffected by the fact that we
    // move slightly less than collision tells us)
    V.normalize();
    collision_package->intersectionPoint -=
    veryCloseDistance * V;
    }
    // Determine the sliding plane
    VECTOR slidePlaneOrigin =
    46
    collision_package->intersectionPoint;
    VECTOR slidePlaneNormal =
    newBasePoint-collision_package->intersectionPoint;
    slidePlaneNormal.normalize();
    PLANE slidingPlane(slidePlaneOrigin,slidePlaneNormal);
    // Again, sorry about formatting.. but look carefully ;)
    VECTOR newDestinationPoint = destinationPoint -
    slidingPlane.signedDistanceTo(destinationPoint)*
    slidePlaneNormal;
    // Generate the slide vector, which will become our new
    // velocity vector for the next iteration
    VECTOR newVelocityVector = newDestinationPoint -
    collision_package->intersectionPoint;
    // Recurse:
    // dont recurse if the new velocity is very small
    if (newVelocityVector.length() < veryCloseDistance) {
    return newBasePoint;
    }
    collisionRecursionDepth++;
    return collideWithWorld(newBasePoint,newVelocityVector);











































#include <math.h>

typedef struct {
    float x;
    float y;
    float z;
} VECTOR;

typedef struct {
    VECTOR r3_position;
    VECTOR r3_velocity;
    float e_radius;
    VECTOR velocity;
    VECTOR normalizedVelocity;
    VECTOR basePoint;
    int foundCollision;
    float nearestDistance;
    VECTOR intersectionPoint;
} Collision_package;

typedef struct {
    VECTOR origin;
    VECTOR normal;
} PLANE;

void MoveTo(VECTOR finalPosition);

const float unitsPerMeter = 100.0f;
Collision_package* collision_package;

void CharacterEntity_collideAndSlide(const VECTOR* vel, const VECTOR* gravity) {
    // Do collision detection:
    collision_package->r3_position = position;
    collision_package->r3_velocity = *vel;
    // calculate position and velocity in eSpace
    VECTOR eSpacePosition;
    eSpacePosition.x = collision_package->r3_position.x / collision_package->e_radius;
    eSpacePosition.y = collision_package->r3_position.y / collision_package->e_radius;
    eSpacePosition.z = collision_package->r3_position.z / collision_package->e_radius;
    VECTOR eSpaceVelocity;
    eSpaceVelocity.x = collision_package->r3_velocity.x / collision_package->e_radius;
    eSpaceVelocity.y = collision_package->r3_velocity.y / collision_package->e_radius;
    eSpaceVelocity.z = collision_package->r3_velocity.z / collision_package->e_radius;
    // Iterate until we have our final position.
    int collisionRecursionDepth = 0;
    VECTOR finalPosition = CharacterEntity_collideWithWorld(&eSpacePosition, &eSpaceVelocity);
    // Add gravity pull:
    // To remove gravity, uncomment from here ...
    // Set the new R3 position (convert back from eSpace to R3)
    collision_package->r3_position.x = finalPosition.x * collision_package->e_radius;
    collision_package->r3_position.y = finalPosition.y * collision_package->e_radius;
    collision_package->r3_position.z = finalPosition.z * collision_package->e_radius;
    collision_package->r3_velocity = *gravity;
    eSpaceVelocity = *gravity / collision_package->e_radius;
    collisionRecursionDepth = 0;
    finalPosition = CharacterEntity_collideWithWorld(&finalPosition, &eSpaceVelocity);
    // ... to here
    // Convert final result back to R3:
    collision_package->r3_position.x = finalPosition.x * collision_package->e_radius;
    collision_package->r3_position.y = finalPosition.y * collision_package->e_radius;
    collision_package->r3_position.z = finalPosition.z * collision_package->e_radius;
    // Move the entity (application-specific function)
    MoveTo(finalPosition);
}

// Set this to match application scale...
const float unitsPerMeter = 100.0f;

VECTOR CharacterEntity_collideWithWorld(const VECTOR* pos, const VECTOR* vel) {
    // All hard-coded distances in this function are scaled to fit the setting above...
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;
    // Do we need to worry?
    if (collisionRecursionDepth > 5)
        return *pos;
    // OK, we need to worry:
    collision_package->velocity = *vel;
    collision_package->normalizedVelocity = *vel;
    collision_package->normalizedVelocity.normalize();
    collision_package->basePoint = *pos;
    collision_package->foundCollision = 0;
    // Check for collision (calls the collision routines)
    // Application specific!!
    world->checkCollision(collision_package);
    // If no collision, we just move along the velocity
    if (!collision_package->foundCollision) {
        VECTOR result;
        result.x = pos->x + vel->x;
        result.y = pos->y + vel->y;
        result.z = pos->z + vel->z;
        return result;
    }
    // *** Collision occurred ***
    // The original destination point
    VECTOR destinationPoint;
    destinationPoint.x = pos->x + vel->x;
    destinationPoint.y = pos->y + vel->y;
    destinationPoint.z = pos->z + vel->z;
    VECTOR newBasePoint = *pos;
    // Only update if we are not already very close
    // And if so, we only move very close to intersection, not
    // to the exact spot.
    if (collision_package->nearestDistance >= veryCloseDistance) {
        VECTOR V = *vel;
        float factor = collision_package->nearestDistance * veryCloseDistance;
        V.x *= factor;
        V.y *= factor;
        V.z *= factor;
        newBasePoint.x = collision_package->basePoint.x + V.x;
        newBasePoint.y = collision_package->basePoint.y + V.y;
        newBasePoint.z = collision_package->basePoint.z + V.z;
        // Adjust polygon intersection point (so sliding
        // plane will be unaffected by the fact that we
        // move slightly less than collision tells us)
        V.normalize();
        collision_package->intersectionPoint.x -= veryCloseDistance * V.x;
        collision_package->intersectionPoint.y -= veryCloseDistance * V.y;
        collision_package->intersectionPoint.z -= veryCloseDistance * V.z;
    }
    // Determine the sliding plane
    VECTOR slidePlaneOrigin = collision_package->intersectionPoint;
    VECTOR slidePlaneNormal;
    slidePlaneNormal.x = newBasePoint.x - collision_package->intersectionPoint.x;
    slidePlaneNormal.y = newBasePoint.y - collision_package->intersectionPoint.y;
    slidePlaneNormal.z = newBasePoint.z - collision_package->intersectionPoint.z;
    float length = sqrt(slidePlaneNormal.x * slidePlaneNormal.x +
                        slidePlaneNormal.y * slidePlaneNormal.y +
                        slidePlaneNormal.z * slidePlaneNormal.z);
    slidePlaneNormal.x /= length;
    slidePlaneNormal.y /= length;
    slidePlaneNormal.z /= length;
    PLANE slidingPlane;
    slidingPlane.origin = slidePlaneOrigin;
    slidingPlane.normal = slidePlaneNormal;
    // Again, sorry about formatting... but look carefully ;)
    VECTOR newDestinationPoint;
    float distance = slidingPlaneDistanceTo(&slidingPlane, &destinationPoint);
    newDestinationPoint.x = destinationPoint.x - distance * slidePlaneNormal.x;
    newDestinationPoint.y = destinationPoint.y - distance * slidePlaneNormal.y;
    newDestinationPoint.z = destinationPoint.z - distance * slidePlaneNormal.z;
    // Generate the slide vector, which will become our new
    // velocity vector for the next iteration
    VECTOR newVelocityVector;
    newVelocityVector.x = newDestinationPoint.x - collision_package->intersectionPoint.x;
    newVelocityVector.y = newDestinationPoint.y - collision_package->intersectionPoint.y;
    newVelocityVector.z = newDestinationPoint.z - collision_package->intersectionPoint.z;
    // Recurse:
    // Don't recurse if the new velocity is very small
    if (lengthOfVector(&newVelocityVector) < veryCloseDistance) {
        return newBasePoint;
    }
    collisionRecursionDepth++;
    return CharacterEntity_collideWithWorld(&newBasePoint, &newVelocityVector);
}








#include <math.h>

typedef struct {
    float r3_position[3];
    float r3_velocity[3];
    float e_radius;
    float velocity[3];
    float normalizedVelocity[3];
    float basePoint[3];
    int foundCollision;
    float nearestDistance;
    float intersectionPoint[3];
} Collision_package;

typedef struct {
    float origin[3];
    float normal[3];
} PLANE;

void MoveTo(float finalPosition[3]);

const float unitsPerMeter = 100.0f;
Collision_package* collision_package;


void CharacterEntity_collideAndSlide(const float vel[3], const float gravity[3]) {
    // Do collision detection:
    for (int i = 0; i < 3; i++) {
        collision_package->r3_position[i] = position[i];
        collision_package->r3_velocity[i] = vel[i];
    }
    // calculate position and velocity in eSpace
    float eSpacePosition[3];
    float eSpaceVelocity[3];
    for (int i = 0; i < 3; i++) {
        eSpacePosition[i] = collision_package->r3_position[i] / collision_package->e_radius;
        eSpaceVelocity[i] = collision_package->r3_velocity[i] / collision_package->e_radius;
    }
    // Iterate until we have our final position.
    int collisionRecursionDepth = 0;
    float finalPosition[3] = CharacterEntity_collideWithWorld(eSpacePosition, eSpaceVelocity);
    // Add gravity pull:
    // To remove gravity, uncomment from here ...
    // Set the new R3 position (convert back from eSpace to R3)
    for (int i = 0; i < 3; i++) {
        collision_package->r3_position[i] = finalPosition[i] * collision_package->e_radius;
    }
    for (int i = 0; i < 3; i++) {
        collision_package->r3_velocity[i] = gravity[i];
    }
    for (int i = 0; i < 3; i++) {
        eSpaceVelocity[i] = gravity[i] / collision_package->e_radius;
    }
    collisionRecursionDepth = 0;
    finalPosition = CharacterEntity_collideWithWorld(finalPosition, eSpaceVelocity);
    // ... to here
    // Convert final result back to R3:
    for (int i = 0; i < 3; i++) {
        collision_package->r3_position[i] = finalPosition[i] * collision_package->e_radius;
    }
    MoveTo(finalPosition);
}

// Set this to match application scale..
const float unitsPerMeter = 100.0f;

float* CharacterEntity_collideWithWorld(const float pos[3], const float vel[3]) {
    // All hard-coded distances in this function are
    // scaled to fit the setting above..
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;
    // do we need to worry?
    if (collisionRecursionDepth > 5)
        return pos;
    // Ok, we need to worry:
    for (int i = 0; i < 3; i++) {
        collision_package->velocity[i] = vel[i];
    }
    for (int i = 0; i < 3; i++) {
        collision_package->normalizedVelocity[i] = vel[i];
    }
    // Normalize the normalizedVelocity vector
    float normalizedVelocityLength = 0.0f;
    for (int i = 0; i < 3; i++) {
        normalizedVelocityLength += vel[i] * vel[i];
    }
    normalizedVelocityLength = sqrt(normalizedVelocityLength);
    for (int i = 0; i < 3; i++) {
        collision_package->normalizedVelocity[i] /= normalizedVelocityLength;
    }
    for (int i = 0; i < 3; i++) {
        collision_package->basePoint[i] = pos[i];
    }
    collision_package->foundCollision = 0;
    // Check for collision (calls the collision routines)
    // Application specific!!
    world->checkCollision(collision_package);
    // If no collision we just move along the velocity
    if (collision_package->foundCollision == 0) {
        float newPos[3];
        for (int i = 0; i < 3; i++) {
            newPos[i] = pos[i] + vel[i];
        }
        return newPos;
    }
    // *** Collision occurred ***
    // The original destination point
    float destinationPoint[3];
    for (int i = 0; i < 3; i++) {
        destinationPoint[i] = pos[i] + vel[i];
    }
    float newBasePoint[3];
    for (int i = 0; i < 3; i++) {
        newBasePoint[i] = pos[i];
    }
    // only update if we are not already very close
    // and if so we only move very close to intersection..not
    // to the exact spot.
    if (collision_package->nearestDistance >= veryCloseDistance) {
        float V[3];
        for (int i = 0; i < 3; i++) {
            V[i] = vel[i];
        }
        for (int i = 0; i < 3; i++) {
            V[i] *= collision_package->nearestDistance / veryCloseDistance;
        }
        for (int i = 0; i < 3; i++) {
            newBasePoint[i] = collision_package->basePoint[i] + V[i];
        }
        // Adjust polygon intersection point (so sliding
        // plane will be unaffected by the fact that we
        // move slightly less than collision tells us)
        float VLength = 0.0f;
        for (int i = 0; i < 3; i++) {
            VLength += V[i] * V[i];
        }
        VLength = sqrt(VLength);
        for (int i = 0; i < 3; i++) {
            V[i] /= VLength;
        }
        for (int i = 0; i < 3; i++) {
            collision_package->intersectionPoint[i] -= veryCloseDistance * V[i];
        }
    }
    // Determine the sliding plane
    float slidePlaneOrigin[3];
    for (int i = 0; i < 3; i++) {
        slidePlaneOrigin[i] = collision_package->intersectionPoint[i];
    }
    float slidePlaneNormal[3];
    for (int i = 0; i < 3; i++) {
        slidePlaneNormal[i] = newBasePoint[i] - collision_package->intersectionPoint[i];
    }
    // Normalize the slidePlaneNormal vector
    float slidePlaneNormalLength = 0.0f;
    for (int i = 0; i < 3; i++) {
        slidePlaneNormalLength += slidePlaneNormal[i] * slidePlaneNormal[i];
    }
    slidePlaneNormalLength = sqrt(slidePlaneNormalLength);
    for (int i = 0; i < 3; i++) {
        slidePlaneNormal[i] /= slidePlaneNormalLength;
    }
    PLANE slidingPlane;
    for (int i = 0; i < 3; i++) {
        slidingPlane.origin[i] = slidePlaneOrigin[i];
        slidingPlane.normal[i] = slidePlaneNormal[i];
    }
    float newDestinationPoint[3];
    for (int i = 0; i < 3; i++) {
        newDestinationPoint[i] = destinationPoint[i] - slidingPlane.signedDistanceTo(destinationPoint) * slidePlaneNormal[i];
    }
    // Generate the slide vector, which will become our new
    // velocity vector for the next iteration
    float newVelocityVector[3];
    for (int i = 0; i < 3; i++) {
        newVelocityVector[i] = newDestinationPoint[i] - collision_package->intersectionPoint[i];
    }
    // Recurse:
    // don't recurse if the new velocity is very small
    float newVelocityVectorLength = 0.0f;
    for (int i = 0; i < 3; i++) {
        newVelocityVectorLength += newVelocityVector[i] * newVelocityVector[i];
    }
    newVelocityVectorLength = sqrt(newVelocityVectorLength);
    if (newVelocityVectorLength < veryCloseDistance) {
        return newBasePoint;
    }
    collisionRecursionDepth++;
    return CharacterEntity_collideWithWorld(newBasePoint, newVelocityVector);
}








}

#endif