#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	// outputs final color
	FragColor = texture(tex0, texCoord);
}