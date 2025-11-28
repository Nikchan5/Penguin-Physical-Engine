// shaders.frag
#version 460 core

in vec3 color;      // Color from Vertex Shader
in vec2 texCoord;   // Texture Coords from Vertex Shader

out vec4 FragColor;

uniform sampler2D uTexture; 

void main()
{

    FragColor = texture(uTexture, texCoord); 
    
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}