#include <stdint.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <hvsengine/shaders.h>
#include <stdlib.h>
#include "assets.h"
#include "hvsengine/camera.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/vec3.h"
#include "stb_image.h"

#define WIDTH 800
#define HEIGHT 600

typedef struct keystate {
    int keycode;
    int state;
} keystate_t;

typedef struct state {
    uint8_t wireframe;
    keystate_t wState;
    float fov;
    vec2 lastMousePos;
} state_t;


state_t state = {
    .wireframe = 0,
    .fov = 45.0,
    .lastMousePos = {
        (float) WIDTH / 2.0, (float) HEIGHT / 2.0,
    },
};

camera_t camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, state_t *state, float dt) {
    if  (
            glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS
            || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
        ) {
        glfwSetWindowShouldClose(window, 1);
    }

    float cameraSpeed = 2.5f * dt;

    vec3 inputVector = {
        (
         glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS
         || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS
        ) 
        - 
        (
         glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS
         || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS
        ),

        (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        - 
        (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS),

        (
         glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS
         || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS
        )
        - 
        (
         glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
         || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS
        )
    };

    cam_applyMovement(inputVector, dt, &camera);

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && state->wState.state != GLFW_PRESS) {
        state->wState.state = GLFW_PRESS;
        state->wireframe = !state->wireframe;
        if (state->wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE && state->wState.state != GLFW_RELEASE) {
        state->wState.state = GLFW_RELEASE;
    }
}

uint8_t firstMouse = 1;
void processMouse(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        state.lastMousePos[0] = xpos;
        state.lastMousePos[1] = ypos;
        firstMouse = 0;
    }
    float xoffset = xpos - state.lastMousePos[0];
    float yoffset = ypos - state.lastMousePos[1];
    state.lastMousePos[0] = xpos;
    state.lastMousePos[1] = ypos;
    cam_applyMouse(xoffset, yoffset, 1, &camera);
}

int main() {
    cam_init((vec3) {0.0, 0.0, 3.0}, GLM_YUP, -90.0f, 0.0f, &camera);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Craft", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    uint32_t VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex buffer attributes
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    shader_t objectShader;
    const char vertex_path[] = SHADERS_LOCATION "/vertex.glsl";
    const char fragment_path[] = SHADERS_LOCATION "/fragment.glsl";
    if (shader_create(vertex_path, fragment_path, &objectShader)) return 1;

    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    float timeSinceFPS = 0.0f;
    float frames = 0.0;
    int fps = 0;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, processMouse);

    while(!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;
        frames += 1.0;

        if((timeSinceFPS += deltaTime) >= 1.0) {
            fps = (int) (frames / timeSinceFPS);
            timeSinceFPS = 0.0f;
            frames = 0.0;
        }

        printf("FrameTime: %f\tFPS: %d\r", deltaTime, fps);
        fflush(stdout);

        processInput(window, &state, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(&objectShader);
        shader_setVec3(&objectShader, "viewPos", camera.position);

        glBindVertexArray(VAO);

        mat4 view;
        cam_viewmatrix(&camera, view);

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        glm_perspective(glm_rad(camera.fov), (float) width / (float) height, 0.1, 100.0, projection);
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        shader_setMat4(&objectShader, "model", model);
        shader_setMat4(&objectShader, "view", view);
        shader_setMat4(&objectShader, "projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    printf("\n");
    glfwTerminate();
    return 0;
}

