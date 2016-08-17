#version 330 core
in vec3 vertexColor;
in vec3 position;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

out vec4 color;

void main(void) 
{
	color = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.2);
//	color = texture(Texture1, TexCoord);
//	color = vec4(vertexColor * 2.0f, 1.0f);
}