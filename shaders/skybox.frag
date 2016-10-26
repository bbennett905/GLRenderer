#insert version

in vec3 TexCoord;

out vec4 color;

uniform samplerCube skybox;

void main(void) {
    color = texture(skybox, TexCoord);
}
