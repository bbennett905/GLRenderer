#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void) 
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	fragPos = vec3(model * vec4(position, 1.0f));
	TexCoord = texCoord;
	normal = mat3(transpose(inverse(model))) * Normal; 
}