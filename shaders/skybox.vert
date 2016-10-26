#insert version

layout (location = 0) in vec3 position;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main(void) {
    gl_Position = projection * view * vec4(position, 1.0f);
    TexCoord = position;
}