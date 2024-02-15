#version 330 core

in vec2 vertexUV;           // UV coordinates from the VS.

uniform sampler2D baseColor;        // The color texture.
uniform float coordMult;

out vec4 outColor; // Outputs color in RGBA.

void main() {
    
    // Multiply UV coords.
    vec2 uv = vertexUV * coordMult;

    // Get the base color from the texture.
    outColor = texture(baseColor, uv);
    
}
