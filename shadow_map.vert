#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

layout (location = 4) in vec4 aBonesWeights;
layout (location = 5) in ivec4 aBoneIDs;

uniform mat4 lightProjection;
uniform mat4 model;

void main()
{


    gl_Position = lightProjection * model * vec4(aPos, 1.0);
}