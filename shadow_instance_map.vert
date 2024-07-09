#version 330 core
// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;
// Instancing Transformations
layout (location = 4) in mat4 instanceMatrix;

uniform mat4 lightProjection;
uniform mat4 model;

void main()
{
    gl_Position = lightProjection * instanceMatrix * vec4(aPos, 1.0);
}