#include "hvsengine/camera.h"
#include "cglm/cam.h"
#include "cglm/vec3.h"
#include <stdbool.h>

void updateCameraVectors(camera_t *camera) {
    camera->front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(-camera->pitch));
    camera->front[1] = sinf(glm_rad(-camera->pitch));
    camera->front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(-camera->pitch));
    glm_normalize(camera->front);

    glm_cross(camera->front, GLM_YUP, camera->right);
    glm_normalize(camera->right);

    glm_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);
}

void cam_init(vec3 pos, vec3 up, float yaw, float pitch, camera_t *camera) {
    glm_vec3_copy(pos, camera->position);
    glm_vec3_copy(up, camera->worldup);
    glm_vec3_copy(GLM_YUP, camera->up);
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->roll = 0.0f;
    camera->sensitivity = SENSITIVITY;
    camera->fov = FOV;
    camera->movespeed = MOVESPEED;
    updateCameraVectors(camera);
}

void cam_viewmatrix(camera_t *camera, mat4 viewmatrix) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(
            camera->position,
            center,
            camera->up,
            viewmatrix
            );
}

void cam_applyMovement(vec3 inputVector, float dt, camera_t *camera) {
    glm_normalize(inputVector);
    vec3 frontMovement;
    frontMovement[0] = camera->front[0];
    frontMovement[1] = 0;
    frontMovement[2] = camera->front[2];
    glm_normalize(frontMovement);
    glm_vec3_scale(frontMovement, inputVector[2], frontMovement);

    vec3 sideMovement;
    sideMovement[1] = 0;
    sideMovement[0] = camera->right[0];
    sideMovement[2] = camera->right[2];
    glm_normalize(sideMovement);
    glm_vec3_scale(sideMovement, inputVector[0], sideMovement);

    vec3 movement;
    glm_vec3_add(frontMovement, sideMovement, movement);
    movement[1] = inputVector[1];
    glm_vec3_scale(movement, camera->movespeed * dt, movement);
    glm_vec3_add(camera->position, movement, camera->position);
}

void cam_applyMouse(float xoffset, float yoffset, bool constrainPitch, camera_t *camera) {
    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;

    camera->yaw     += xoffset;
    camera->pitch   += yoffset;

    if (constrainPitch) {
        if (camera->pitch >  89.0f) camera->pitch = 89.0f;
        if (camera->pitch < -89.0f) camera->pitch = -89.0f;
    }

    updateCameraVectors(camera);
}

