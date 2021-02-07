#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
//varying vec2 pixelPos; // reflects change in resolution of screen

void main()
{
    gl_Position = vec4 (position.x, position.y, position.z, 1.0);
}
