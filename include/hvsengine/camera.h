#ifndef _CAMERA_H
#define _CAMERA_H

#include "cglm/types.h"
#include <stdbool.h>

#define SENSITIVITY 0.1f
#define MOVESPEED 2.5f
#define FOV 70.0f

typedef struct camera {
    vec3 position;

    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldup;

    float pitch;
    float yaw;
    float roll;

    float movespeed;
    float sensitivity;
    float fov;
} camera_t;

/*
 * Initializes a camera
 */
void cam_init(vec3 pos, vec3 up, float yaw, float pitch, camera_t *camera);

/*
 * Produces a view matrix from the camera and puts it in the matrix
 */
void cam_viewmatrix(camera_t *camera, mat4 viewmatrix);

/*
 * Applies a translation to the camera using an input vector
 */
void cam_applyMovement(vec3 inputVector, float dt, camera_t *camera);

/*
 *
 */
void cam_applyMouse(float xoffset, float yoffset, bool constrainPitch, camera_t* camera);

#endif
