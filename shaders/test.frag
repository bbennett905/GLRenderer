#version 330 core
in vec3 vertexColor;
in vec3 position;
in vec2 TexCoord;

uniform vec4 ourColor;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

out vec4 color;

void main(void) 
{
//	color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
//	color = texture(ourTexture1, TexCoord);
	color = vec4(vertexColor * 2.0f
	, 1.0f);
//	color = ourColor;
}