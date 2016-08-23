#version 330 core
in vec3 vertexColor;
in vec3 position;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;

//This matches the C++ Material struct/class
struct Material {
	sampler2D Texture1;
	sampler2D Texture2;
	float MixRatio;
	sampler2D SpecMap;
    float AmbientStrength;
    float DiffuseStrength;
    float SpecularStrength;
    float Shininess;
}; 

uniform Material material;

uniform vec3 viewPos;

//Light Properties - eventually to struct, matching c++
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform float ambientIntensity;
uniform vec3 lightColor;
uniform float lightIntensity;

out vec4 color;

void main(void) 
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diffFactor = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = material.DiffuseStrength * lightIntensity * diffFactor * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = material.SpecularStrength * specFactor * lightColor;  

	vec3 ambient = material.AmbientStrength * ambientIntensity * ambientColor;

	color = vec4((ambient + diffuse) * mix(texture(material.Texture1, TexCoord), texture(material.Texture2, TexCoord), material.MixRatio), 1.0) +
			vec4(specular * texture(material.SpecMap, TexCoord), 1.0);
}