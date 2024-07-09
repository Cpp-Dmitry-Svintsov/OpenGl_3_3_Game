#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

layout (location = 4) in vec4 aBonesWeights;
layout (location = 5) in ivec4 aBoneIDs;


out vec3 crntPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;
out vec4 fragPosLight;

const int MAX_BONES = 200;
uniform mat4 bones_matrix[MAX_BONES];



// Imports the camera matrix
uniform mat4 camMatrix;
// Imports the transformation matrices
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

uniform mat4 lightProjection;



void main()
{
	mat4 BoneTransform = bones_matrix[aBoneIDs[0]];
    //BoneTransform     *= bones_matrix[aBoneIDs[1]];
	//BoneTransform     *= bones_matrix[aBoneIDs[2]];
    BoneTransform     *= bones_matrix[aBoneIDs[3]];

	crntPos = vec3(  (	model  * BoneTransform)	*	vec4(aPos, 1.0f)	);
	//crntPos = vec3(  (	model * translation * -rotation * scale * BoneTransform)	*	vec4(aPos, 1.0f)	);

	Normal = aNormal;
	color = aColor;
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

	fragPosLight = lightProjection * vec4(crntPos, 1.0f);
	
	gl_Position = camMatrix   * vec4(crntPos, 1.0);
}