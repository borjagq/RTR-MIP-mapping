#version 330 core

layout (location = 0) in vec3 inVertex;    // Vertex.
layout (location = 1) in vec3 inNormal;    // Normal (not necessarily normalized).
layout (location = 2) in vec3 inColor;    // Color (not necessarily normalized).
layout (location = 3) in vec2 inUV;        // UV coordinates.
layout (location = 4) in vec3 inTangents;   // UV coordinates.
layout (location = 5) in vec3 inBitangents; // UV coordinates.

uniform mat4 Model;            // Imports the model matrix.
uniform mat4 View;            // Imports the View matrix.
uniform mat4 Projection;    // Imports the projection matrix.
uniform mat4 modelView;        // Imports the modelView already multiplied.

out vec2 vertexUV;            // Passes the UV coordinates to the fragment shader.

void main() {

    // Assigns the direct passes.
    vertexUV = mat2(0.0, -1.0, 1.0, 0.0) * inUV;

    // Sets the visualized position by applying the camera matrix.
    gl_Position = Projection * modelView * vec4(inVertex, 1.0);
    
}
