// shaders.vert
#version 460 core

layout (location = 0) in vec3 aPos;     // Position
layout (location = 1) in vec3 aColor;   // Color
layout (location = 2) in vec2 aTex;     // Texture Coords (LOCATION 2 IS CRITICAL)

out vec3 color;
out vec2 texCoord;

uniform mat4 uModel;
uniform mat4 uVP;

void main()
{
    color = aColor;
    texCoord = aTex; // Passing texture coords to the fragment shader
    gl_Position = uVP * uModel * vec4(aPos, 1.0);
}