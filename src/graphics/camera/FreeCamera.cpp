//
// Created by Milan van Zanten on 16.03.18.
//

#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES

#include "FreeCamera.h"
#include "../../util/Log.h"

FreeCamera::FreeCamera(Window &capturedIn, Mouse &mouse, Keyboard &keyboard, glm::vec3 position, glm::vec2 angles)
        : capturedIn(capturedIn), angles(angles) {
    this->position = position;

    changeAspectRatio(capturedIn.getWindowWidth(), capturedIn.getWindowHeight());
    update(0);

    mouse.capture(capturedIn);
    mouse.addMoveCallback([this](Mouse &mouse) {
        if(mouseCaptured) {
            int sign = this->upDirection.y < 0 ? 1 : -1;
            this->angles.x += glm::radians<float>(static_cast<float>(-mouse.position().y)/10.0f);
            this->angles.y += glm::radians<float>(static_cast<float>(sign*mouse.position().x)/10.0f);

            this->leaning += leaningAmount*mouse.position().x;

            double a = std::fmod(this->angles.x + M_PI/2, 2*M_PI);
            sign = a < 0 ? -1 : 1;
            this->upDirection.y = (a <= sign*M_PI ? 1 : -1);
        }
    });

    keyboard.addPressedCallback([&mouse, this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(mouseCaptured)
            mouse.free();
        else
            mouse.capture(this->capturedIn);
        mouseCaptured = mouse.isCaptured();
    }, GLFW_KEY_LEFT_CONTROL);

    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.z = -speed;
    }, GLFW_KEY_W);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_S))
            acceleration.z = speed;
        else
            acceleration.z = 0;
    }, GLFW_KEY_W);
    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.z = speed;
    }, GLFW_KEY_S);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_W))
            acceleration.z = -speed;
        else
            acceleration.z = 0;
    }, GLFW_KEY_S);

    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.x = speed;
    }, GLFW_KEY_D);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_A))
            acceleration.x = -speed;
        else
            acceleration.x = 0;
    }, GLFW_KEY_D);
    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.x = -speed;
    }, GLFW_KEY_A);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_D))
            acceleration.x = speed;
        else
            acceleration.x = 0;
    }, GLFW_KEY_A);

    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.y = speed;
    }, GLFW_KEY_SPACE);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_LEFT_SHIFT))
            acceleration.y = -speed;
        else
            acceleration.y = 0;
    }, GLFW_KEY_SPACE);
    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.y = -speed;
    }, GLFW_KEY_LEFT_SHIFT);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_SPACE))
            acceleration.y = speed;
        else
            acceleration.y = 0;
    }, GLFW_KEY_LEFT_SHIFT);
}

void FreeCamera::changeAspectRatio(int width, int height) {
    aspectRatio = (float) width/(float) height;

    // projection matrix: 45° field of view, display range : 0.1 unit <-> 100 units
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void FreeCamera::update(long long int time) {
    lookingDirection = glm::normalize(glm::rotateY(glm::rotateX(glm::vec3(0.0f, 0.0f, -1.0f), angles.x), angles.y));
    leaning -= leaning*leaningFriction;
    glm::vec3 upDirectionTmp = glm::rotate(upDirection, leaning, lookingDirection);
    rightDirection = glm::normalize(glm::cross(lookingDirection, upDirectionTmp));

    float seconds = time/1000000000.0f;
    glm::vec3 accelerationTmp = acceleration*seconds;
    velocity += -lookingDirection*accelerationTmp.z + rightDirection*accelerationTmp.x + upDirectionTmp*accelerationTmp.y - velocity*friction;
    position += velocity*seconds;

    view = glm::lookAt(position, position + lookingDirection, upDirectionTmp);
}
