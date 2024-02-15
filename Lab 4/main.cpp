/**
 * @file main.cpp
 * @brief Main aladdin 3d file.
 * @version 1.0.0 (2022-10-21)
 * @date 2022-10-21
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#define GLM_ENABLE_EXPERIMENTAL

#include "main.h"

#include <math.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/common.hpp"
#include "glm/gtx/string_cast.hpp"

#include "classes/camera/camera.h"
#include "classes/light/light.h"
#include "classes/object/object.h"
#include "classes/shader/shader.h"
#include "structs/bounding_box/bounding_box.h"

void clean() {

	// Delete all the shaders.
	shader->remove();
    
    // Terminate ImGUI.
    ImGui_ImplGlfwGL3_Shutdown();
    
    // Close GL context and any other GLFW resources.
    glfwTerminate();

}

void clear() {
    
    // Specify the color of the background
    glClearColor(background.x, background.y, background.z, background.w);

    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}

void displayElements() {
    
    // Get info from the model.
    bgq_opengl::BoundingBox bb = objects[current_object].getBoundingBox();
    glm::vec3 centre = (bb.min + bb.max) / 2.0f;
    glm::vec3 size = bb.max - bb.min;
    float max_dim = std::max(size.x, std::max(size.y, size.z));
    float scale_rat = NORM_SIZE / max_dim;
        
    // Pass the parameters to the shaders.
    shader->activate();
    shader->passLight(scene_light);
    
    objects[current_object].resetTransforms();
    
    if (current_object == 0) {
        objects[current_object].translate(0.0f, 0.0f, -5.0f);
        objects[current_object].scale(15.0, 15.0, 15.0);
    }
    
    // Rotate
    objects[current_object].rotate(0.0, 1.0, 0.0, internal_time * 5.0);
    
    // Resize it to normalize it.
    objects[current_object].scale(scale_rat, scale_rat, scale_rat);
    
    // Center the object and get it in the right position.
    objects[current_object].translate(-centre.x, -centre.y, -centre.z);
    
    // Pass variables to the shaders.
    shader->passFloat("coordMult", coord_multiplier);
    
    // Pass the textures.
    shader->passTexture(textures[5 * current_texture + mipmapping_on * (2 * texel_interpol + mipmap_interpol) + mipmapping_on]);

    // Draw the object.
    objects[current_object].draw(*shader, *camera);
        
}

void displayGUI() {
    
    // Init ImGUI for rendering.
    ImGui_ImplGlfwGL3_NewFrame();
    
    // Begin the new widget.
    ImGui::Begin("Parameters");
    
    // Pass these values to the shaders.
    ImGui::Text("Object");
    ImGui::RadioButton("Plane", &current_object, 0);
    ImGui::RadioButton("Cube", &current_object, 1);
    ImGui::RadioButton("Sphere", &current_object, 2);

    // Pass these values to the shaders.
    ImGui::Text("Textures");
    ImGui::RadioButton("Chess", &current_texture, 0);
    ImGui::RadioButton("Colours", &current_texture, 1);
    ImGui::RadioButton("Tiles", &current_texture, 2);
    
    ImGui::Text("Mipmapping");
    ImGui::RadioButton("No", &mipmapping_on, 0);
    ImGui::RadioButton("Yes", &mipmapping_on, 1);
    
    ImGui::Text("Texel minification");
    ImGui::RadioButton("Nearest1", &texel_interpol, 0);
    ImGui::RadioButton("Interpolate1", &texel_interpol, 1);
    
    ImGui::Text("Mipmap minification");
    ImGui::RadioButton("Nearest2", &mipmap_interpol, 0);
    ImGui::RadioButton("Interpolate2", &mipmap_interpol, 1);
    
    // Pass these values to the shaders.
    ImGui::Text("Texture params");
    ImGui::SliderFloat("Size", &coord_multiplier, 0.01, 100.0);

    ImGui::End();
    
    // Render ImGUI.
    ImGui::Render();
    
}

void handleKeyEvents() {
    
    // Key W will move camera 0 forward.
    if (ImGui::IsKeyPressed('W')) {

        // Move forward.
        camera->moveFront();
        
    }

    // Key S will move camera 0 backwards.
    if (ImGui::IsKeyPressed('S')) {

        // Move backwards.
        camera->moveBack();

    }

    // Key D will move camera 0 to the right.
    if (ImGui::IsKeyPressed('D')) {

        // Move right.
        camera->moveRight();

    }

    // Key A will move camera 0 to the right.
    if (ImGui::IsKeyPressed('A')) {

        // Move to the left.
        camera->moveLeft();

    }

    // Key R will move camera up.
    if (ImGui::IsKeyPressed('R')) {

        // Move up.
        camera->moveUp();

    }

    // Key F will move camera down.
    if (ImGui::IsKeyPressed('F')) {

        // Move up.
        camera->moveDown();

    }

    // Key E will rotate camera left.
    if (ImGui::IsKeyPressed('E')) {

        // Move up.
        camera->rotateLeft();

    }

    // Key Q will rotate the camera right.
    if (ImGui::IsKeyPressed('Q')) {

        // Move up.
        camera->rotateRight();

    }

    // Key T will bring the camera up.
    if (ImGui::IsKeyPressed('T')) {

        // Move up.
        camera->rotateUp();

    }

    // Key G will bring the camera down.
    if (ImGui::IsKeyPressed('G')) {

        // Move up.
        camera->rotateDown();

    }
    
}

void initElements() {
    
	// Create a white light in the center of the world.
    scene_light = bgq_opengl::Light(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Init the shader.
	shader = new bgq_opengl::Shader("mipmap.vert", "mipmap.frag");
    
	// Creates the first camera object
    camera = new bgq_opengl::Camera(glm::vec3(0.0f, 0.5f, 2.0f), glm::vec3(0.0f, -0.25f, -1.0f), 45.0f, 0.1f, 300.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Load the textures.
    textures.push_back(bgq_opengl::Texture("chess.png", "baseColor", 1, GL_NEAREST, GL_NEAREST));
    textures.push_back(bgq_opengl::Texture("chess.png", "baseColor", 1, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    textures.push_back(bgq_opengl::Texture("chess.png", "baseColor", 1, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR));
    textures.push_back(bgq_opengl::Texture("chess.png", "baseColor", 1, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST));
    textures.push_back(bgq_opengl::Texture("chess.png", "baseColor", 1, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR));

    textures.push_back(bgq_opengl::Texture("colors.png", "baseColor", 1, GL_NEAREST, GL_NEAREST));
    textures.push_back(bgq_opengl::Texture("colors.png", "baseColor", 1, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    textures.push_back(bgq_opengl::Texture("colors.png", "baseColor", 1, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR));
    textures.push_back(bgq_opengl::Texture("colors.png", "baseColor", 1, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST));
    textures.push_back(bgq_opengl::Texture("colors.png", "baseColor", 1, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR));

    textures.push_back(bgq_opengl::Texture("tiles.png", "baseColor", 1, GL_NEAREST, GL_NEAREST));
    textures.push_back(bgq_opengl::Texture("tiles.png", "baseColor", 1, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    textures.push_back(bgq_opengl::Texture("tiles.png", "baseColor", 1, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR));
    textures.push_back(bgq_opengl::Texture("tiles.png", "baseColor", 1, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST));
    textures.push_back(bgq_opengl::Texture("tiles.png", "baseColor", 1, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR));

    // Load the objects.
    objects.push_back(bgq_opengl::Object("Plane.obj", "Assimp"));
    objects.push_back(bgq_opengl::Object("Cube.obj", "Assimp"));
    objects.push_back(bgq_opengl::Object("Sphere.obj", "Assimp"));

}

void initEnvironment(int argc, char** argv) {
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        exit(1);

    }
    
    // MacOS initializations.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create the window.
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME, NULL, NULL);
    if (!window) {
        
        std::cerr << "Error 121-1001 - Failed to create the window." << std::endl;
        glfwTerminate();
        exit(1);

    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW and OpenGL.
    GLenum res = glewInit();

    // Check for any errors.
    if (res != GLEW_OK) {

        std::cerr << "Error 121-1002 - GLEW could not be initialized:" << glewGetErrorString(res) << std::endl;
        exit(1);

    }
    
    // Get version info.
    std::cerr << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cerr << "OpenGL version supported " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "GLSL version supported " << (char *) glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
}

int main(int argc, char** argv) {

	// Initialise the environment.
    initEnvironment(argc, argv);
    
	// Initialise the objects and elements.
	initElements();

	// Main loop.
    while(!glfwWindowShouldClose(window)) {
        
        // Clear the scene.
        clear();
        
        // Get the window size.
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        camera->setWidth(width);
        camera->setHeight(height);
        
        // Get the current time.
        internal_time = glfwGetTime();

        // Handle key events.
        handleKeyEvents();
        
        // Display the scene.
        displayElements();
        
        // Make the things to print everything.
        displayGUI();
        
        // Update other events like input handling.
        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }

	// Clean everything and terminate.
	clean();

}
